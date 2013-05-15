#include "LayoutApp.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>

void LayoutApp::setup(){
    ofSetWindowTitle("Map");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    fontVerd10.loadFont("verdana.ttf", 10);
    fontVerd14.loadFont("verdana.ttf", 14);

    if (!visConfig.loadFile(getMainAppDataDirectory() + "VisConfig.txt")) {
        printf("ERR: (fatal) config load failed\n");
        exitApp();
    }

    if (visConfig.fake_data) gelink.ensureFakeData();

    setupLayouts();
    setupUI();

    mainRenderMode.structure    = true;
    mainRenderMode.locations    = true;
    mainRenderMode.texture      = false;
    mainRenderMode.presence     = true;
    mainRenderMode.userLocation = true;
    mainRenderMode.skeletons    = true;
}

void LayoutApp::setupLayouts() {
    bool reload = layoutRenderers.size() != 0;
    // clear old layouts
    // TODO: clean up memory!
    if (reload) layoutRenderers.clear();

    // TODO this screen corner sucks
    POINT2D screen_px_corner    = {5, UI_TOP_BAR_HEIGHT + 50};
    POINT2D real_corner         = {-14, -8.5};
    double screenPixelsPerMeter = 9;

    vector<string> layout_info_files;
    if (visConfig.quick_start) {
        layout_info_files.push_back("E14_5_LayoutInfo.txt");
    } else {
        layout_info_files.push_back("E14_6_LayoutInfo.txt");
        layout_info_files.push_back("E14_5_LayoutInfo.txt");
        layout_info_files.push_back("E14_4_LayoutInfo.txt");
        layout_info_files.push_back("E14_3_LayoutInfo.txt");
        layout_info_files.push_back("E14_2_LayoutInfo.txt");
        layout_info_files.push_back("E14_1_LayoutInfo.txt");
    }

    BOOST_FOREACH (string layout_info_file, layout_info_files) {
        Layout* newLayout = new Layout();
        newLayout->loadLayoutFiles(getMainAppDataDirectory(), layout_info_file);
        layoutRenderers.push_back(LayoutRenderer());
        layoutRenderers.back().attachLayout(newLayout);
        layoutRenderers.back().attachFonts(&fontVerd10, &fontVerd14);
        layoutRenderers.back().setupProjection(screen_px_corner, real_corner, screenPixelsPerMeter);
    }

    if (!reload) {
        if (visConfig.quick_start) {
            renderers_active_i = 0;
            renderers_transition_i = renderers_active_i;
        } else {
            renderers_active_i = 1;
            renderers_transition_i = renderers_active_i;
        }
    }
}

void LayoutApp::setupUI() {
    uiColors.color_back = ofColor(50, 50, 50, 75);
    // uiColors.color_back = OFX_UI_COLOR_BACK;

    // ofColor color_back = OFX_UI_COLOR_BACK;
    // ofColor color_outline = OFX_UI_COLOR_OUTLINE;
    // ofColor color_outline_highlight = OFX_UI_COLOR_OUTLINE_HIGHLIGHT;
    // ofColor color_fill = OFX_UI_COLOR_FILL;
    // ofColor color_fill_highlight = OFX_UI_COLOR_FILL_HIGHLIGHT;
    // ofColor color_padded_rect = OFX_UI_COLOR_PADDED;
    // ofColor color_padded_rect_outline = OFX_UI_COLOR_PADDED_OUTLINE;     

    // ofColor colorBack             (0x444444, 0);
    // ofColor colorOutline          (0xdddddd, 255);
    // ofColor colorOutlineHighlight (0xcccccc, 255);
    // ofColor colorFill             (0xaaaaaa, 255);
    // ofColor colorFillHighlight    (0x999999, 255);
    // ofColor colorPadded           (0x110000, 255);
    // ofColor colorPaddedOutline    (0x001100, 255);

    ofxUICanvas* gui = new ofxUICanvas(0, UI_TOP_BAR_HEIGHT, 0, 0);
    viewGuis["main"] = ofPtr<ofxUICanvas>(gui);
    // gui->setUIColors(colorBack, colorOutline, colorOutlineHighlight, colorFill, colorFillHighlight, colorPadded, colorPaddedOutline);
    // gui->setUIColors(color_back, color_outline, color_outline_highlight, color_fill, color_fill_highlight, color_padded_rect, color_padded_rect_outline);
    // gui->setTheme(ofx_theme);
    uiColors.apply(*gui);
    // gui->color_back = ofColor(60,60,60);
    ofAddListener(gui->newGUIEvent, this, &LayoutApp::guiEvent);
    titleLabel = new ofxUILabel(titleLabelBase, OFX_UI_FONT_LARGE);
    gui->addWidgetDown(titleLabel);

    setupUI_TopBar();
    setupUI_ViewControl();
    setupUI_ViewOptions();
    setupUI_MapSelect();
}

