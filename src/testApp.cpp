#include "testApp.h"
#include <boost/algorithm/string/predicate.hpp>


void testApp::setup(){
    ofSetWindowTitle("Map");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    mainAppDataDirectory = getMainAppDataDirectory();
    mainAppFont.loadFont("verdana.ttf", 10);

    setupLayouts();
    setupUI();
}

void testApp::setupLayouts() {
    POINT2D screen_px_corner = {15, 15};
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
        layoutRenderers.back().setupProjection(screen_px_corner, real_corner, screenPixelsPerMeter);
    }
    active_layout_renderer = &layoutRenderers.back();
}

void testApp::setupUI() {
    ofColor backgroundColor(255, 255, 255);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    UImainView.init((void*) this, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()), backgroundColor, foregroundColor);
    UImainView.showBorder = false;
    UImainView.borderWidth = 1;
    UImainView.borderColor = borderColor;

    setupUIServer();
    setupUILayouts();
}

void testApp::setupUIServer() {
    ofColor backgroundColor(255, 255, 255);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    ofUISubView UIserverSubView;
    UIserverSubView.init("UIserverSubView",
        ofRectangle( UImainView.getBounds().x, UImainView.getBounds().y + 768 - 120 ,
                     UImainView.getBounds().width, UImainView.getBounds().height ),
        backgroundColor, foregroundColor);
    UIserverSubView.showBorder = true;
    UIserverSubView.borderWidth = 1;
    UIserverSubView.borderColor = borderColor;

    ofUIButton buttonFactory;
    buttonFactory.setFont(mainAppFont);
    buttonFactory.setCallback(buttonCallback);

    float xo = 12; // origin / cursor
    float xb = 130; // size
    float xs = 10; // spacing
    float xi = xb + xs; // increment
    float yo = 12;
    float yb = 40;
    float ys = 10;
    float yi = yb + ys;

    ofRectangle cookieCutter(xo, yo, xb, yb);

    buttonFactory.init("register",
        cookieCutter, "Register",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UIserverSubView.addButton(buttonFactory);

    // yo += yi;
    cookieCutter.translate(0, yi);
    buttonFactory.init("unregister",
        cookieCutter, "Unregister",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(xi, -yi);
    buttonFactory.init("stream",
        cookieCutter, "Stream",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UIserverSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("unstream",
        cookieCutter, "Unstream",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UIserverSubView.addButton(buttonFactory);

    UImainView.addSubView(UIserverSubView);
}

void testApp::setupUILayouts()  {
    ofColor backgroundColor(255, 255, 255);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    ofUISubView UIlayoutSubView;
    UIlayoutSubView.init("UIlayoutSubView",
        ofRectangle( UImainView.getBounds().x + 850, UImainView.getBounds().y ,
                     UImainView.getBounds().width, UImainView.getBounds().height ),
        backgroundColor, foregroundColor);
    UIlayoutSubView.showBorder = true;
    UIlayoutSubView.borderWidth = 1;
    UIlayoutSubView.borderColor = borderColor;

    ofUIButton buttonFactory;
    buttonFactory.setFont(mainAppFont);
    buttonFactory.setCallback(buttonCallback);

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
            ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
        UIlayoutSubView.addButton(buttonFactory);
        cookieCutter.translate(0, yi);
    }

    UImainView.addSubView(UIlayoutSubView);
}

void testApp::update(){
    UImainView.update();
    gelink.update();
}

void testApp::draw(){
    ofBackground(0xFFFFFF);
    UImainView.draw(0, 0);

    active_layout_renderer->render(gelink);
}

void testApp::exit() {
    gelink.disconnect();
}

void buttonCallback(ofUIButton* button, void* appPointer){
    testApp& mainAppHandle = *(testApp*)appPointer;

    // printf("buttonCallbackFunction()\n");

    // Only respond to a button down press
    if( !button->isButtonDownState() ) return;
    // printf("isButtonDownState() -> true\n");

    printf("button pressed (%s)\n", button->getButtonID().c_str());

    if (button->getButtonID() == "register") {
        mainAppHandle.gelink.connect(mainAppHandle.ge_server_host, mainAppHandle.ge_server_port, mainAppHandle.listening_port);
    }

    if (button->getButtonID() == "unregister") {
        mainAppHandle.gelink.disconnect();
    }

    if (button->getButtonID() == "stream") {
        mainAppHandle.gelink.sendLocationInfo();
        mainAppHandle.gelink.streamUserPresenceData(true);
    }

    if (button->getButtonID() == "unstream") {
        mainAppHandle.gelink.streamUserPresenceData(false);
    }

    string layout_prefix = "layout_";
    if (boost::starts_with(button->getButtonID(), "layout_")) {
        printf("layout button pressed %s\n", button->getButtonID().c_str());
        for (LayoutRenderer& layoutRenderer : mainAppHandle.layoutRenderers) {
            Layout& layout = *layoutRenderer.layout;
            if (boost::ends_with(button->getButtonID(), layout.layoutName)) {
                mainAppHandle.active_layout_renderer = &layoutRenderer;
                return;
            }
        }
        printf("WARN: failed to find matching layout for button\n");
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
