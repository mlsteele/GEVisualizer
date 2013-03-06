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

void testApp::setupUI() {
    gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->addWidgetDown(new ofxUILabel("Media Lab", OFX_UI_FONT_LARGE)); 

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
    layoutGui->addWidgetDown(new ofxUILabelButton("dry", (bool) false, 200, 40, 0, 0, OFX_UI_FONT_MEDIUM));

    vector<string> layoutNames;
    for (LayoutRenderer& layoutRenderer : layoutRenderers) {
        layoutNames.push_back(layoutRenderer.layout->layoutName);
    }

    layoutGui->addWidgetDown(new ofxUIRadio("Floors", layoutNames, 0, 50, 50));

}

void testApp::update(){
    gelink.update();

    // transition to approach selection
    static const float transition_speed_1 = 0.06;
    const float transition_diff_mag = fabs(renderers_transition_i - renderers_active_i);
    if (transition_diff_mag < transition_speed_1) {
        renderers_transition_i = renderers_active_i;
    } else {
        const float transition_speed = fmax(transition_diff_mag / 10, transition_speed_1);
        renderers_transition_i += (renderers_active_i - renderers_transition_i) > 0 ? transition_speed : -transition_speed;
    }
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

    // UImainView.draw(0, 0);
}

void testApp::exit() {
    gelink.disconnet();
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
        for (LayoutRenderer& renderer : layoutRenderers) {
            renderer.projection.zRotation = val;
        }
    }

    if (e.widget->getName() == "Zoom") {
        float val = (*(ofxUISlider*) e.widget).getScaledValue();
        for (LayoutRenderer& renderer : layoutRenderers) {
            renderer.projection.zoomFactor = val;
        }
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
                gelink.disconnet();
            }

            if (btn.getName() == "Stream") {
                gelink.sendLocationInfo();
                gelink.streamUserPresenceData(true);
                gelink.streamUserLocationData(true);
            }

            if (btn.getName() == "Unstream") {
                gelink.streamUserPresenceData(false);
                gelink.streamUserLocationData(false);
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

/*
void testApp::buttonCallback(ofUIButton& button) {
    // syncButtonVisibility(button);

    // Only respond to a visible button press
    if( !button.isButtonDownState() || !button.isVisible() ) return;
    // printf("isButtonDownState() -> true\n");

    printf("button pressed (%s)\n", button.getButtonID().c_str());


    string layout_prefix = "layout_";
    if (boost::starts_with(button.getButtonID(), layout_prefix)) {
        printf("layout button pressed %s\n", button.getButtonID().c_str());
        for (int i = 0; i < layoutRenderers.size(); i++) {
            Layout& layout = *layoutRenderers[i].layout;
            if (boost::ends_with(button.getButtonID(), layout.layoutName)) {
                renderers_active_i = i;
                return;
            }
        }
        printf("WARN: failed to find matching layout for button\n");
    }

    string rendermode_prefix = "rendermode_";
    if (boost::starts_with(button.getButtonID(), rendermode_prefix)) {
        string id = button.getButtonID();
        printf("rendermode button pressed %s\n", button.getButtonID().c_str());
        bool state = boost::ends_with(button.getButtonID(), "_on");
        string mode = string(id.begin() + rendermode_prefix.length(), id.end() - (state ? 3 : 4));
        printf("button mode %s <- %i\n", mode.c_str(), state);

        if (mode == "locations") {
            mainRenderMode.locations = state;
        }
        else if (mode == "texture") {
            mainRenderMode.texture = state;
        }
        else if (mode == "presence") {
            mainRenderMode.presence = state;
        }
        else if (mode == "userlocation") {
            mainRenderMode.userLocation = state;
        }

        // printf("rmbutton visibility (%s) -> %i\n", button.getButtonID().c_str(), button.isVisible());
        // button.setVisible(!button.isVisible());
        // printf("rmbutton visibility (%s) -> %i\n", button.getButtonID().c_str(), button.isVisible());
    }
}

void testApp::syncButtonVisibility(ofUIButton& button) {
    printf("%i\n", button.getButtonID() == "register");
    if (!gelink.getIsConnected() && !(button.getButtonID() == "register" || button.getButtonID() == "unregister")){
        printf("visibility false %s\n", button.getButtonID().c_str());
        button.setVisible(false);
    } else {
        printf("visibility true %s\n", button.getButtonID().c_str());
        button.setVisible(true);
    }
}*/

void testApp::keyPressed(int key){
    // UImainView.keyPressedEvent(key);

    switch( key ){
        case 'q':
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
        default:
            printf("Key Pressed: %i\n", key);
            break;
    }
}

void testApp::keyReleased(int key) {}

void testApp::mouseMoved(int x, int y ) {}

void testApp::mouseDragged(int x, int y, int button) {}

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
