#include "DummyApp.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>

void DummyApp::setup(){
    ofSetWindowTitle("Dummy");
    ofSeedRandom();
    ofEnableSmoothing();
    ofSetFrameRate(30);

    fontVerd10.loadFont("verdana.ttf", 10);
    fontVerd14.loadFont("verdana.ttf", 14);
}

void DummyApp::update(){
}

void DummyApp::draw(){
    ofFill();
    ofSetHexColor(0xFF0000);
    ofCircle(100, 100, 50);
}

void DummyApp::exit() {
}

void DummyApp::keyPressed(int key){
    printf("dummy handling\n");

    switch( key ){
        case 'q':
            exitApp();
            break;
        default:
            printf("Key Pressed: %i\n", key);
            break;
    }
}

void DummyApp::keyReleased(int key) {}

void DummyApp::mouseMoved(int x, int y ) {}

void DummyApp::mouseDragged(int x, int y, int button) {}

void DummyApp::mousePressed(int x, int y, int button) {}

void DummyApp::mouseReleased(int x, int y, int button) {}

void DummyApp::windowResized(int w, int h) {}

void DummyApp::gotMessage(ofMessage msg) {}

void DummyApp::dragEvent(ofDragInfo dragInfo) {}
