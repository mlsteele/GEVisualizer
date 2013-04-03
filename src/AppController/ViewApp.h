// TODO: add controller introspection
#pragma once
#include <boost/foreach.hpp>

#include "ofMain.h"
#define OFX_UI_NO_XML
#include "ofxUI.h"

#include "ViewAppBase.h"
#include "ViewControllerApp.h"


class ViewApp : public ViewAppBase {
public:
    void enable()  {
        BOOST_FOREACH (ofPtr<ofxUICanvas> gui, viewGuis) {
            gui->enable();
        }
    }

    void disable() {
        BOOST_FOREACH (ofPtr<ofxUICanvas> gui, viewGuis) {
            gui->disable();
        }
    }

public:
    vector<ofPtr<ofxUICanvas> > viewGuis;
};
