#include "GELink.h"

void GELink::setup() {
    sender.setup(ge_server_host, ge_server_port);
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

void subscribe_helper(GELink& gelink, string address, bool state) {
    printf("%s for streaming %s... ", (state ? "registering" : "unregistering"), address);
    ofxOscMessage m;
    m.setAddress("/StreamUserPresenceData");
    m.addIntArg( (int)gelink.listening_port );
    m.addIntArg( (int)(state ? 1 : 0) );
    gelink.sendMessage(m);
    printf("done\n");
}

void GELink::subscribeToPresenceInfo(streamingPresenceInfoCallback_t callback) {
    if (streamingCallbackReferent == NULL) printf("ERROR: tried to subscribe against empty callback target\n");
    streamingPresenceInfoCallback = callback;
    subscribe_helper(*this, "/StreamUserPresenceData", callback != NULL);
}

void GELink::subscribeToCount(streamingCountCallback_t callback) {
    if (streamingCallbackReferent == NULL) printf("ERROR: tried to subscribe against empty callback target\n");
    streamingCountCallback = callback;
    subscribe_helper(*this, "/StreamUserCountData", callback != NULL);
}

void GELink::processOSCMsg(ofxOscMessage& m) {
    if (debug) {
        printf("received OSC message\n");
        printf("from: %s:%i\n", m.getRemoteIp().c_str(), m.getRemotePort());
        printf("address: %s\n", m.getAddress().c_str());
        printf("args[%u]\n", m.getNumArgs());
    }

    // printf("<procOSC> [A] locationStreams.size() -> %i\n", layout_debug->locationStreams.size());

    // if (m.getAddress() == "Response" && m.getArgAsString(0) == "/Register") {
    //     printf("received response \n");
    // }

    if (m.getAddress() == "/UserPresenceData") {
        printf("recvd /UserPresenceData\n");
        if (streamingCallbackReferent != NULL && streamingPresenceInfoCallback != NULL) {
            int numLocations = m.getArgAsInt32(0);
            for(int i = 0; i < numLocations; i += 3) {
                unsigned int locationID       = m.getArgAsInt32(i + 1);
                unsigned int presenceEstimate = m.getArgAsInt32(i + 2);
                double presenceLikelihood     = m.getArgAsFloat(i + 3);

                streamingPresenceInfoCallback( streamingCallbackReferent, (presenceInfoStreamData) {
                    locationID ,
                    presenceEstimate ,
                    presenceLikelihood });
            }
        }
    } else if (m.getAddress() == "/UserCountData") {
        printf("recvd /UserCountData\n");
        if (streamingCallbackReferent != NULL && streamingCountCallback != NULL) {
            int numLocations = m.getArgAsInt32(0);
            int messageIndex = 1;
            for(int i = 0; i < numLocations; i++) {
                presenceCountStreamData
                unsigned int locationID      = m.getArgAsInt32(i + 1);
                unsigned int countEstimate   = m.getArgAsInt32(i + 2);
                double countLikelihood       = m.getArgAsFloat(i + 3);
                streamingCountCallback( streamingCallbackReferent, (countStreamData) {
                    locationID ,
                    countEstimate ,
                    countLikelihood });
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
