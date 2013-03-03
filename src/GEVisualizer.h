#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "GaussianDistribution.h"

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

struct ProximityEstimate{
    ProximityEstimate(){
        locationID = 0;
        numProximityZones = 0;
        closestActiveZoneID = 0;
        totalNumberUsersVisible = 0;
        closestUserX = 0;
        closestUserY = 0;
        closestUserZ = 0;
    }
    
    double getClosestUserDistanceFromLocation(){
        return sqrt( (closestUserX*closestUserX)+(closestUserY*closestUserY)+(closestUserZ*closestUserZ) );
    }
    unsigned int locationID;
    unsigned int numProximityZones;
    unsigned int closestActiveZoneID;
    unsigned int totalNumberUsersVisible;
    double closestUserX;
    double closestUserY;
    double closestUserZ;
    vector< unsigned int > proximityZonesUserCounts;
    
};
typedef struct ProximityEstimate ProximityEstimate;

struct UserLocationEstimate{
    UserLocationEstimate(){
        userID = 0;
        x = 0;
        y = 0;
        z = 0;
        estimationLikelihood = 0;
    }
    unsigned int userID;
    double x;
    double y;
    double z;
    double estimationLikelihood;
};
typedef struct UserLocationEstimate UserLocationEstimate;

struct UserLocationData{
    UserLocationData(){
        locationID = 0;
    }
    unsigned int locationID;
    vector< UserLocationEstimate > userLocationEstimates;
};
typedef struct UserLocationData UserLocationData;

struct KeyUserLocationEstimate{
    KeyUserLocationEstimate(){
        userID = 0;
        lastObservedLocationID = 0;
        observationLikelihood = 0;
        timeStampAsString = "";
    }
    unsigned int userID;
    unsigned int lastObservedLocationID;
    double observationLikelihood;
    string timeStampAsString;
};
typedef struct KeyUserLocationEstimate KeyUserLocationEstimate;

class GEVisualizer{

public:
    GEVisualizer();
    ~GEVisualizer();
    
    bool connect(string serverIPAddress,unsigned int serverPort,unsigned int listenerPort);
    bool disconnet();
    bool update();
    
    bool sendLocationInfo();
    bool streamUserPresenceData(bool state);
    bool streamUserCountData(bool state);
    bool streamUserProximityData(bool state);
    bool streamUserLocationData(bool state);
    bool streamUserTrackingData(bool state);
    bool streamUserIdentityData(bool state);
    bool streamUserLocationProbabilityData(bool state);
    bool streamKeyUserEstimatedLocationData(bool state);
    
    bool recordRGBImages(bool state);
    bool autoLabelRGBImages(bool state);
    
    vector< LocationInfo >& getLocationInfo();
    vector< PresenceData >& getPresenceData();
    vector< CountData >& getCountData();
    vector< ProximityEstimate >& getProximityData();
    vector< UserLocationData >& getUserLocationData();
    vector< GaussianDistribution >& getUserLocationProbabilityData();
    vector< KeyUserLocationEstimate >& getKeyUserEstimatedLocationData();

    bool getIsConnected() {return connected;}
    
protected:
    bool sendMessage( ofxOscMessage message );
    
    string serverIPAddress;
    unsigned int serverPort;
    unsigned int listenerPort;
    bool connected;
    bool verbose;
    bool streamingUserPresenceData;
    bool streamingUserCountData;
    bool streamingUserProximityData;
    bool streamingUserLocationData;
    bool streamingUserTrackingData;
    bool streamingUserIdentityData;
    bool streamingUserLocationProbabilityData;
    bool streamingKeyUserEstimatedLocationData;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    vector< LocationInfo > locationInfo;
    vector< PresenceData > presenceData;
    vector< CountData > countData;
    vector< ProximityEstimate > proximityData;
    vector< UserLocationData > locationData;
    vector< GaussianDistribution > userLocationProbabilityData;
    vector< KeyUserLocationEstimate > keyUserEstimatedLocationData;
    
};
