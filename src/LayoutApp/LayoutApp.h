#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define OFX_UI_NO_XML
#include "ofxUI.h"
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

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

    void setupUI_TopBar();
    void setupUI_ViewControl();
    void setupUI_ViewOptions();
    void setupUI_MapSelect();

    void refreshUI_TopBar();
    void refreshUI_ViewControl();
    void refreshUI_ViewOptions();
    void refreshUI_MapSelect();

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
    void guiEventServer(ofxUIEventArgs& e);
    void guiEventRenderOpts(ofxUIEventArgs& e);
    void guiEvent_TopBar(ofxUIEventArgs& e);
    void guiEvent_ViewControl(ofxUIEventArgs& e);
    void guiEvent_ViewOptions(ofxUIEventArgs& e);
    void guiEvent_MapSelect(ofxUIEventArgs& e);

    // config
    VisConfig visConfig;

    // ui settings
    const int ofx_theme = 0;
    string titleLabelBase = "Media Lab";

    // operation modes
    LayoutRenderMode mainRenderMode;
    LayoutProjectionDynamic layoutRenderTransform;

    // layout measurements
    // TopBar
    const int UI_TOP_BAR_HEIGHT        = 50;
    const int UI_BUTTON_TB_W           = 110;
    const int UI_BUTTON_TB_H           = 32;
    const int UI_TB_MID_SPACEX         = 170;
    const int UI_BUTTON_TB_SERVER_IP_W = 279;
    const int UI_BUTTON_TB_CLI_PORT_W  = 164;
    const int UI_BUTTON_TB_SENSC_W     = 130;
    // ViewControl
    const int UI_VC_WIDTH  = 255;
    const int UI_VC_HEIGHT = 206;
    const int UI_VC_ROTATION_X = 142;
    const int UI_VC_ROTATION_Y = 25;
    const int UI_VC_ROTATION_RAD = 70;
    const int UI_VC_ZOOM_X = 20;
    const int UI_VC_ZOOM_Y = 72;
    const int UI_VC_ZOOM_W = 84;
    const int UI_VC_ZOOM_H = 24;
    const int UI_VC_BTNS_Y = 150;
    const int UI_VC_BTN_W = 119;
    const int UI_VC_BTN_H = 32;
    // ViewOptions
    const int UI_VOPTS_WIDTH = 150;
    const int UI_VOPTS_HEIGHT = 300;
    const int UI_VOPTS_BTN_W = 125;
    // Map Select
    const int UI_MAPS_WIDTH  = UI_VC_WIDTH;
    const int UI_MAPS_HEIGHT = ofGetHeight() - UI_VC_HEIGHT - UI_TOP_BAR_HEIGHT;
    // Skeleton View
    const int UI_SKELV_W = 317;
    const int UI_SKELV_H = 262;


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