void LayoutApp::update(){
    lastMouseX = ofGetMouseX();
    lastMouseY = ofGetMouseY();

    if (!visConfig.fake_data) gelink.update();

    // transition to approach selection
    static const float transition_speed_1 = 0.06;
    const float transition_diff_mag = fabs(renderers_transition_i - renderers_active_i);
    if (transition_diff_mag < transition_speed_1) {
        renderers_transition_i = renderers_active_i;
    } else {
        const float transition_speed = fmax(transition_diff_mag / 10, transition_speed_1);
        renderers_transition_i += (renderers_active_i - renderers_transition_i) > 0 ? transition_speed : -transition_speed;
    }

    // update renderer transformations
    BOOST_FOREACH (LayoutRenderer& renderer, layoutRenderers) {
        renderer.projection.dyn = layoutRenderTransform;
    }

    // update label
    titleLabel->setLabel(titleLabelBase + " - " + layoutRenderers[renderers_active_i].layout->layoutName);
}

void LayoutApp::renderSkeletons2D() {
    const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
    BOOST_FOREACH (const LocationSkeletonData& locationSkeletons, allSkelData) {
        LayoutRenderer& active_renderer = layoutRenderers[renderers_active_i];
        if (active_renderer.hoverClosestLocationExists && locationSkeletons.locationID == active_renderer.hoverClosestLocationID && active_renderer.hoverClosestLocationDistance < 40) {
            SkeletonRenderer::RenderMode renderMode2D;
            SkeletonRenderer::Projection2D projection2D;

            renderMode2D.node_label_location_index = -1;

            projection2D.view_rect = ofRectangle(0, ofGetHeight() - UI_SKELV_H, UI_SKELV_W, UI_SKELV_H);
            projection2D.real_center.x = 0;
            projection2D.real_center.y = 0;
            projection2D.screenPixelsPerMeter = 70;
            bool clear = true;

            BOOST_FOREACH (const SkeletonData& skel, locationSkeletons.userJointData) {
                SkeletonRenderer::render2D(
                    skel, 
                    renderMode2D, 
                    projection2D,
                    // ofPtr<ofTrueTypeFont>(&fontVerd10),
                    fontVerd10,
                    clear // clear
                );

                clear = false;
            }
        }
    }
}

void LayoutApp::draw(){
    ofBackground(0xD8D8D8);
    // ((ofxUIScrollableCanvas*) &*viewGuis["MapSelect"])->drawScrollableRect();

    refreshUI_TopBar();
    refreshUI_ViewOptions();

    // render layouts
    for (int i = 0; i < layoutRenderers.size(); i++) {
        float transition = i - renderers_transition_i;
        if (fabs(transition) < .97) {
            LayoutRenderMode activeRenderMode = mainRenderMode;
            const bool really_nearby = fabs(transition) < 0.2;
            activeRenderMode.locations    &= really_nearby;
            activeRenderMode.texture      &= really_nearby;
            activeRenderMode.presence     &= really_nearby;
            activeRenderMode.userLocation &= really_nearby;
            layoutRenderers[i].render(activeRenderMode, gelink, transition);
        }
    }

    // render skeletons for current location in 2d
    if (mainRenderMode.skeletons) renderSkeletons2D();
}

