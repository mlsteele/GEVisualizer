#include "testApp.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>

void testApp::setup(){
    ofSetWindowTitle("Map");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    mainAppDataDirectory = getMainAppDataDirectory();
    fontVerd10.loadFont("verdana.ttf", 10);
    fontVerd14.loadFont("verdana.ttf", 14);

    setupLayouts();
    setupUI();

    { // setup skeleton renderer
        POINT2D screen_px_corner = {450, 100};
        POINT3D real_corner = {0, 0};
        double screenPixelsPerMeter = 200;
        skelRenderer.setupProjection(screen_px_corner, real_corner, screenPixelsPerMeter);
        skelRenderer.attachFonts(&fontVerd10);
    }

    mainRenderMode.structure = true;
    mainRenderMode.locations = true;
    mainRenderMode.texture = false;
    mainRenderMode.presence = true;
    mainRenderMode.userLocation = true;
}

void testApp::setupLayouts() {
    bool reload = layoutRenderers.size() != 0;
    // clear old layouts
    // TODO: clean up memory!
    if (reload) layoutRenderers.clear();

    POINT2D screen_px_corner = {135, 100};
    POINT2D real_corner = {0, 0};
    double screenPixelsPerMeter = 9.5;

    vector<string> layout_info_files;
    layout_info_files.push_back("E14_6_LayoutInfo.txt");
    layout_info_files.push_back("E14_5_LayoutInfo.txt");
    layout_info_files.push_back("E14_4_LayoutInfo.txt");
    layout_info_files.push_back("E14_3_LayoutInfo.txt");
    layout_info_files.push_back("E14_2_LayoutInfo.txt");
    layout_info_files.push_back("E14_1_LayoutInfo.txt");

    for (string layout_info_file : layout_info_files) {
        Layout* newLayout = new Layout();
        newLayout->loadLayoutFiles(mainAppDataDirectory, layout_info_file);
        layoutRenderers.push_back(LayoutRenderer());
        layoutRenderers.back().attachLayout(newLayout);
        layoutRenderers.back().attachFonts(&fontVerd10, &fontVerd14);
        layoutRenderers.back().setupProjection(screen_px_corner, real_corner, screenPixelsPerMeter);
    }

    if (!reload) {
        renderers_active_i = 1;
        renderers_transition_i = renderers_active_i;
    }
}

void testApp::rescaleAllLayouts(float increment) {
    for (LayoutRenderer& lr : layoutRenderers) {
        lr.projection.screenPixelsPerMeter += increment;
        lr.projection.screen_px_corner.x += increment * 25;
        lr.reloadProjection();
    }
}

void testApp::setupUI() {
    // ofColor colorBack             (0x444444, 0);
    // ofColor colorOutline          (0xdddddd, 255);
    // ofColor colorOutlineHighlight (0xcccccc, 255);
    // ofColor colorFill             (0xaaaaaa, 255);
    // ofColor colorFillHighlight    (0x999999, 255);
    // ofColor colorPadded           (0x110000, 255);
    // ofColor colorPaddedOutline    (0x001100, 255);

    gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
    // gui->setUIColors(colorBack, colorOutline, colorOutlineHighlight, colorFill, colorFillHighlight, colorPadded, colorPaddedOutline);
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    titleLabel = new ofxUILabel(titleLabelBase, OFX_UI_FONT_LARGE);
    gui->addWidgetDown(titleLabel);

    setupUIServer();
    setupUILayouts();
}

