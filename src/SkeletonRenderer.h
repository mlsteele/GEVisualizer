#pragma once

#include "ofMain.h"
#include "geometry.h"
#include "GEVisualizer.h"

// dynamic portion of SkeletonProjection (for common offsets like zoom and pan)
typedef struct {
    float zoomFactor = 1;
    POINT2D pan = {0, 0};
} SkeletonProjectionDynamic;

typedef struct {
    POINT3D offset = {0, 0, 0};
    POINT3D scale = {1, 1, 1};
    ofRectangle view_rect;
    POINT3D real_center = {0,0};
    double screenPixelsPerMeter = 1;
    SkeletonProjectionDynamic dyn;

} SkeletonProjection;

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
} SkeletonRenderMode;

class SkeletonRenderer {
public:
    void attachFonts(ofTrueTypeFont* fontMain) {
        this->fontMain = fontMain;
    }

    void setupProjection(ofRectangle view_rect, POINT3D real_center, double screenPixelsPerMeter);
    void render(SkeletonRenderMode& renderMode, const SkeletonData& skeleton);
    // void print(SkeletonData& skel);

    SkeletonProjection projection;

private:
    ofTrueTypeFont*  fontMain;
};
