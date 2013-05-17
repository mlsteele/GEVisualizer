/*
    ViewApp

    Class derived from ofBaseApp represents an OF app which can be part of a program using ViewControllerApp.
    Contains ofxUI containers and switches.
*/

// TODO: add a way for apps to access controller

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
