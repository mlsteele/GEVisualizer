#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define OFX_UI_NO_XML
#include "ofxUI.h"
#include <boost/regex.hpp>

// #include "ofUi/ofUIMainView.h"
#include "ViewApp.h"
#include "VisConfig.h"
#include "Layout.h"
#include "LayoutRenderer.h"
#include "SkeletonRenderer.h"
#include "LocationData.h"
#include "ofxUIColorSet.h"
// #include "GELink.h"
#include "GEVisualizer.h"


class LayoutApp : public ViewApp {
public:
    // setup
    void setup();
    void setupLayouts();
    void setupUI();
    void setupUIServer();
    void setupUILayouts();
    void setupUIRenderOpts();

    // frame steps
    void update();
    void renderSkeletons2D();
    void draw();

    void exit();

    // interaction callbacks
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // gui callbacks
    void guiEvent(ofxUIEventArgs& e);
    void guiEventLayouts(ofxUIEventArgs& e);
    void guiEventServer(ofxUIEventArgs& e);
    void guiEventRenderOpts(ofxUIEventArgs& e);

    // config
    VisConfig visConfig;

    // ui settings
    const int ofx_theme = 0;
    string titleLabelBase = "Media Lab";

    // operation modes
    LayoutRenderMode mainRenderMode;
    LayoutProjectionDynamic layoutRenderTransform;

    // layout measurements
    const int UI_START_X = ofGetWidth() - 240;
    int UI_START_Y = ofGetHeight() - 100;

    // ui storage
    vector<LayoutRenderer> layoutRenderers;
    ofxUIColorSet uiColors;
    ofxUICanvas* gui;
    ofxUILabel* titleLabel;
    ofTrueTypeFont fontVerd10;
    ofTrueTypeFont fontVerd14;

    // util storage
    GEVisualizer gelink;
    string getMainAppDataDirectory();
    void rescaleAllLayouts(float);
    float lastMouseX = 0;
    float lastMouseY = 0;
    int renderers_active_i;
    float renderers_transition_i;
};
