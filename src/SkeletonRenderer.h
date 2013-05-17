/*
    SkeletonRenderer

    Namespace which contains methods for rendering user skeleton data to the screen.
*/

#pragma once

#include "ofMain.h"
#include "geometry.h"
#include "GEVisualizer.h"

namespace SkeletonRenderer {

typedef struct {
    // backdrop is for 2D only, enforces square clip box when rendering and draws background if clear=true on render2D
    bool backdrop                  = true;
    bool joints                    = true;
    bool head                      = false; // untested
    // sticks are sticks for limbs
    bool sticks                    = false;
    // chains enables stick drawing that connects over missing nodes
    // chains and sticks should not both be enabled.
    bool chains                    = true;
    // confidence renders with regard to joint localization confidence (NOT IMPLEMENTED)
    bool confidence                = false;
    bool node_label_indices        = false;
    bool node_label_locations      = false;
    int  node_label_location_index = 8; // -1 for off
    // humane hides improbable and unpleasant mangling of people such as backwards bent knees (NOT IMPLEMENTED)
    bool humane                    = false;
} RenderMode;

typedef struct {
    ofRectangle view_rect;
    POINT3D     real_center;
    double      screenPixelsPerMeter;
} Projection2D;

typedef struct {
    double  screenPixelsPerMeter;
    POINT3D locationRoot;
    double  theta = 0;
} Projection3D;

void render2D(const SkeletonData&, const RenderMode&, const Projection2D&,
              ofTrueTypeFont&, bool clear=false);

void render3D(const SkeletonData&, const RenderMode&, const Projection3D&,
              ofTrueTypeFont&);

} // namespace
