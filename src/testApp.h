#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofUi/ofUIMainView.h"
#include "Layout.h"
#include "LocationData.h"
#include "GELink.h"

class testApp : public ofBaseApp {
public:
    void setup();
    void setupLayouts();
    void setupUI();
    void setupOSC();

    void update();
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

    ofTrueTypeFont mainAppFont;
    string getMainAppDataDirectory();
    string mainAppDataDirectory;

    ofUIMainView UImainView;
    Layout layout1;
    GELink gelink;
};

void buttonCallback(ofUIButton* button, void* appPointer);
void streamingPresenceInfoCallback(void* appPointer, presenceInfoStreamData data);
