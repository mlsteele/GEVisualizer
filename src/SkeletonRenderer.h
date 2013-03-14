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
    bool sticks = true;
    bool confidence = false;
    bool humane = false; // hides improbable and unpleasant mangling of people
} SkeletonRenderMode;

// indices of connected skeleton joints
// [0] Head
// [1] Neck
// [2] Torso
// [3] Waist
// [4] LeftCollar
// [5] LeftShoulder
// [6] LeftElbow
// [7] LeftWrist
// [8] LeftHand
// [9] LeftFingertip
// [10] RightCollar
// [11] RightShoulder
// [12] RightElbow
// [13] RightWrist
// [14] RightHand
// [15] RightFingertip
// [16] LeftHip
// [17] LeftKnee
// [18] LeftAnkle
// [19] LeftFoot
// [20] RightHip
// [21] Right Knee
// [22] RightAnkle
// [23] RightFoot

// center
static const int skeleton_num_connection = 23;
static const int skeleton_connections[skeleton_num_connection][2] = {
{ 0      , 1 },
{ 1      , 2 },
{ 2      , 3 },
// symmetrical top left },
{ 1      , (4) },
{ (4)    , (5) },
{ (5)    , (6) },
{ (6)    , (7) },
{ (7)    , (8) },
{ (8)    , (9) },
// symmetrical top right },
{ 1      , (4+6) },
{ (4+6)  , (5+6) },
{ (5+6)  , (6+6) },
{ (6+6)  , (7+6) },
{ (7+6)  , (8+6) },
{ (8+6)  , (9+6) },
// symmetrical bottom left },
{ 3      , (16) },
{ (16)   , (17) },
{ (17)   , (18) },
{ (18)   , (19) },
// symmetrical bottom right },
{ 3      , (16+4) },
{ (16+4) , (17+4) },
{ (17+4) , (18+4) },
{ (18+4) , (19+4) }};


class SkeletonRenderer {
public:
    void attachFonts(ofTrueTypeFont* fontMain, ofTrueTypeFont* fontMapNameLabel) {
        this->fontMain = fontMain;
        this->fontMapNameLabel = fontMapNameLabel;
    }

    void setupProjection(POINT2D screen_px_corner, POINT3D real_corner, double screenPixelsPerMeter);
    void render(SkeletonRenderMode& renderMode, const SkeletonData& skeleton);
    void print(SkeletonData& skel);

    SkeletonProjection projection;

private:
    // void recalculateTexture(GEVisualizer& store);
    void mouseTestRecalculateTexture();

    ofTrueTypeFont*  fontMain;
    ofTrueTypeFont*  fontMapNameLabel;
};
