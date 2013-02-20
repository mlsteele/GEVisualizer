#pragma once

#include "ofMain.h"
#include "geometry.h"

class Location {
public:
    int locationID;

    // real world positions
    POINT3D position;
    ROTATION3D rotation;
};

class LocationStream {
public:
    LocationStream(Location* location) : location(location) {}
    // LocationStream(const LocationStream &rhs) : location(rhs.location) {}
    // LocationStream &operator= (const LocationStream &rhs) {
    //     if 
    //     location = rhs.location; // TODO: this should be const
    //     return *this;
    // }

    Location* location = NULL;
    int presenceInfo = PRESENCE_EMPTY;
    int count = 0;

    // This will be a 1 if a user is present at that location, 0 otherwise.
    enum presenceInfoVals {PRESENCE_EMPTY=0, PRESENCE_PRESENT=1};
};
