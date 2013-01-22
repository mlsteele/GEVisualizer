#include "GELink.h"

void GELink::setup() {
    sender.setup("127.0.0.1", ge_server_port);
    receiver.setup(listening_port);
}

void GELink::connect() {
    printf("registering with GE server... ");
    ofxOscMessage m;
    m.setAddress("/Register");
    m.addIntArg((int)listening_port);
    m.addStringArg("Visualizer@491*cHK!%"); // password
    sendMessage(m);
    printf("done\n");
}

void GELink::disconnect() {
    printf("unregistering with GE server... ");
    ofxOscMessage m;
    m.setAddress("/Unregister");
    m.addIntArg((int)listening_port);
    sendMessage(m);
    printf("done\n");
}

void GELink::processQueue() {
    // process OSC queue
    ofxOscMessage m;
    while (receiver.hasWaitingMessages()) {
        receiver.getNextMessage(&m);
        processOSCMsg(m);
    }
}

void GELink::subscribeToStreamingPresenceInfo(void* app, streamingPresenceInfoCallback_t callback) {
    if (app != NULL && callback != NULL) {
        printf("registering for streaming presence info... ");
        streamingPresenceInfoCallbackReferent = app;
        streamingPresenceInfoCallback = callback;
        ofxOscMessage m;
        m.setAddress("/StreamUserPresenceData");
        m.addIntArg( (int)listening_port );
        m.addIntArg( (int)1 );
        sendMessage(m);
        printf("done\n");
    } else {
        printf("unregistering for streaming presence info... ");
        streamingPresenceInfoCallbackReferent = NULL;
        streamingPresenceInfoCallback = NULL;
        ofxOscMessage m;
        m.setAddress("/StreamUserPresenceData");
        m.addIntArg( (int)listening_port );
        m.addIntArg( (int)0 );
        sendMessage(m);
        printf("done\n");
    }
}

void GELink::processOSCMsg(ofxOscMessage& m) {
    if (debug) {
        printf("received OSC message\n");
        printf("from: %s:%i\n", m.getRemoteIp().c_str(), m.getRemotePort());
        printf("address: %s\n", m.getAddress().c_str());
        printf("args[%u]\n", m.getNumArgs());
    }

    if (m.getAddress() == "/UserPresenceData") {
        printf("dump /UserPresenceData -> ");
        for(int i = 0; i < m.getNumArgs(); i++) {
            printf(" %i", m.getArgAsInt32(i));
        }
        printf("\n");

        if (streamingPresenceInfoCallback != NULL) {
            int numLocations = m.getArgAsInt32(0);
            int messageIndex = 1;
            for(int i = 0; i < numLocations; i++) {
                streamingPresenceInfoCallback( &streamingPresenceInfoCallbackReferent, (presenceInfoStreamData){
                    m.getArgAsInt32(messageIndex++) ,
                    m.getArgAsInt32(messageIndex++) ,
                    m.getArgAsFloat(messageIndex++) });
            }
        }
    } else {
        printf("WARN: ignoring OSC message (%s) [nArgs:%i].\n", m.getAddress().c_str(), m.getNumArgs());
    }
}

bool GELink::sendMessage( ofxOscMessage message ){
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
