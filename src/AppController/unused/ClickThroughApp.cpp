#include "ClickThroughApp.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>

void ClickThroughApp::setup(){
    ofSetWindowTitle("Dummy");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    fontVerd10.loadFont("verdana.ttf", 10);
    fontVerd14.loadFont("verdana.ttf", 14);
}

void ClickThroughApp::update(){
    printf("update\n");
}

void ClickThroughApp::draw(){
    ofFill();
    ofSetHexColor(0xFF0000);
    ofCircle(100, 100, 50);
}

void ClickThroughApp::exit() {
}

void ClickThroughApp::keyPressed(int key){
    // UImainView.keyPressedEvent(key);

    switch( key ){
        case 'q':
            exitApp();
            break;
        default:
            printf("Key Pressed: %i\n", key);
            break;
    }
}

void ClickThroughApp::keyReleased(int key) {}

void ClickThroughApp::mouseMoved(int x, int y ) {}

void ClickThroughApp::mouseDragged(int x, int y, int button) {}

void ClickThroughApp::mousePressed(int x, int y, int button) {}

void ClickThroughApp::mouseReleased(int x, int y, int button) {
    printf("released\n");
    ofSetAppPtr(nextApp);
}

void ClickThroughApp::windowResized(int w, int h) {}

void ClickThroughApp::gotMessage(ofMessage msg) {}

void ClickThroughApp::dragEvent(ofDragInfo dragInfo) {}
