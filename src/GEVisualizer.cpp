#include "GEVisualizer.h"

GEVisualizer::GEVisualizer(){
    verbose = true;
    connected = false;
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
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
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    
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
    streamingUserPresenceData = false;
    streamingUserCountData = false;
    streamingUserLocationData = false;
    streamingUserTrackingData = false;
    streamingUserIdentityData = false;
    presenceData.clear();
    
    return true;
}

bool GEVisualizer::update(){
    
    while( receiver.hasWaitingMessages() ){
        
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        //printf("Got new message! Address: %s\n",m.getAddress().c_str());
        
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

vector< LocationInfo > GEVisualizer::getLocationInfo(){
    return locationInfo;
}

vector< CountData > GEVisualizer::getCountData(){
    return countData;
}

vector< PresenceData > GEVisualizer::getPresenceData(){
    return presenceData;
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