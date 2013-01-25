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

typedef struct {
    unsigned int locationID;
    unsigned int countEstimate;
    double countLikelihood;
} countStreamData;

typedef void (*streamingPresenceInfoCallback_t) (void *appPointer, presenceInfoStreamData data);
typedef void (*streamingCountCallback_t)        (void *appPointer, countStreamData        data);

class GELink {
public:
    void setup();
    void connect(); // 'register' is a c++ keyword :(
    void disconnect();
    void processQueue();

    void setStreamingCallbackTarget(void* target) { streamingCallbackReferent = target; }
    // takes callback, if null then cancel streaming
    void subscribeToPresenceInfo(streamingPresenceInfoCallback_t callback);
    void subscribeToCount       (streamingCountCallback_t        callback);

    // bool registered = false;

private:
    void processOSCMsg(ofxOscMessage& m);
    bool sendMessage(ofxOscMessage message);

    ofxOscSender sender;
    ofxOscReceiver receiver;

    void* streamingCallbackReferent = NULL;
    streamingPresenceInfoCallback_t streamingPresenceInfoCallback = NULL;
    streamingCountCallback_t        streamingCountCallback        = NULL;

    const bool debug = true;

    const string ge_server_host = "18.85.55.184";
    const int ge_server_port = 5000;

    // const string ge_server_host = "127.0.0.1";
    // const int ge_server_port = 5094;

    const int listening_port = 5087;
};

void buttonCallback(ofUIButton* button, void* appPointer);
