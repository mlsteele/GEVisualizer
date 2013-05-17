/*
    ViewAppBase

    ViewAppBase exists only to avoid circular dependencies around ViewApp and ViewControllerApp.
    However, ViewApp cannot yet refer to ViewControllerApp so for now this does nothing.
*/

#pragma once

#include "ofMain.h"

class ViewAppBase : public ofBaseApp {
public:
    virtual void disable() = 0;
    virtual void enable() = 0;
};
