#pragma once

#include "ofMain.h"
#include "geometry.h"
#include "GEVisualizer.h"
#include "LayoutRenderer.h"

typedef struct {
    bool backdrop = true;
    bool joints = true;
    bool sticks = false;
    bool chains = true; // stick drawing that connects over missing nodes
    bool confidence = false;
    bool node_label_indices = false;
    bool node_label_locations = false;
    int node_label_location_index = 8; // -1 for off
    bool humane = false; // hides improbable and unpleasant mangling of people (NOT IMPLEMENTED)
} SkeletonRenderMode3D;

class SkeletonRenderer3D {
public:
    void render3D(SkeletonRenderMode3D& renderMode, const SkeletonData& skel, const POINT3D& location_root, const LayoutRenderer* layoutRenderer);

private:
};
