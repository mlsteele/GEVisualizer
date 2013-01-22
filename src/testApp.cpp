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

    UImainView.init((void*) this, ofRectangle(0, 0, 300, 300), backgroundColor, foregroundColor);
    UImainView.showBorder = true;
    UImainView.borderWidth = 1;
    UImainView.borderColor = borderColor;

    ofUISubView UImainSubView;
    UImainSubView.init("UImainSubView",
        ofRectangle( UImainView.getBounds().x, UImainView.getBounds().y          ,
                     UImainView.getBounds().width, UImainView.getBounds().height ),
        backgroundColor, foregroundColor);
    UImainSubView.showBorder = true;
    UImainSubView.borderWidth = 1;
    UImainSubView.borderColor = borderColor;

    ofUIButton buttonFactory;
    buttonFactory.setFont(mainAppFont);
    buttonFactory.setCallback(buttonCallback);

    buttonFactory.init("button1",
        ofRectangle(12, 12, 130, 40), "Button 1",
        ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(100, 100, 100));
    UImainSubView.addButton(buttonFactory);

    UImainView.addSubView(UImainSubView);
}

void testApp::setupOSC() {
    // OSC IO
    clientSender.setup("127.0.0.1", ge_server_port);
    clientReceiver.setup(listening_port);

    // register with GE Server
    {
    printf("registering with GE server... ");
    ofxOscMessage m;
    m.setAddress("/Register");
    m.addIntArg(listening_port);
    clientSender.sendMessage(m);
    printf("done\n");
    }

    // subscribe to presence data
    {
    printf("subscring to user presence data... ");
    ofxOscMessage m;
    m.setAddress("/StreamUserPresenceData");
    m.addIntArg(1); // subscription state
    m.addIntArg(1); // allLocations
    clientSender.sendMessage(m);
    printf("done\n");
    }
}

void testApp::update(){
    UImainView.update();
    processOSCQueue();
}

void testApp::processOSCQueue() {
    // process OSC queue
    ofxOscMessage m;
    while (clientReceiver.hasWaitingMessages()) {
        clientReceiver.getNextMessage(&m);
        printf("received OSC message\n");
        printf("from: %s:%i\n", m.getRemoteIp().c_str(), m.getRemotePort());
        printf("address: %s\n", m.getAddress().c_str());
        printf("args[%u]\n", m.getNumArgs());

        processOSCMsg(m);
    }

    // layout1.locationStreams[0].presenceInfo = LocationStream::PRESENCE_PRESENT;
}

void testApp::processOSCMsg(ofxOscMessage& m) {
    if (m.getAddress() == "/UserPresenceData") {
        printf("dump /UserPresenceData -> ");
        for(int i = 0; i < m.getNumArgs(); i++) {
            printf(" %i", m.getArgAsInt32(i));
        }
        printf("\n");


        int numDatas = m.getArgAsInt32(0);
        if (m.getNumArgs() == numDatas * 2 + 1) {
            for (int i = 0; i < numDatas; i++) {
                int location_id = m.getArgAsInt32(i + 1);
                int new_presence_info = m.getArgAsInt32(i + numDatas + 1);
                attemptToSetPresenceInfo(location_id, new_presence_info);
            }
        } else {
            printf("m.getNumArgs() -> %i\n", numDatas);
            printf("WARN: malformed OSC message (%s) [nArgs:%i].\n", m.getAddress().c_str(), m.getNumArgs());
        }
    } else {
        printf("WARN: ignoring OSC message (%s) [nArgs:%i].\n", m.getAddress().c_str(), m.getNumArgs());
    }
}

void testApp::attemptToSetPresenceInfo(int location_id, int new_presence_info) {
    for (LocationStream& locationStream : layout1.locationStreams) {
        if (locationStream.location.ge_id == location_id) {
            locationStream.presenceInfo = new_presence_info;
            printf("set presence info for [%i] to (%i)\n", location_id, new_presence_info);
            return;
        }
    }
    printf("failed to find matching location in layout [%i]\n", location_id);
}

void testApp::draw(){
    ofBackground(0xFFFFFF);
    UImainView.draw(0, 0);
    // layout1.render();
}

void testApp::exit() {
    // unregister
    printf("unregistering from GE server... ");
    ofxOscMessage m;
    m.setAddress("/Unregister");
    m.addIntArg(listening_port);
    clientSender.sendMessage(m);
    printf("done\n");
}

void buttonCallback(ofUIButton* button, void* appPointer){
    testApp& mainAppHandle = *(testApp*)appPointer;

    printf("buttonCallbackFunction()\n");
    printf("getButtonID() -> %s\n", button->getButtonID().c_str());
    
    //Only respond to a button down press
    if( !button->isButtonDownState() ) return;
    printf("isButtonDownState() -> true\n");
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
