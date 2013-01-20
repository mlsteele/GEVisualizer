#include "testApp.h"

void testApp::setup(){

    ofSetFrameRate( 30 );

    mainAppDataDirectory = getMainAppDataDirectory();

    layout1.loadLayoutFiles(mainAppDataDirectory, "E14_5_LayoutInfo.txt");
    layout1.setupLocationStreams();
    layout1.setupProjection();

    ofEnableSmoothing();

    // layoutManager.setMainAppDataDirectory( mainAppDataDirectory );

    // layoutManager.init();
}

void testApp::update(){

    //Update the sensor manager, this gets any new sensor data from the network
    // locationManager.update();

    //Update the layout manager, this updates the view
    // layoutManager.update();

    //Give the layout manager the most recent sensor data so it can draw it
    // layoutManager.updateLocationData( locationManager.getLocationData() );

    layout1.locationStreams[0].presenceInfo = LocationStream::PRESENCE_PRESENT;
}

void testApp::draw(){
    ofBackground(0xFFFFFF);
    layout1.render();
}

void testApp::keyPressed(int key){
    switch( key ){
        case 'q':
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