void LayoutApp::exit() {
    gelink.disconnect();
    // TODO cleanup gui memory
}

void LayoutApp::guiEvent(ofxUIEventArgs &e ) {}

void LayoutApp::keyPressed(int key){
    // UImainView.keyPressedEvent(key);

    switch( key ){
        case 'q':
            exitApp();
            break;
        case 'r':
            setupLayouts();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case '=':
            rescaleAllLayouts(1);
            break;
        case '-':
            rescaleAllLayouts(-1);
            break;
        case 'x':
            layoutRenderers[renderers_active_i].projection.print();
            // layoutRenderTransform = LayoutProjectionDynamic();
            break;
        case 'y':
            BOOST_FOREACH(viewGui_t viewGuiPair, viewGuis) {
                viewGuiPair.second->enable();
            }
            break;
        case 'u':
            BOOST_FOREACH(viewGui_t viewGuiPair, viewGuis) {
                viewGuiPair.second->disable();
            }
            break;
        case 'm':
            if (visConfig.fake_data) gelink.update();
            break;
        default:
            printf("Key Pressed: %i\n", key);
            break;
    }
}

void LayoutApp::keyReleased(int key) {}

void LayoutApp::mouseMoved(int x, int y ) {}

void LayoutApp::mouseDragged(int x, int y, int button) {
    // drag layouts
    // TODO make this less dumb
    LayoutRenderer& active_renderer = layoutRenderers[renderers_active_i];
    bool inLayoutViewBounds = (x > 0) && (x < UI_START_X) && (y > 0) && (y < UI_START_Y);

    if (inLayoutViewBounds) {
        float pandx = (ofGetMouseX() - lastMouseX);
        float pandy = (ofGetMouseY() - lastMouseY);
        LayoutProjectionDynamic& dyn = layoutRenderTransform;
        dyn.pan.x += pandx * cos(-dyn.zRotation / 180. * M_PI) - pandy * sin(-dyn.zRotation / 180. * M_PI);
        dyn.pan.y += pandx * sin(-dyn.zRotation / 180. * M_PI) + pandy * cos(-dyn.zRotation / 180. * M_PI);
    }

}

void LayoutApp::mousePressed(int x, int y, int button) {}

void LayoutApp::mouseReleased(int x, int y, int button) {}

void LayoutApp::windowResized(int w, int h) {}

void LayoutApp::gotMessage(ofMessage msg) {}

void LayoutApp::dragEvent(ofDragInfo dragInfo) {}

void LayoutApp::rescaleAllLayouts(float increment) {
    printf("BAD rescaleAllLayouts\n");
    BOOST_FOREACH (LayoutRenderer& lr, layoutRenderers) {
        lr.projection.screenPixelsPerMeter += increment;
        lr.projection.screen_px_corner.x += increment * 25;
        lr.reloadProjection();
    }
}

string LayoutApp::getMainAppDataDirectory() {
    string mainAppDataDirectory = "";
    ofFilePath filePath;
    string workingDir = filePath.getCurrentWorkingDirectory();
    mainAppDataDirectory = "";
    size_t index = workingDir.rfind( "/bin/" );
    if( index != string::npos ){
        mainAppDataDirectory = workingDir.substr(0,index) + "/bin/data/";
        printf("mainAppDataDirectory: %s\n", mainAppDataDirectory.c_str());
    }else{
        mainAppDataDirectory = "";
        size_t index = workingDir.rfind( ".app/" );
        if( index != string::npos ){
            //Remove the name of the app
            size_t appNameStartindex = workingDir.rfind( "/", index );
            if( appNameStartindex != string::npos ){
                mainAppDataDirectory = workingDir.substr(0,appNameStartindex) + "/data/";
            }else mainAppDataDirectory = "";
        }else mainAppDataDirectory = workingDir;
    }

    return mainAppDataDirectory;
}
