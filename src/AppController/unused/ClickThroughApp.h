#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define OFX_UI_NO_XML
#include "ofxUI.h"
#include <boost/regex.hpp>

class ClickThroughApp : public ofBaseApp {
public:
    ClickThroughApp(ofPtr<ofBaseApp> nextApp) : nextApp(nextApp) {}

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

public:

    ofPtr<ofBaseApp> nextApp;

    ofTrueTypeFont fontVerd10;
    ofTrueTypeFont fontVerd14;
};
