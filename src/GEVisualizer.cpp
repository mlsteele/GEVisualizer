#include "GEVisualizer.h"

GEVisualizer::GEVisualizer(){
    verbose = true;
    connected = false;
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    streamingKeyUserEstimatedLocationData = false;
    serverIPAddress = "";
    serverPort = 5000;
    listenerPort = 5000;
}

GEVisualizer::~GEVisualizer(){
    
}

bool GEVisualizer::connect(string serverIPAddress,unsigned int serverPort,unsigned int listenerPort){
    
    if( connected ){
        if( verbose ){
            printf("-[GEVisualizer] Already connected, disconnecting first...\n");
        }
        
        disconnet();
        
        if( verbose ){
            printf("-[GEVisualizer] Disconnected\n");
        }
    }
    
    this->serverIPAddress = serverIPAddress;
    this->serverPort = serverPort;
    this->listenerPort = listenerPort;
    
    sender.setup(serverIPAddress, (int)serverPort);
    receiver.setup((int)listenerPort);
    
    connected = true;
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserProximityData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    streamingKeyUserEstimatedLocationData = false;
    locationInfo.clear();
    presenceData.clear();
    countData.clear();
    proximityData.clear();
    locationData.clear();
    userLocationProbabilityData.clear();
    keyUserEstimatedLocationData.clear();
    
    ofxOscMessage message;
    message.setAddress("/Register");
    message.addIntArg( (int)listenerPort );
    message.addStringArg("Visualizer@491*cHK!%");
    sendMessage( message );
    
    return true;
}

