// ViewAppBase exists only to avoid circular inclusion
#pragma once

#include "ofMain.h"

class ViewAppBase : public ofBaseApp {
public:
    virtual void disable() = 0;
    virtual void enable() = 0;
};
