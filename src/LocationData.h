#pragma once

#include "ofMain.h"

class Location {
public:
    int ge_id;

    // real world positions
    double positionX;
    double positionY;
    double positionZ;
};

class LocationStream {
public:
    LocationStream(Location& location) : location(location) {}

    Location& location;
    int presenceInfo = PRESENCE_EMPTY;

    // This will be a 1 if a user is present at that location, 0 otherwise.
    enum presenceInfoVals {PRESENCE_EMPTY=0, PRESENCE_PRESENT=1};
};