bool GEVisualizer::disconnet(){
    
    if( !connected ){
        if( verbose ){
            printf("-[GEVisualizer] Can't disconnect, not connected!\n");
        }
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/Unregister");
    message.addIntArg( (int)listenerPort );
    sendMessage( message );
    
    connected = false;
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserProximityData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    streamingKeyUserEstimatedLocationData = false;
    locationInfo.clear();
    presenceData.clear();
    countData.clear();
    proximityData.clear();
    locationData.clear();
    userLocationProbabilityData.clear();
    keyUserEstimatedLocationData.clear();
    
    return true;
}

bool GEVisualizer::update(){
    
    while( receiver.hasWaitingMessages() ){
        
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        printf("received new message: %s\n",m.getAddress().c_str());
        
        if( m.getAddress() == "/Response" ){
            string responseString = m.getArgAsString(0);
            int status = m.getArgAsInt32(1);
            printf("Got /Response. ResponseString: %s State: %i\n",responseString.c_str(),status);
            
        }else if( m.getAddress() == "/LocationInfo" ){
            printf("Got Location Info\n");
            locationInfo.clear();
            int numLocations = m.getArgAsInt32(0);
            if( numLocations > 0 ){
                locationInfo.resize( numLocations );
                int messageIndex = 1;
                for(int i=0; i<numLocations; i++){
                    locationInfo[i].locationID =  m.getArgAsInt32(messageIndex++);
                    locationInfo[i].locationType =  m.getArgAsInt32(messageIndex++);
                    locationInfo[i].x =  m.getArgAsFloat(messageIndex++);
                    locationInfo[i].x =  m.getArgAsFloat(messageIndex++);
                    locationInfo[i].x =  m.getArgAsFloat(messageIndex++);
                    locationInfo[i].notes =  m.getArgAsString(messageIndex++);
                }
            }
            for (LocationInfo& locationInfo : this->locationInfo) {
                printf("got location info for ID (%i)\n", locationInfo.locationID);
            }
        }else if( m.getAddress() == "/UserPresenceData" ){
            presenceData.clear();
            int numLocations = m.getArgAsInt32(0);
            if( numLocations > 0 ){
                presenceData.resize( numLocations );
                int messageIndex = 1;
                for(int i=0; i<numLocations; i++){
                   presenceData[i].locationID =  m.getArgAsInt32(messageIndex++);
                   presenceData[i].presenceEstimate =  m.getArgAsInt32(messageIndex++);
                   presenceData[i].presenceLikelihood =  m.getArgAsFloat(messageIndex++);
                }
            }
        }else if( m.getAddress() == "/UserCountData" ){
            countData.clear();
            int numLocations = m.getArgAsInt32(0);
            if( numLocations > 0 ){
                countData.resize( numLocations );
                int messageIndex = 1;
                for(int i=0; i<numLocations; i++){
                    countData[i].locationID =  m.getArgAsInt32(messageIndex++);
                    countData[i].countEstimate =  m.getArgAsInt32(messageIndex++);
                    countData[i].countLikelihood =  m.getArgAsFloat(messageIndex++);
                }
            }
        }else if( m.getAddress() == "/UserProximityData" ){
            proximityData.clear();
            int messageIndex = 0;
            int numLocations = m.getArgAsInt32(messageIndex++);
            int numProximityZones = m.getArgAsInt32(messageIndex++);
            if( numLocations > 0 ){
                proximityData.resize( numLocations );
                for(int i=0; i<numLocations; i++){
                    proximityData[i].locationID = m.getArgAsInt32(messageIndex++);
                    proximityData[i].numProximityZones = numProximityZones;
                    proximityData[i].closestActiveZoneID = m.getArgAsInt32(messageIndex++);
                    proximityData[i].totalNumberUsersVisible = m.getArgAsInt32(messageIndex++);
                    proximityData[i].closestUserX = m.getArgAsFloat(messageIndex++);
                    proximityData[i].closestUserY = m.getArgAsFloat(messageIndex++);
                    proximityData[i].closestUserZ = m.getArgAsFloat(messageIndex++);
                    
                    proximityData[i].proximityZonesUserCounts.resize( numProximityZones );
                    for(int j=0; j<numProximityZones; j++){
                        proximityData[i].proximityZonesUserCounts[j] =  m.getArgAsFloat(messageIndex++);
                    }
                }
            }
        }else if( m.getAddress() == "/UserLocationData" ){
            locationData.clear();
            
            int messageIndex = 0;
            int numLocations = m.getArgAsInt32(messageIndex++);
            
            if( numLocations > 0 ){
                locationData.resize(numLocations);
                
                for(unsigned int i=0; i<numLocations; i++){
                    locationData[i].locationID = m.getArgAsInt32(messageIndex++);
                    int numUsersAtLocation = m.getArgAsInt32(messageIndex++);
                    if( numUsersAtLocation > 0 ){
                        printf("resizing locar %i\n", numUsersAtLocation);
                        locationData[i].userLocationEstimates.resize(numUsersAtLocation);
                        for(unsigned int j=0; j<numUsersAtLocation; j++){
                            locationData[i].userLocationEstimates[j].userID = m.getArgAsInt32(messageIndex++);
                            locationData[i].userLocationEstimates[j].x = m.getArgAsFloat(messageIndex++);
                            locationData[i].userLocationEstimates[j].y = m.getArgAsFloat(messageIndex++);
                            locationData[i].userLocationEstimates[j].z = m.getArgAsFloat(messageIndex++);
                            locationData[i].userLocationEstimates[j].estimationLikelihood = m.getArgAsFloat(messageIndex++);
                            //printf("Got User Location Data: %f %f %f\n",locationData[i].userLocationEstimates[j].x,locationData[i].userLocationEstimates[j].y,locationData[i].userLocationEstimates[j].z);
                        }
                    }
                }
            }
            
            
        }else if( m.getAddress() == "/UserLocationProbabilityData" ){
            userLocationProbabilityData.clear();
            
            int numFunctions = m.getArgAsInt32(0);
            printf("Got /UserLocationProbabilityData message! NumFunctions: %i \n",numFunctions);
            if( numFunctions > 0 ){
                userLocationProbabilityData.resize( numFunctions );
                int messageIndex = 1;
                int N = 0;
                double weight;
                vector< double > mu;
                vector< double > sigma;
                
                for(int i=0; i<numFunctions; i++){
                    N = m.getArgAsInt32(messageIndex++);
                    weight = m.getArgAsFloat(messageIndex++);
                    mu.resize(N);
                    sigma.resize(N);
                    for(int j=0; j<N; j++){
                        mu[j] = m.getArgAsFloat(messageIndex++);
                    }
                    for(int j=0; j<N; j++){
                        sigma[j] = m.getArgAsFloat(messageIndex++);
                    }
                    
                    printf("mu: %f %f %f\n",mu[0],mu[1],mu[2]);
                    
                    userLocationProbabilityData[i].init(mu, sigma, weight);
                }
            }
        }else if( m.getAddress() == "/KeyUsersEstimatedLocationData" ){
            keyUserEstimatedLocationData.clear();
            int numKeyUsers = m.getArgAsInt32(0);
            if( numKeyUsers > 0 ){
                keyUserEstimatedLocationData.resize( numKeyUsers );
                int messageIndex = 1;
                for(int i=0; i<numKeyUsers; i++){
                    keyUserEstimatedLocationData[i].userID = m.getArgAsInt32(messageIndex++);
                    keyUserEstimatedLocationData[i].lastObservedLocationID =  m.getArgAsInt32(messageIndex++);
                    keyUserEstimatedLocationData[i].observationLikelihood =  m.getArgAsFloat(messageIndex++);
                    keyUserEstimatedLocationData[i].timeStampAsString = m.getArgAsString(messageIndex++);
                }
            }
        }
        
    }
    
    return true;
}

bool GEVisualizer::sendLocationInfo(){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send sendLocationInfo() message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/SendLocationInfo");
    message.addIntArg( (int)listenerPort );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamUserPresenceData(bool state){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserPresenceData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserPresenceData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamUserCountData(bool state){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserCountData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserCountData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamUserProximityData(bool state){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserProximityData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserProximityData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
    
}

bool GEVisualizer::streamUserLocationData(bool state){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserLocationData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserLocationData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    message.addIntArg( 1 ); //Send global coordinates
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamUserTrackingData(bool state){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserTrackingData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserTrackingData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamUserIdentityData(bool state){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserIdentityData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserIdentityData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamUserLocationProbabilityData(bool state){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserLocationProbabilityData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserLocationProbabilityData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamKeyUserEstimatedLocationData(bool state){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamKeyUserEstimatedLocationData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamKeyUsersEstimatedLocationData");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

bool GEVisualizer::recordRGBImages(bool state){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send recordRGBImages(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/RecordRGBImages");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;

}

bool GEVisualizer::autoLabelRGBImages(bool state){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send autoLabelRGBImages(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/AutoLabelRGBImages");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
    sendMessage( message );
    return true;
}

vector< LocationInfo >& GEVisualizer::getLocationInfo(){
    return locationInfo;
}

vector< PresenceData >& GEVisualizer::getPresenceData(){
    return presenceData;
}

vector< CountData >& GEVisualizer::getCountData(){
    return countData;
}

vector< ProximityEstimate >& GEVisualizer::getProximityData(){
    return proximityData;
}

vector< UserLocationData >& GEVisualizer::getUserLocationData(){
    return locationData;
}

vector< GaussianDistribution >& GEVisualizer::getUserLocationProbabilityData(){
    return userLocationProbabilityData;
}

vector< KeyUserLocationEstimate >& GEVisualizer::getKeyUserEstimatedLocationData(){
    return keyUserEstimatedLocationData;
}

bool GEVisualizer::sendMessage( ofxOscMessage message ){
    
    const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
    
    p << osc::BeginMessage( message.getAddress().c_str() );
	for ( int i=0; i< message.getNumArgs(); ++i )
	{
		if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
			p << message.getArgAsInt32( i );
		else if ( message.getArgType(i) == OFXOSC_TYPE_INT64 )
			p << (osc::int64)message.getArgAsInt64( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			p << message.getArgAsFloat( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
			p << message.getArgAsString( i ).c_str();
		else
		{
			ofLogError() << "bad argument type" + ofToString(message.getArgType( i ));
			assert( false );
		}
	}
	p << osc::EndMessage;
    
    UdpTransmitSocket *socket = sender.getSocket();
    socket->Send( p.Data(), p.Size() );
    
    return true;
}