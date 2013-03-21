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
    POINT2D screen_px_corner = {0,0};
    POINT3D real_corner = {0,0};
    double screenPixelsPerMeter = 1;
    SkeletonProjectionDynamic dyn;

} SkeletonProjection;

typedef struct {
    bool joints = true;
    bool sticks = false;
    bool chains = true; // stick drawing that connects over missing nodes
    bool confidence = false;
    bool node_indices = false;
    bool node_locations = true;
    bool humane = false; // hides improbable and unpleasant mangling of people (NOT IMPLEMENTED)
} SkeletonRenderMode;

class SkeletonRenderer {
public:
    void attachFonts(ofTrueTypeFont* fontMain) {
        this->fontMain = fontMain;
    }

    void setupProjection(POINT2D screen_px_corner, POINT3D real_corner, double screenPixelsPerMeter);
    void render(SkeletonRenderMode& renderMode, const SkeletonData& skeleton);
    // void print(SkeletonData& skel);

    SkeletonProjection projection;

private:
    ofTrueTypeFont*  fontMain;
};
