#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define OFX_UI_NO_XML
#include "ofxUI.h"
#include "ViewControllerApp.h"

class VCDummyApp : public DummyApp {
public:
    VCDummyApp(ofPtr<ViewController> vc) : vc(vc) {}

    void keyPressed(int button) {
        printf("handling\n");
        switch( key ){
            case 'n':
                vc.selectApp('layouts');
                break;
            default:
                printf("Key Pressed: %i\n", key);
                break;
        }
    }

protected:
    ofPtr<ViewController> vc;
};
