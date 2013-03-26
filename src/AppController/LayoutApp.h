#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define OFX_UI_NO_XML
#include "ofxUI.h"
#include <boost/regex.hpp>

// #include "ofUi/ofUIMainView.h"
#include "ViewApp.h"
#include "Layout.h"
#include "LayoutRenderer.h"
#include "SkeletonRenderer.h"
#include "LocationData.h"
// #include "GELink.h"
#include "GEVisualizer.h"

class LayoutApp : public ViewApp {
public:
    void setup();
    void setupLayouts();
    void rescaleAllLayouts(float);
    void setupUI();
    void setupUIServer();
    void setupUILayouts();

    void update();
    void draw();
    void exit();

    void skeletonTestPrint();
    void skeletonTestDraw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void guiEvent(ofxUIEventArgs &e);
    void guiEventLayouts(ofxUIEventArgs &e);
    void guiEventServer(ofxUIEventArgs &e);


    ofTrueTypeFont fontVerd10;
    ofTrueTypeFont fontVerd14;
    string getMainAppDataDirectory();
    string mainAppDataDirectory;

    ofxUICanvas* gui;
    string titleLabelBase = "Media Lab";
    ofxUILabel* titleLabel;

    // ofUIMainView UImainView;
    // ofUISubView UIserverSubView;
    // ofUISubView UIlayoutSubView;

    vector<LayoutRenderer> layoutRenderers;
    LayoutRenderMode mainRenderMode;
    LayoutProjectionDynamic layoutRenderTransform;
    int renderers_active_i;
    float renderers_transition_i;

    SkeletonRenderer skelRenderer;
    SkeletonRenderMode skelRenderMode;
    // SkeletonProjectionDynamic renderTransform;

    float lastMouseX = 0;
    float lastMouseY = 0;

    GEVisualizer gelink;
    bool geFakeData = false;
    bool quickStartup = false; // only loads 1 floor, faster startup, not for production use.

    const string ge_server_host = "18.133.7.63";
    const int ge_server_port = 5000;
    // const string ge_server_host = "127.0.0.1";
    // const int ge_server_port = 5094;
    const int listening_port = 5087;
};
