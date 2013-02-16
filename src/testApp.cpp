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
    POINT2D screen_px_corner = {135, 100};
    POINT2D real_corner = {0, 0};
    double screenPixelsPerMeter = 10;

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
    renderers_active_i = 1;
    renderers_transition_i = renderers_active_i;
}

void testApp::setupUI() {
    // gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());

    // gui->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM));
    // gui->addWidgetDown(new ofxUIButton(200, 200, false, "B1"));

    // ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);

    ////////

    ofColor backgroundColor(255, 255, 255);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    UImainView.init((void*) this, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()), backgroundColor, foregroundColor);
    UImainView.showBorder = false;
    UImainView.borderWidth = 1;
    UImainView.borderColor = borderColor;
    UImainView.showBackground = false;

    setupUIServer();
    setupUILayouts();
}

void testApp::setupUIServer() {
    ofColor backgroundColor(100, 100, 100);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    UIserverSubView.init("UIserverSubView",
        ofRectangle( UImainView.getBounds().x, UImainView.getBounds().y + 768 - 120 ,
                     UImainView.getBounds().width, UImainView.getBounds().height ),
        backgroundColor, foregroundColor);
    UIserverSubView.showBorder = true;
    UIserverSubView.borderWidth = 1;
    UIserverSubView.borderColor = borderColor;
    UIserverSubView.drawBackground = true;

    ofUIButton buttonFactory;
    buttonFactory.setFont(fontVerd10);
    buttonFactory.setCallback(::buttonCallback);

    float xo = 12; // origin / cursor
    float xb = 130; // size
    float xs = 10; // spacing
    float xi = xb + xs; // increment
    float yo = 12;
    float yb = 40;
    float ys = 10;
    float yi = yb + ys;

    ofRectangle cookieCutter(xo, yo, xb, yb);
    // buttonID, ofRectangle bounds, text, backgroundColor, foregroundColor, selectedColor);

    buttonFactory.init("register",
        cookieCutter, "Register",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);

    // yo += yi;
    cookieCutter.translate(0, yi);
    buttonFactory.init("unregister",
        cookieCutter, "Unregister",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);


    cookieCutter.translate(xi, -yi);
    buttonFactory.init("stream",
        cookieCutter, "Stream",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("unstream",
        cookieCutter, "Unstream",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);


    cookieCutter.translate(xi, -yi);
    buttonFactory.init("rendermode_locations_on",
        cookieCutter, "Show Locations",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("rendermode_locations_off",
        cookieCutter, "Hide Locations",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);


    cookieCutter.translate(xi, -yi);
    buttonFactory.init("rendermode_texture_on",
        cookieCutter, "Show Texture",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("rendermode_texture_off",
        cookieCutter, "Hide Textures",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);


    cookieCutter.translate(xi, -yi);
    buttonFactory.init("rendermode_presence_on",
        cookieCutter, "Show Presence",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("rendermode_presence_off",
        cookieCutter, "Hide Presence",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);


    cookieCutter.translate(xi, -yi);
    buttonFactory.init("rendermode_userlocation_on",
        cookieCutter, "Show User Locations",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("rendermode_userlocation_off",
        cookieCutter, "Hide User Locations",
        ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
    UIserverSubView.addButton(buttonFactory);


    UImainView.addSubView(UIserverSubView);
}

void testApp::setupUILayouts()  {
    ofColor backgroundColor(100, 100, 100);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    UIlayoutSubView.init("UIlayoutSubView",
        ofRectangle( UImainView.getBounds().x + 850, UImainView.getBounds().y ,
                     UImainView.getBounds().width, UImainView.getBounds().height ),
        backgroundColor, foregroundColor);
    UIlayoutSubView.showBorder = true;
    UIlayoutSubView.borderWidth = 1;
    UIlayoutSubView.borderColor = borderColor;
    UIlayoutSubView.drawBackground = true;

    ofUIButton buttonFactory;
    buttonFactory.setFont(fontVerd10);
    buttonFactory.setCallback(::buttonCallback);

    float xo = 12; // origin / cursor
    float xb = 130; // size
    float xs = 10; // spacing
    float xi = xb + xs; // increment
    float yo = 12;
    float yb = 40;
    float ys = 10;
    float yi = yb + ys;

    ofRectangle cookieCutter(xo, yo, xb, yb);

    for (LayoutRenderer& layoutRenderer : layoutRenderers) {
        Layout& layout = *(layoutRenderer.layout);
        buttonFactory.init("layout_" + layout.layoutName,
            cookieCutter, layout.layoutName,
            ofColor(210, 210, 210), ofColor(0, 0, 0), ofColor(50, 50, 50));
        UIlayoutSubView.addButton(buttonFactory);
        cookieCutter.translate(0, yi);
    }

    UImainView.addSubView(UIlayoutSubView);
}

void testApp::update(){
    UImainView.update();
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

    UImainView.draw(0, 0);
}

void testApp::exit() {
    gelink.disconnet();
    // delete gui;
}

void buttonCallback(ofUIButton* button_pointer, void* appPointer) {
    testApp& app = *(testApp*)appPointer;
    app.buttonCallback(*button_pointer);
}

void testApp::buttonCallback(ofUIButton& button) {
    syncButtonVisibility(button);

    // Only respond to a visible button press
    if( !button.isButtonDownState() || !button.isVisible() ) return;
    // printf("isButtonDownState() -> true\n");

    printf("button pressed (%s)\n", button.getButtonID().c_str());

    if (button.getButtonID() == "register") {
        gelink.connect(ge_server_host, ge_server_port, listening_port);
    }

    if (button.getButtonID() == "unregister") {
        gelink.disconnet();
    }

    if (button.getButtonID() == "stream") {
        gelink.sendLocationInfo();
        gelink.streamUserPresenceData(true);
        gelink.streamUserLocationData(true);
    }

    if (button.getButtonID() == "unstream") {
        gelink.streamUserPresenceData(false);
        gelink.streamUserLocationData(false);
    }

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
}

void testApp::keyPressed(int key){
    UImainView.keyPressedEvent(key);

    switch( key ){
        case 'q':
            break;
        default:
            printf("Key Pressed: %i\n", key);
            break;
    }
}

void testApp::keyReleased(int key) { UImainView.keyPressedEvent(key); }

void testApp::mouseMoved(int x, int y ) { UImainView.mouseMovedEvent(x,y); }

void testApp::mouseDragged(int x, int y, int button) { UImainView.mouseDraggedEvent(x,y); }

void testApp::mousePressed(int x, int y, int button) { UImainView.mousePressedEvent(x,y); }

void testApp::mouseReleased(int x, int y, int button) { UImainView.mouseReleasedEvent(x,y); }

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
