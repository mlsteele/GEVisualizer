#include "LayoutApp.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>

void LayoutApp::setup(){
    ofSetWindowTitle("Map");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    mainAppDataDirectory = getMainAppDataDirectory();
    fontVerd10.loadFont("verdana.ttf", 10);
    fontVerd14.loadFont("verdana.ttf", 14);

    if (!visConfig.loadFile(mainAppDataDirectory + "VisConfig.txt")) {
        printf("ERR: (fatal) config load failed\n");
        exitApp();
    }

    if (visConfig.fake_data) gelink.ensureFakeData();

    setupLayouts();
    setupUI();

    mainRenderMode.structure = true;
    mainRenderMode.locations = true;
    mainRenderMode.texture = false;
    mainRenderMode.presence = true;
    mainRenderMode.userLocation = true;
    mainRenderMode.skeletons = true;
}

void LayoutApp::setupLayouts() {
    bool reload = layoutRenderers.size() != 0;
    // clear old layouts
    // TODO: clean up memory!
    if (reload) layoutRenderers.clear();

    POINT2D screen_px_corner = {135, 100};
    POINT2D real_corner = {0, 0};
    double screenPixelsPerMeter = 9.5;

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
        newLayout->loadLayoutFiles(mainAppDataDirectory, layout_info_file);
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

void LayoutApp::rescaleAllLayouts(float increment) {
    printf("BAD\n");
    BOOST_FOREACH (LayoutRenderer& lr, layoutRenderers) {
        lr.projection.screenPixelsPerMeter += increment;
        lr.projection.screen_px_corner.x += increment * 25;
        lr.reloadProjection();
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

    ofxUICanvas* gui = new ofxUICanvas(0, 0, 0, 0);
    viewGuis.push_back(ofPtr<ofxUICanvas>(gui));
    // gui->setUIColors(colorBack, colorOutline, colorOutlineHighlight, colorFill, colorFillHighlight, colorPadded, colorPaddedOutline);
    // gui->setUIColors(color_back, color_outline, color_outline_highlight, color_fill, color_fill_highlight, color_padded_rect, color_padded_rect_outline);
    // gui->setTheme(ofx_theme);
    uiColors.apply(*gui);
    // gui->color_back = ofColor(60,60,60);
    ofAddListener(gui->newGUIEvent, this, &LayoutApp::guiEvent);
    titleLabel = new ofxUILabel(titleLabelBase, OFX_UI_FONT_LARGE);
    gui->addWidgetDown(titleLabel);

    setupUIServer();
    setupUILayouts();
    // setupUIRenderOpts();
}

void LayoutApp::setupUIServer() {
    int h = 100;
    int UI_START_Y = ofGetHeight() - 100;
    // ofxUICanvas* serverGui = new ofxUICanvas(0, UI_START_Y, UI_START_X, 100);
    // ofxUICanvas* serverGui = new ofxUICanvas(0, ofGetHeight() - h, ofGetWidth() - 240, h);
    ofxUICanvas* serverGui = new ofxUICanvas(0, UI_START_Y, ofGetWidth() - 240, h);
    // serverGui->setTheme(ofx_theme);
    uiColors.apply(*serverGui);
    // serverGui->color_back = ofColor(60,60,60);
    viewGuis.push_back(ofPtr<ofxUICanvas>(serverGui));
    ofAddListener(serverGui->newGUIEvent, this, &LayoutApp::guiEventServer);

    // TODO make radio
    vector<string> toggleable_attributes;
    toggleable_attributes.push_back("Locations");
    toggleable_attributes.push_back("Texture");
    toggleable_attributes.push_back("Presence");
    toggleable_attributes.push_back("Users");

    ofxUIWidget* cursor;

    cursor = serverGui->addWidgetDown(new ofxUILabelButton(
        "Register", (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL));
    serverGui->addWidgetSouthOf(new ofxUILabelButton(
        "Unregister", (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());

    cursor = serverGui->addWidgetEastOf(new ofxUILabelButton(
        "Stream", (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());
    serverGui->addWidgetSouthOf(new ofxUILabelButton(
        "Unstream", (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());

    BOOST_FOREACH (string attr, toggleable_attributes) {
        cursor = serverGui->addWidgetEastOf(new ofxUILabelButton(
            "Show " + attr, (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());
        serverGui->addWidgetSouthOf(new ofxUILabelButton(
            "Hide " + attr, (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());
    }

}

void LayoutApp::setupUILayouts() {
    int w = 240;
    ofxUICanvas* layoutGui = new ofxUICanvas(UI_START_X, 0, 240, ofGetHeight());
    // layoutGui->setTheme(ofx_theme);
    uiColors.apply(*layoutGui);
    // layoutGui->color_back = ofColor(60,60,60);
    viewGuis.push_back(ofPtr<ofxUICanvas>(layoutGui));
    ofAddListener(layoutGui->newGUIEvent, this, &LayoutApp::guiEventLayouts);

    layoutGui->addWidgetDown(new ofxUIRotarySlider((float) w/4, (float) 0, (float) 360, (float) 0, (string) "Z Spin"));
    layoutGui->addWidgetDown(new ofxUISlider("Zoom", 1, 5, 1, w, 25));
    layoutGui->addWidgetDown(new ofxUILabelButton("Reset", (bool) false, 210, 40, 0, 0, OFX_UI_FONT_MEDIUM));

    vector<string> layoutNames;
    BOOST_FOREACH (LayoutRenderer& layoutRenderer, layoutRenderers) {
        layoutNames.push_back(layoutRenderer.layout->layoutName);
    }

    layoutGui->addWidgetDown(new ofxUIRadio("Floors", layoutNames, 0, 50, 50));

}

// void LayoutApp::skeletonTestPrint() {
//     printf("testing all skeleton data\n");

//     const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
//     printf("how many locations have skeletons? %li\n", allSkelData.size());
//     BOOST_FOREACH (const LocationSkeletonData& locationSkeletons, allSkelData) {
//         printf("how many skeletons at location %i? %li\n", locationSkeletons.locationID, locationSkeletons.userJointData.size());
//         // printf("this LocationSkeletonData is @0x%x\n", &locationSkeletons);
//         BOOST_FOREACH (const SkeletonData& skel, locationSkeletons.userJointData) {
//             printf("how many joints on skeleton of user %i? %li\n", skel.userID, skel.jointData.size());
//             BOOST_FOREACH (const SkeletonJoint& j, skel.jointData) {
//                 printf("joint (%f, %f, %f)     confidence: %f\n", j.x, j.y, j.z, j.confidence);
//             }
//         }
//     }
// }

// void LayoutApp::skeletonTestDraw() {
//     ofPushMatrix();
//     ofTranslate(300, 300, 0);
//     ofTranslate(ofGetMouseX(), ofGetMouseY(), 0);

//     const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
//     BOOST_FOREACH (const LocationSkeletonData& locationSkeletons, allSkelData) {
//         BOOST_FOREACH (const SkeletonData& skel, locationSkeletons.userJointData) {
//             BOOST_FOREACH (const SkeletonJoint& j, skel.jointData) {
//                 ofSetHexColor(0x000000);
//                 // ofSphere(j.x, j.y, j.z, 10);
//                 ofSphere(-j.x, -j.y, 0, 10);
//             }
//         }
//     }

//     ofPopMatrix();
// }


void LayoutApp::update(){
    lastMouseX = ofGetMouseX();
    lastMouseY = ofGetMouseY();

    if (!visConfig.fake_data) gelink.update();
    // skeletonTestPrint();

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

void LayoutApp::draw(){
    ofBackground(0xD8D8D8);

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
    const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
    BOOST_FOREACH (const LocationSkeletonData& locationSkeletons, allSkelData) {
        LayoutRenderer& active_renderer = layoutRenderers[renderers_active_i];
        if (active_renderer.hoverClosestLocationExists && locationSkeletons.locationID == active_renderer.hoverClosestLocationID && active_renderer.hoverClosestLocationDistance < 40) {
            SkeletonRenderer::RenderMode renderMode2D;
            SkeletonRenderer::Projection2D projection2D;

            renderMode2D.node_label_location_index = -1;

            projection2D.view_rect = ofRectangle(20, 40, 230, 230);
            projection2D.real_center.x = 0;
            projection2D.real_center.y = 1.2;
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

void LayoutApp::exit() {
    gelink.disconnect();
    // TODO cleanup gui memory
}

void LayoutApp::guiEvent(ofxUIEventArgs &e ) {
    if(e.widget->getName() == "BACKGROUND VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        ofBackground(slider->getScaledValue());
    }
    else if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());   
    }
}

void LayoutApp::guiEventLayouts(ofxUIEventArgs &e ) {
    if (e.widget->getKind() == OFX_UI_WIDGET_TOGGLE) {
        for (int i = 0; i < layoutRenderers.size(); i++) {
            Layout& layout = *layoutRenderers[i].layout;
            if (boost::ends_with(e.widget->getName(), layout.layoutName)) {
                renderers_active_i = i;
                return;
            }
        }
        printf("WARN: failed to find matching layout for button\n");
    }

    if (e.widget->getName() == "Z Spin") {
        float val = (*(ofxUIRotarySlider*) e.widget).getScaledValue();
        layoutRenderTransform.zRotation = val;
    }

    if (e.widget->getName() == "Zoom") {
        float val = (*(ofxUISlider*) e.widget).getScaledValue();
        layoutRenderTransform.zoomFactor = val;
    }

    if (e.widget->getName() == "Reset") {
        layoutRenderTransform = LayoutProjectionDynamic();

        // TODO get widget foreach
        // BOOST_FOREACH(viewGuis) {
        //     getWidget("Z Spin")->setValue(0);
        //     getWidget("Zoom")->setValue(0);
        // }
    }
}

void LayoutApp::guiEventServer(ofxUIEventArgs &e) {
    // TODO debounce buttons
    if (e.widget->getKind() == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton& btn = *((ofxUILabelButton*)e.widget);
        if (btn.getValue()) {
            printf("%i %i %s\n", btn.getKind(), btn.getValue(), btn.getName().c_str());

            if (btn.getName() == "Register") {
                gelink.connect(visConfig.ge_server_host, visConfig.ge_server_port, visConfig.ge_client_port);
            }

            if (btn.getName() == "Unregister") {
                gelink.disconnect();
            }

            if (btn.getName() == "Stream") {
                gelink.sendLocationInfo();
                gelink.streamUserPresenceData(true);
                gelink.streamUserLocationData(true);
                gelink.streamUserJointData(true);
            }

            if (btn.getName() == "Unstream") {
                gelink.streamUserPresenceData(false);
                gelink.streamUserLocationData(false);
                gelink.streamUserJointData(false);
            }

            if (boost::starts_with(btn.getName(), "Show") || boost::starts_with(btn.getName(), "Hide")) {
                printf("rendermode button pressed ");
                string btn_name = btn.getName();
                printf("(%s) ", btn_name.c_str());
                bool state = boost::starts_with(btn.getName(), "Show");
                printf("[%i] ", state);
                string mode = string(btn.getName().begin() + 5, btn.getName().end());
                printf("(%s)\n", mode.c_str());

                if (mode == "Locations") {
                    mainRenderMode.locations = state;
                }
                else if (mode == "Texture") {
                    mainRenderMode.texture = state;
                }
                else if (mode == "Presence") {
                    mainRenderMode.presence = state;
                }
                else if (mode == "Users") {
                    mainRenderMode.userLocation = state;
                }

                // printf("rmbutton visibility (%s) -> %i\n", button.getButtonID().c_str(), button.isVisible());
                // button.setVisible(!button.isVisible());
                // printf("rmbutton visibility (%s) -> %i\n", button.getButtonID().c_str(), button.isVisible());
            }

        }
    }
}

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
            BOOST_FOREACH (ofPtr<ofxUICanvas> gui, viewGuis) {
                gui->enable();
            }
            break;
        case 'u':
            BOOST_FOREACH (ofPtr<ofxUICanvas> gui, viewGuis) {
                gui->disable();
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

string LayoutApp::getMainAppDataDirectory(){

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
