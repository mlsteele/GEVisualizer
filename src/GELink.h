#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofUi/ofUIMainView.h"
#include "Layout.h"
#include "LocationData.h"

typedef struct {
    unsigned int locationID;
    unsigned int presenceEstimate;
    double presenceLikelihood;
} presenceInfoStreamData;

typedef void (*streamingPresenceInfoCallback_t) (void *appPointer, presenceInfoStreamData data);

class GELink {
public:
    void setup();
    void connect(); // 'register' is a c++ keyword :(
    void disconnect();
    void processQueue();

    // takes callback, if null then cancel streaming
    void subscribeToStreamingPresenceInfo(void* app, streamingPresenceInfoCallback_t callback);

private:
    void processOSCMsg(ofxOscMessage& m);
    bool sendMessage(ofxOscMessage message);

    ofxOscSender sender;
    ofxOscReceiver receiver;

    void* streamingPresenceInfoCallbackReferent = NULL;
    streamingPresenceInfoCallback_t streamingPresenceInfoCallback = NULL;

    const bool debug = true;

    // const string ge_server_host = "18.85.55.184";
    // const int ge_server_port = 5000;

    const string ge_server_host = "127.0.0.1";
    const int ge_server_port = 5094;

    const int listening_port = 5087;
};

void buttonCallback(ofUIButton* button, void* appPointer);
