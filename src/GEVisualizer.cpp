#include "GEVisualizer.h"

GEVisualizer::GEVisualizer(){
    verbose = true;
    connected = false;
    streamingServerInfo = false;
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
        
        disconnect();
        
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
    streamingServerInfo = false;
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserProximityData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    streamingKeyUserEstimatedLocationData = false;
    locationInfo.clear();
    userInfo.clear();
    keyUserInfo.clear();
    locationDescriptorInfo.clear();
    presenceData.clear();
    countData.clear();
    proximityData.clear();
    locationData.clear();
    userLocationProbabilityData.clear();
    keyUserEstimatedLocationData.clear();
    userJointData.clear();
    
    ofxOscMessage message;
    message.setAddress("/Register");
    message.addIntArg( (int)listenerPort );
    message.addStringArg("Visualizer@491*cHK!%");
    sendMessage( message );
    
    return true;
}

bool GEVisualizer::disconnect(){
    
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
    streamingServerInfo = false;
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserProximityData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    streamingKeyUserEstimatedLocationData = false;
    locationInfo.clear();
    userInfo.clear();
    keyUserInfo.clear();
    locationDescriptorInfo.clear();
    presenceData.clear();
    countData.clear();
    proximityData.clear();
    locationData.clear();
    userLocationProbabilityData.clear();
    keyUserEstimatedLocationData.clear();
    userJointData.clear();
    
    return true;
}

