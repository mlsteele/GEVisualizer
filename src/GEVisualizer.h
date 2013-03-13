#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "GaussianDistribution.h"

const int UserPDataBufferLen = 20;

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

struct UserInfo{
    UserInfo(unsigned int userID=0,string firstName="",string lastName="",string screenName="",bool isKeyUser=false){
        this->userID = userID;
        this->firstName = firstName;
        this->lastName = lastName;
        this->screenName = screenName;
        this->isKeyUser = isKeyUser;
    }
    unsigned int userID;
    string firstName;
    string lastName;
    string screenName;
    bool isKeyUser;
};
typedef struct UserInfo UserInfo;

// persistent user data
struct UserPData {
    unsigned int userID;
    bool hasLastSnapshot;
    unsigned int lastSnapshotTime;
    unsigned int lastLocationID;
    // queue<UserPDataBufferLen> lastEstimate;
    UserLocationEstimate lastEstimate;
    double vx;
    double vy;
};
typedef struct UserPData UserPData;

struct LocationDescriptorInfo{
    LocationDescriptorInfo(unsigned int locationID=0,string locationDescriptor=""){
        this->locationID = locationID;
        this->locationDescriptor = locationDescriptor;
    }
    unsigned int mappingID;
    unsigned int locationID;
    string locationDescriptor;
};
typedef struct LocationDescriptorInfo LocationDescriptorInfo;

struct ServerInfo{
    ServerInfo(){
        inputDataQueueSize = 0;
        outputDataQueueSize = 0;
        databaseWriterQueueSize = 0;
        dataAnalysisQueueSize = 0;
        numAssignedMemoryBlocks = 0;
        numFreeMemoryBlocks = 0;
        numConnectedClients = 0;
        numConnectedSensorGroups = 0;
        realtimeDataInputDelay = 0;
        realtimeDataOutputDelay = 0;
        totalMemoryUsage = 0;
    }
    unsigned int inputDataQueueSize;
    unsigned int outputDataQueueSize;
    unsigned int databaseWriterQueueSize;
    unsigned int dataAnalysisQueueSize;
    unsigned int numAssignedMemoryBlocks;
    unsigned int numFreeMemoryBlocks;
    unsigned int numConnectedClients;
    unsigned int numConnectedSensorGroups;
    double realtimeDataInputDelay;
    double realtimeDataOutputDelay;
    double totalMemoryUsage;
};
typedef struct ServerInfo ServerInfo;


class GEVisualizer{

public:
    GEVisualizer();
    ~GEVisualizer();
    
    bool connect(string serverIPAddress,unsigned int serverPort,unsigned int listenerPort);
    bool disconnect();
    bool update();
    
    bool sendLocationInfo();
    bool sendUserInfo();
    bool sendLocationDescriptorInfo();
    bool streamServerInfo(bool state);
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
    
    bool getIsConnected(){ return connected; }
    ServerInfo getServerInfo();
    const vector< LocationInfo >&            getLocationInfo();
    const vector< UserInfo >&                getUserInfo();
    const map< unsigned int, UserPData >&    getUserPData() {return userPData;};
    const vector< LocationDescriptorInfo >&  getLocationDescriptorInfo();
    const vector< UserInfo >&                getKeyUserInfo();
    const vector< PresenceData >&            getPresenceData();
    const vector< CountData >&               getCountData();
    const vector< ProximityEstimate >&       getProximityData();
    const vector< UserLocationData >&        getUserLocationData();
    const vector< GaussianDistribution >&    getUserLocationProbabilityData();
    const vector< KeyUserLocationEstimate >& getKeyUserEstimatedLocationData();
    
protected:
    bool sendMessage( ofxOscMessage message );
    
    string serverIPAddress;
    unsigned int serverPort;
    unsigned int listenerPort;
    bool connected;
    bool verbose;
    bool streamingServerInfo;
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
    
    ServerInfo serverInfo;
    vector< LocationInfo > locationInfo;
    vector< UserInfo > userInfo;
    map< unsigned int, UserPData > userPData;
    vector< LocationDescriptorInfo > locationDescriptorInfo;
    vector< PresenceData > presenceData;
    vector< CountData > countData;
    vector< ProximityEstimate > proximityData;
    vector< UserLocationData > locationData;
    vector< GaussianDistribution > userLocationProbabilityData;
    vector< KeyUserLocationEstimate > keyUserEstimatedLocationData;
    
};
