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


// TODO this struct belongs elsewhere
typedef struct {
    ofColor color_back = OFX_UI_COLOR_BACK;
    ofColor color_outline = OFX_UI_COLOR_OUTLINE;
    ofColor color_outline_highlight = OFX_UI_COLOR_OUTLINE_HIGHLIGHT;
    ofColor color_fill = OFX_UI_COLOR_FILL;
    ofColor color_fill_highlight = OFX_UI_COLOR_FILL_HIGHLIGHT;
    ofColor color_padded_rect = OFX_UI_COLOR_PADDED;
    ofColor color_padded_rect_outline = OFX_UI_COLOR_PADDED_OUTLINE;     

    void apply(ofxUICanvas& c) const {
        c.setColorBack(color_back);
        c.setColorOutline(color_outline);
        c.setColorOutlineHighlight(color_outline_highlight);
        c.setColorFill(color_fill);
        c.setColorFillHighlight(color_fill_highlight);
        c.setColorPadded(color_padded_rect);
        c.setColorPaddedOutline(color_padded_rect_outline);
    }
} ofxUIColorSet;


class LayoutApp : public ViewApp {
public:
    void setup();
    void setupLayouts();
    void rescaleAllLayouts(float);
    void setupUI();
    void setupUIServer();
    void setupUILayouts();
    void setupUIRenderOpts();

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

    void guiEvent(ofxUIEventArgs& e);
    void guiEventLayouts(ofxUIEventArgs& e);
    void guiEventServer(ofxUIEventArgs& e);
    void guiEventRenderOpts(ofxUIEventArgs& e);


    ofTrueTypeFont fontVerd10;
    ofTrueTypeFont fontVerd14;
    string getMainAppDataDirectory();
    string mainAppDataDirectory;

    // const int ofx_theme = OFX_UI_THEME_MINBLACK;
    const int ofx_theme = 0;
    ofxUIColorSet uiColors;
    ofxUICanvas* gui;
    string titleLabelBase = "Media Lab";
    ofxUILabel* titleLabel;
    const int UI_START_X = ofGetWidth() - 240;
    int UI_START_Y = ofGetHeight() - 100;

    // ofUIMainView UImainView;
    // ofUISubView UIserverSubView;
    // ofUISubView UIlayoutSubView;

    vector<LayoutRenderer> layoutRenderers;
    LayoutRenderMode mainRenderMode;
    LayoutProjectionDynamic layoutRenderTransform;
    int renderers_active_i;
    float renderers_transition_i;

    float lastMouseX = 0;
    float lastMouseY = 0;

    GEVisualizer gelink;
    bool geFakeData = false;
    bool quickStartup = true; // only loads 1 floor, faster startup, not for production use.

    // const string ge_server_host = "18.133.7.63";
    const string ge_server_host = "18.133.5.116";
    const int ge_server_port = 5000;
    // const string ge_server_host = "127.0.0.1";
    // const int ge_server_port = 5094;
    const int listening_port = 5087;
};