bool GEVisualizer::update(){
    
    while( receiver.hasWaitingMessages() ){
        
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        bool debug = false;
        
        //printf("Got new message! Address: %s\n",m.getAddress().c_str());
        
        if( m.getAddress() == "/Response" ){
            string responseString = m.getArgAsString(0);
            int status = m.getArgAsInt32(1);
            if( debug ) printf("Got /Response. ResponseString: %s State: %i\n",responseString.c_str(),status);
            
            //TODO - Process response messages
            
        }else if( m.getAddress() == "/ServerInfo" ){
            if( debug ) printf("Got Server Info\n");
            int messageIndex = 0;
            serverInfo.inputDataQueueSize = m.getArgAsInt32(messageIndex++);
            serverInfo.outputDataQueueSize = m.getArgAsInt32(messageIndex++);
            serverInfo.databaseWriterQueueSize = m.getArgAsInt32(messageIndex++);
            serverInfo.dataAnalysisQueueSize = m.getArgAsInt32(messageIndex++);
            serverInfo.numAssignedMemoryBlocks = m.getArgAsInt32(messageIndex++);
            serverInfo.numFreeMemoryBlocks = m.getArgAsInt32(messageIndex++);
            serverInfo.numConnectedClients = m.getArgAsInt32(messageIndex++);
            serverInfo.numConnectedSensorGroups = m.getArgAsInt32(messageIndex++);
            serverInfo.realtimeDataInputDelay = m.getArgAsFloat(messageIndex++);
            serverInfo.realtimeDataOutputDelay = m.getArgAsFloat(messageIndex++);
            serverInfo.totalMemoryUsage = m.getArgAsFloat(messageIndex++);
        
        }else if( m.getAddress() == "/LocationInfo" ){
            if( debug ) printf("Got Location Info\n");
            locationInfo.clear();
            int numLocations = m.getArgAsInt32(0);
            if( numLocations > 0 ){
                locationInfo.resize( numLocations );
                int messageIndex = 1;
                for(int i=0; i<numLocations; i++){
                    locationInfo[i].locationID =  m.getArgAsInt32(messageIndex++);
                    locationInfo[i].locationType =  m.getArgAsInt32(messageIndex++);
                    locationInfo[i].x =  m.getArgAsFloat(messageIndex++);
                    locationInfo[i].y =  m.getArgAsFloat(messageIndex++);
                    locationInfo[i].z =  m.getArgAsFloat(messageIndex++);
                    locationInfo[i].notes =  m.getArgAsString(messageIndex++);
                    if( debug ) printf("LocationInfo: %i %s\n",locationInfo[i].locationID,locationInfo[i].notes.c_str());
                }
            }
        }else if( m.getAddress() == "/UserInfo" ){
            if( debug ) printf("Got User Info\n");
            userInfo.clear();
            int numUsers = m.getArgAsInt32(0);
            if( numUsers > 0 ){
                userInfo.resize( numUsers );
                int messageIndex = 1;
                for(int i=0; i<numUsers; i++){
                    userInfo[i].userID =  m.getArgAsInt32(messageIndex++);
                    userInfo[i].firstName =  m.getArgAsString(messageIndex++);
                    userInfo[i].lastName =  m.getArgAsString(messageIndex++);
                    userInfo[i].screenName =  m.getArgAsString(messageIndex++);
                    userInfo[i].isKeyUser = m.getArgAsInt32(messageIndex++) == 1 ? true : false;
                    userInfo[i].privacySharingState = m.getArgAsInt32(messageIndex++);
                    if( debug ) printf("UserInfo: %i %s\n",userInfo[i].userID,userInfo[i].screenName.c_str());
                }
            }
        }else if( m.getAddress() == "/KeyUserInfo" ){
            if( debug ) printf("Got Key User Info\n");
            keyUserInfo.clear();
            int numUsers = m.getArgAsInt32(0);
            if( numUsers > 0 ){
                keyUserInfo.resize( numUsers );
                int messageIndex = 1;
                for(int i=0; i<numUsers; i++){
                    keyUserInfo[i].userID =  m.getArgAsInt32(messageIndex++);
                    keyUserInfo[i].firstName =  m.getArgAsString(messageIndex++);
                    keyUserInfo[i].lastName =  m.getArgAsString(messageIndex++);
                    keyUserInfo[i].screenName =  m.getArgAsString(messageIndex++);
                    keyUserInfo[i].isKeyUser = true;
                    keyUserInfo[i].privacySharingState = m.getArgAsInt32(messageIndex++);
                    if( debug ) printf("KeyUserInfo: %i %s\n",keyUserInfo[i].userID,keyUserInfo[i].screenName.c_str());
                }
            }
        }else if( m.getAddress() == "/LocationDescriptorInfo" ){
            if( debug ) printf("Got Location Descriptor Info\n");
            locationDescriptorInfo.clear();
            int numDescriptors = m.getArgAsInt32(0);
            if( numDescriptors > 0 ){
                locationDescriptorInfo.resize( numDescriptors );
                int messageIndex = 1;
                for(int i=0; i<numDescriptors; i++){
                   locationDescriptorInfo[i].locationID =  m.getArgAsInt32(messageIndex++);
                   locationDescriptorInfo[i].locationDescriptor =  m.getArgAsString(messageIndex++);
                   if( debug ) printf("LocationDescriptor: %i %s\n",locationDescriptorInfo[i].locationID,locationDescriptorInfo[i].locationDescriptor.c_str());
                }
            }
        }else if( m.getAddress() == "/UserPresenceData" ){
            if( debug ) printf("Got User Presence Data\n");
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
            if( debug ) printf("Got User Count Data\n");
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
            if( debug ) printf("Got User Proximity Data\n");
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
            if( debug ) printf("Got User Location Data\n");
            locationData.clear();
            
            int messageIndex = 0;
            int numLocations = m.getArgAsInt32(messageIndex++);
            
            if( numLocations > 0 ){
                locationData.resize(numLocations);
                
                for(unsigned int i=0; i<numLocations; i++){
                    locationData[i].locationID = m.getArgAsInt32(messageIndex++);
                    int numUsersAtLocation = m.getArgAsInt32(messageIndex++);
                    if( numUsersAtLocation > 0 ){
                        locationData[i].userLocationEstimates.resize(numUsersAtLocation);
                        for(unsigned int j=0; j<numUsersAtLocation; j++){
                            locationData[i].userLocationEstimates[j].userID = m.getArgAsInt32(messageIndex++);
                            locationData[i].userLocationEstimates[j].x = m.getArgAsFloat(messageIndex++);
                            locationData[i].userLocationEstimates[j].y = m.getArgAsFloat(messageIndex++);
                            locationData[i].userLocationEstimates[j].z = m.getArgAsFloat(messageIndex++);
                            locationData[i].userLocationEstimates[j].estimationLikelihood = m.getArgAsFloat(messageIndex++);
                            //printf("Got User Location Data: %f %f %f\n",locationData[i].userLocationEstimates[j].x,locationData[i].userLocationEstimates[j].y,locationData[i].userLocationEstimates[j].z);
                            //printf("estimationLikelihood %f\n", locationData[i].userLocationEstimates[j].estimationLikelihood);

                            const UserLocationEstimate& est = locationData[i].userLocationEstimates[j];
                            userPData[est.userID].userID = est.userID;
                            userPData[est.userID].hasLastSnapshot = true;
                            userPData[est.userID].lastSnapshotTime = ofGetElapsedTimeMillis();
                            userPData[est.userID].lastLocationID = locationData[i].locationID;

                            userPData[est.userID].vx = 0.95 * userPData[est.userID].vx + 0.05 * (est.x - userPData[est.userID].lastEstimate.x);
                            userPData[est.userID].vy = 0.95 * userPData[est.userID].vy + 0.05 * (est.y - userPData[est.userID].lastEstimate.y);

                            userPData[est.userID].lastEstimate = est;

                        }
                    }
                }
            }
        }else if( m.getAddress() == "/UserLocationProbabilityData" ){
            userLocationProbabilityData.clear();
            
            int numFunctions = m.getArgAsInt32(0);
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
                    
                    userLocationProbabilityData[i].init(mu, sigma, weight);
                }
            }
        }else if( m.getAddress() == "/KeyUsersEstimatedLocationData" ){
            if( debug ) printf("Got Key Users Estimated Location Data \n");
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
        }else{
            
            //Parse the userJointData
            // TODO: fix this clear
            // userJointData.clear();
            
            if( locationInfo.size() > 0 ){
                userJointData.resize( locationInfo.size() );
                for(int i=0; i<locationInfo.size(); i++){
                    string address = "/UserJointData/Location_" + ofToString(locationInfo[i].locationID);
                    int messageIndex = 0;
                    if( m.getAddress() == address ){
                        LocationSkeletonData& locationSkeletonData = userJointData[i];
                        // printf("Got UserJointData for location %i\n",locationInfo[i].locationID);
                        locationSkeletonData.locationID = locationInfo[i].locationID;
                        // printf("Stored UserJointData for location %i @0x%x\n", locationSkeletonData.locationID, &(locationSkeletonData));
                        int numUsersAtLocation = m.getArgAsInt32(messageIndex++);
                        
                        if( numUsersAtLocation > 0 ){
                            locationSkeletonData.userJointData.resize( numUsersAtLocation );
                            
                            for(int k=0; k<numUsersAtLocation; k++){
                                locationSkeletonData.userJointData[k].userID = m.getArgAsInt32(messageIndex++);
                                locationSkeletonData.userJointData[k].jointData.resize( 24 );
                                //TODO - need to setup a switch to cover the different joint modes
                                for(int j=0; j<24; j++){
                                    locationSkeletonData.userJointData[k].jointData[j].x = m.getArgAsFloat(messageIndex++);
                                    locationSkeletonData.userJointData[k].jointData[j].y = m.getArgAsFloat(messageIndex++);
                                    locationSkeletonData.userJointData[k].jointData[j].z = m.getArgAsFloat(messageIndex++);
                                    locationSkeletonData.userJointData[k].jointData[j].confidence = m.getArgAsFloat(messageIndex++);
                                }
                            }
                        }
                        
                    }
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

bool GEVisualizer::sendUserInfo(){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send sendUserInfo() message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/SendUserInfo");
    message.addIntArg( (int)listenerPort );
    sendMessage( message );
    return true;
}

bool GEVisualizer::sendKeyUserInfo(){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send sendKeyUserInfo() message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/SendKeyUserInfo");
    message.addIntArg( (int)listenerPort );
    sendMessage( message );
    return true;
}

bool GEVisualizer::sendLocationDescriptorInfo(){
    
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send sendLocationDescriptorInfo() message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/SendLocationDescriptorInfo");
    message.addIntArg( (int)listenerPort );
    sendMessage( message );
    return true;
}

bool GEVisualizer::streamServerInfo(bool state){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamServerInfo(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamServerInfo");
    message.addIntArg( (int)listenerPort );
    message.addIntArg( (state ? 1 : 0) );
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

bool GEVisualizer::streamUserJointData(bool state){
    if( !connected ){
        if( verbose )
            printf("-[GEVisualizer] Can't send streamUserJointData(..) message, not connected!\n");
        return false;
    }
    
    ofxOscMessage message;
    message.setAddress("/StreamUserJointData");
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

ServerInfo GEVisualizer::getServerInfo(){
    return serverInfo;
}

const vector< LocationInfo >& GEVisualizer::getLocationInfo(){
    return locationInfo;
}

const vector< UserInfo >& GEVisualizer::getUserInfo(){
    return userInfo;
}

const vector< UserInfo >& GEVisualizer::getKeyUserInfo(){
    return keyUserInfo;
}

const vector< LocationDescriptorInfo >& GEVisualizer::getLocationDescriptorInfo(){
    return locationDescriptorInfo;
}

const vector< PresenceData >& GEVisualizer::getPresenceData(){
    return presenceData;
}

const vector< CountData >& GEVisualizer::getCountData(){
    return countData;
}

const vector< ProximityEstimate >& GEVisualizer::getProximityData(){
    return proximityData;
}

const vector< UserLocationData >& GEVisualizer::getUserLocationData(){
    return locationData;
}

const vector< GaussianDistribution >& GEVisualizer::getUserLocationProbabilityData(){
    return userLocationProbabilityData;
}

const vector< KeyUserLocationEstimate >& GEVisualizer::getKeyUserEstimatedLocationData(){
    return keyUserEstimatedLocationData;
}

const vector< LocationSkeletonData >& GEVisualizer::getUserJointData(){
    return userJointData;
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