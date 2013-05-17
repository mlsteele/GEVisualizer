#pragma once

#include "ofMain.h"
#include "geometry.h"

// GE Location of sensor(s)
class Location {
public:
    int locationID;

    // real world positions
    POINT3D position;
    ROTATION3D rotation;
};
