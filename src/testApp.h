#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Layout.h"
#include "LocationData.h"

class testApp : public ofBaseApp {
public:
    void setup();
    void update();
    void processOSCQueue();
    void processOSCMsg(ofxOscMessage& m);
    void attemptToSetPresenceInfo(int location_id, int new_presence_info);
    void draw();
    void exit();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    string getMainAppDataDirectory();
    string mainAppDataDirectory;

    Layout layout1;

    ofxOscSender clientSender;
    ofxOscReceiver clientReceiver;

    const string ge_server_host = "127.0.0.1";
    const int ge_server_port = 5094;
    const int listening_port = 5087;
};
