#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
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

    string layout_info_files[4] = {
        "E14_1_LayoutInfo.txt" ,
        "E14_4_LayoutInfo.txt" ,
        "E14_5_LayoutInfo.txt" ,
        "E14_6_LayoutInfo.txt" };

    ofUIMainView UImainView;
    vector<LayoutRenderer> layoutRenderers;
    LayoutRenderer* active_layout_renderer;

    GEVisualizer gelink;

    const string ge_server_host = "18.85.55.184";
    const int ge_server_port = 5000;
    // const string ge_server_host = "127.0.0.1";
    // const int ge_server_port = 5094;
    const int listening_port = 5087;
};

void buttonCallback(ofUIButton* button, void* appPointer);
