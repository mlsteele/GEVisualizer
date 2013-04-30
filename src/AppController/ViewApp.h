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
        BOOST_FOREACH(viewGui_t viewGuiPair, viewGuis) {
            viewGuiPair.second->enable();
        }
    }

    void disable() {
        BOOST_FOREACH(viewGui_t viewGuiPair, viewGuis) {
            viewGuiPair.second->disable();
        }
    }

public:
    typedef pair<string, ofPtr<ofxUICanvas> > viewGui_t;
    map<string, ofPtr<ofxUICanvas> > viewGuis;
};
