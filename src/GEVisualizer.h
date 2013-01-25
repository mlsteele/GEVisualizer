#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

struct LocationInfo{
    LocationInfo(){
        locationID = 0;
        locationType = 0;
        x = 0;
        y = 0;
        z = 0;
        notes = "";
    }
    unsigned int locationID;
    unsigned int locationType;
    double x;
    double y;
    double z;
    string notes;

    enum locationTypeVals{DEFAULT_NULL_LOCATION=0,GI_DISPLAY=1};
};
typedef struct LocationInfo LocationInfo;

struct PresenceData{
    PresenceData(){
        locationID = 0;
        presenceEstimate = 0;
        presenceLikelihood = 0;
    }
    unsigned int locationID;
    unsigned int presenceEstimate;
    double presenceLikelihood;
    
};
typedef struct PresenceData PresenceData;

struct CountData{
    CountData(){
        locationID = 0;
        countEstimate = 0;
        countLikelihood = 0;
    }
    unsigned int locationID;
    unsigned int countEstimate;
    double countLikelihood;
    
};
typedef struct CountData CountData;

class GEVisualizer{

public:
    GEVisualizer();
    ~GEVisualizer();
    
    bool connect(string serverIPAddress,unsigned int serverPort,unsigned int listenerPort);
    bool disconnect();
    bool update();
    
    bool sendLocationInfo();
    bool streamUserPresenceData(bool state);
    bool streamUserCountData(bool state);
    bool streamUserLocationData(bool state);
    bool streamUserTrackingData(bool state);
    bool streamUserIdentityData(bool state);
    
    vector< LocationInfo > getLocationInfo();
    vector< PresenceData > getPresenceData();
    vector< CountData > getCountData();
    
protected:
    bool sendMessage( ofxOscMessage message );
    
    string serverIPAddress;
    unsigned int serverPort;
    unsigned int listenerPort;
    bool connected;
    bool verbose;
    bool streamingUserPresenceData;
    bool streamingUserCountData;
    bool streamingUserLocationData;
    bool streamingUserTrackingData;
    bool streamingUserIdentityData;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    vector< LocationInfo > locationInfo;
    vector< PresenceData > presenceData;
    vector< CountData > countData;
    
};
