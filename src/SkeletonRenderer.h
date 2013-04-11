#pragma once

#include "ofMain.h"
#include "geometry.h"
#include "GEVisualizer.h"

namespace SkeletonRenderer {

typedef struct {
    bool backdrop = true; // for 2D only, enforces square rendering and draw background if clear=true on render2D
    bool joints = true;
    bool head = false; // untested
    bool sticks = false;
    // stick drawing that connects over missing nodes
    bool chains = true;
    bool confidence = false;
    bool node_label_indices = false;
    bool node_label_locations = false;
    int  node_label_location_index = 8; // -1 for off
    // hides improbable and unpleasant mangling of people (NOT IMPLEMENTED)
    bool humane = false;
} RenderMode;

typedef struct {
    ofRectangle view_rect;
    POINT3D real_center;
    double screenPixelsPerMeter;
} Projection2D;

typedef struct {
    double screenPixelsPerMeter;
    POINT3D locationRoot;
    double theta = 0;
} Projection3D;

void render2D(const SkeletonData&, const RenderMode&, const Projection2D&,
              ofTrueTypeFont&, bool clear=false);

void render3D(const SkeletonData&, const RenderMode&, const Projection3D&,
              ofTrueTypeFont&);

} // namespace
