#include "testApp.h"

void testApp::setup(){
    ofSetWindowTitle("Map");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    mainAppDataDirectory = getMainAppDataDirectory();
    mainAppFont.loadFont("verdana.ttf", 10);

    setupLayouts();
    setupUI();
    setupOSC();
}

void testApp::setupLayouts() {
    layout1.loadLayoutFiles(mainAppDataDirectory, "E14_5_LayoutInfo.txt");
    layout1.setupLocationStreams();
    layout1.setupProjection();
}

void testApp::setupUI() {
    ofColor backgroundColor(255, 255, 255);
    ofColor foregroundColor(0, 0, 0);
    ofColor borderColor(0, 0, 0);

    UImainView.init((void*) this, ofRectangle(0, 0, 300, 120), backgroundColor, foregroundColor);
    UImainView.showBorder = false;
    UImainView.borderWidth = 1;
    UImainView.borderColor = borderColor;

    ofUISubView UImainSubView;
    UImainSubView.init("UImainSubView",
        ofRectangle( UImainView.getBounds().x, UImainView.getBounds().y + 768 - 120 ,
                     UImainView.getBounds().width, UImainView.getBounds().height ),
        backgroundColor, foregroundColor);
    UImainSubView.showBorder = true;
    UImainSubView.borderWidth = 1;
    UImainSubView.borderColor = borderColor;

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
    UImainSubView.addButton(buttonFactory);

    // yo += yi;
    cookieCutter.translate(0, yi);
    buttonFactory.init("unregister",
        cookieCutter, "Unregister",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UImainSubView.addButton(buttonFactory);

    cookieCutter.translate(xi, -yi);
    buttonFactory.init("stream",
        cookieCutter, "Stream",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UImainSubView.addButton(buttonFactory);

    cookieCutter.translate(0, yi);
    buttonFactory.init("unstream",
        cookieCutter, "Unstream",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UImainSubView.addButton(buttonFactory);

    UImainView.addSubView(UImainSubView);
}

void testApp::setupOSC() {
    gelink.setup();
}

void testApp::update(){
    UImainView.update();
    gelink.processQueue();
}

void testApp::attemptToSetPresenceInfo(int location_id, presenceInfoStreamData new_presence_info) {
    // for (LocationStream& locationStream : layout1.locationStreams) {
    // printf("hunting for location_id %i\n", location_id);
    int lslen = layout1.locationStreams.size();
    for(int i = 0; i < lslen; i++ ) {
        if (layout1.locationStreams[i].location->ge_id == location_id) {
            layout1.locationStreams[i].presenceInfo = new_presence_info.presenceEstimate;
            printf("set presence info for [%i] to (%i)\n", location_id, new_presence_info.presenceEstimate);
            return;
        }
   }

   // printf("failed to find matching location in layout [%i]\n", location_id);
}

void testApp::draw(){
    ofBackground(0xFFFFFF);
    UImainView.draw(0, 0);
    layout1.render();
}

void testApp::exit() {
    gelink.disconnect();
}

void streamingPresenceInfoCallback(void* appPointer, presenceInfoStreamData data) {
    testApp& mainAppHandle = *(testApp*)appPointer;
    // printf("handling presence info [%i, %i, %f]\n",
    //     data.locationID         ,
    //     data.presenceEstimate   ,
    //     data.presenceLikelihood );
    mainAppHandle.attemptToSetPresenceInfo(data.locationID, data);
}

void buttonCallback(ofUIButton* button, void* appPointer){
    testApp& mainAppHandle = *(testApp*)appPointer;

    printf("buttonCallbackFunction()\n");
    printf("getButtonID() -> %s\n", button->getButtonID().c_str());

    //Only respond to a button down press
    if( !button->isButtonDownState() ) return;
    printf("isButtonDownState() -> true\n");

    if (button->getButtonID() == "register") {
        mainAppHandle.gelink.connect();
    }

    if (button->getButtonID() == "unregister") {
        mainAppHandle.gelink.disconnect();
    }

    if (button->getButtonID() == "stream") {
        mainAppHandle.gelink.subscribeToStreamingPresenceInfo(appPointer, streamingPresenceInfoCallback);
    }

    if (button->getButtonID() == "unstream") {
        mainAppHandle.gelink.subscribeToStreamingPresenceInfo(appPointer, NULL);
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
