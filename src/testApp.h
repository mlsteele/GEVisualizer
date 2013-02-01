#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
// #define OFX_UI_NO_XML
// #include "ofxUI.h"

#include "ofUi/ofUIMainView.h"
#include "Layout.h"
#include "LayoutRenderer.h"
#include "LocationData.h"
// #include "GELink.h"
#include "GEVisualizer.h"

class testApp : public ofBaseApp {
public:
    void setup();
    void setupLayouts();
    void setupUI();
    void setupUIServer();
    void setupUILayouts();

    void update();
    void draw();
    void exit();

    // void guiEvent(ofxUIEventArgs &e);
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

    // ofxUICanvas* gui;

    ofUIMainView UImainView;
    vector<LayoutRenderer> layoutRenderers;
    int renderers_active_i;
    float renderers_transition_i;

    GEVisualizer gelink;

    const string ge_server_host = "18.85.55.184";
    const int ge_server_port = 5000;
    // const string ge_server_host = "127.0.0.1";
    // const int ge_server_port = 5094;
    const int listening_port = 5087;
};

void buttonCallback(ofUIButton* button, void* appPointer);