void testApp::setupUIServer() {
    float h = 100;
    ofxUICanvas* serverGui = new ofxUICanvas(0, ofGetHeight() - h, ofGetWidth() - 240, h);
    gui->addWidget(serverGui);
    ofAddListener(serverGui->newGUIEvent, this, &testApp::guiEventServer);

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

    for (string attr : toggleable_attributes) {
        cursor = serverGui->addWidgetEastOf(new ofxUILabelButton(
            "Show " + attr, (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());
        serverGui->addWidgetSouthOf(new ofxUILabelButton(
            "Hide " + attr, (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL), cursor->getName());
    }

}

void testApp::setupUILayouts() {
    float w = 240;
    ofxUICanvas* layoutGui = new ofxUICanvas(ofGetWidth() - w, 0, w, ofGetHeight());
    gui->addWidget(layoutGui);
    ofAddListener(layoutGui->newGUIEvent, this, &testApp::guiEventLayouts);

    layoutGui->addWidgetDown(new ofxUIRotarySlider((float) w/4, (float) 0, (float) 360, (float) 0, (string) "Z Spin"));
    layoutGui->addWidgetDown(new ofxUISlider("Zoom", 1, 5, 1, w, 25));
    layoutGui->addWidgetDown(new ofxUILabelButton("Reset", (bool) false, 210, 40, 0, 0, OFX_UI_FONT_MEDIUM));

    vector<string> layoutNames;
    for (LayoutRenderer& layoutRenderer : layoutRenderers) {
        layoutNames.push_back(layoutRenderer.layout->layoutName);
    }

    layoutGui->addWidgetDown(new ofxUIRadio("Floors", layoutNames, 0, 50, 50));

}

void testApp::skeletonTestPrint() {
    printf("testing all skeleton data\n");

    const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
    printf("how many locations have skeletons? %i\n", allSkelData.size());
    for (const LocationSkeletonData& locationSkeletons : allSkelData) {
        printf("how many skeletons at location %i? %i\n", locationSkeletons.locationID, locationSkeletons.userJointData.size());
        // printf("this LocationSkeletonData is @0x%x\n", &locationSkeletons);
        for (const SkeletonData& skel : locationSkeletons.userJointData) {
            printf("how many joints on skeleton of user %i? %i\n", skel.userID, skel.jointData.size());
            for (const SkeletonJoint& j : skel.jointData) {
                printf("joint (%f, %f, %f)     confidence: %s\n", j.x, j.y, j.z, j.confidence);
            }
        }
    }
}

void testApp::skeletonTestDraw() {
    ofPushMatrix();
    ofTranslate(300, 300, 0);
    ofTranslate(ofGetMouseX(), ofGetMouseY(), 0);

    const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
    for (const LocationSkeletonData& locationSkeletons : allSkelData) {
        for (const SkeletonData& skel : locationSkeletons.userJointData) {
            for (const SkeletonJoint& j : skel.jointData) {
                ofSetHexColor(0x000000);
                // ofSphere(j.x, j.y, j.z, 10);
                ofSphere(-j.x, -j.y, 0, 10);
            }
        }
    }

    ofPopMatrix();
}


void testApp::update(){
    lastMouseX = ofGetMouseX();
    lastMouseY = ofGetMouseY();

    gelink.update();
    if (geFakeData) gelink.ensureFakeData();
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
    for (LayoutRenderer& renderer : layoutRenderers) {
        renderer.projection.dyn = layoutRenderTransform;
    }

    // update label
    titleLabel->setLabel(titleLabelBase + " - " + layoutRenderers[renderers_active_i].layout->layoutName);
}

void testApp::draw(){
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

    // render skeleton
    {
        const vector< LocationSkeletonData >& allSkelData = gelink.getUserJointData();
        for (const LocationSkeletonData& locationSkeletons : allSkelData) {
            for (const SkeletonData& skel : locationSkeletons.userJointData) {
                skelRenderer.render(skelRenderMode, skel);
            }
        }
    }
}

void testApp::exit() {
    gelink.disconnect();
    delete gui;
}

void testApp::guiEvent(ofxUIEventArgs &e ) {
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

void testApp::guiEventLayouts(ofxUIEventArgs &e ) {
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
    }
}

void testApp::guiEventServer(ofxUIEventArgs &e) {
    // TODO debounce buttons
    if (e.widget->getKind() == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton& btn = *((ofxUILabelButton*)e.widget);
        if (btn.getValue()) {
            printf("%i %i %s\n", btn.getKind(), btn.getValue(), btn.getName().c_str());

            if (btn.getName() == "Register") {
                gelink.connect(ge_server_host, ge_server_port, listening_port);
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

void testApp::keyPressed(int key){
    // UImainView.keyPressedEvent(key);

    switch( key ){
        case 'q':
            exitApp();
            break;
        case 'r':
            setupLayouts();
            break;
        case 'p':
            gui->setDrawWidgetPadding(true);
            break;
        case 'P':
            gui->setDrawWidgetPadding(false);
            break;
        case 'v':
            gui->toggleVisible();
            for (ofxUIWidget* w : gui->getWidgets()) {
                w->toggleVisible();
            }
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
        case 'w':
            skelRenderMode.joints = ! skelRenderMode.joints;
            // skelRenderMode.sticks = skelRenderMode.joints;
            skelRenderMode.chains = skelRenderMode.joints;
            break;
        default:
            printf("Key Pressed: %i\n", key);
            break;
    }
}

void testApp::keyReleased(int key) {}

void testApp::mouseMoved(int x, int y ) {}

void testApp::mouseDragged(int x, int y, int button) {
    float pandx = (ofGetMouseX() - lastMouseX);
    float pandy = (ofGetMouseY() - lastMouseY);
    LayoutProjectionDynamic& dyn = layoutRenderTransform;
    dyn.pan.x += pandx * cos(-dyn.zRotation / 180. * M_PI) - pandy * sin(-dyn.zRotation / 180. * M_PI);
    dyn.pan.y += pandx * sin(-dyn.zRotation / 180. * M_PI) + pandy * cos(-dyn.zRotation / 180. * M_PI);
}

void testApp::mousePressed(int x, int y, int button) {}

void testApp::mouseReleased(int x, int y, int button) {}

void testApp::windowResized(int w, int h) {}

void testApp::gotMessage(ofMessage msg) {}

void testApp::dragEvent(ofDragInfo dragInfo) {}

string testApp::getMainAppDataDirectory(){

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
