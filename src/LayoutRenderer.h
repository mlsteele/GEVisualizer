#pragma once

#include "ofMain.h"
#include "Layout.h"
#include "GEVisualizer.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

// dynamic portion of layoutprojection (for common offsets like zoom and pan)
typedef struct {
    float xRotation = 20;
    float zRotation = 0;
    float zoomFactor = 1;
    POINT2D pan = {0, 0};

    void print() {
        printf("projection.dyn.xRotation: %f\n", xRotation);
        printf("projection.dyn.zRotation: %f\n", zRotation);
        printf("projection.dyn.zoomFactor: %f\n", zoomFactor);
        printf("projection.dyn.pan: (%f, %f)\n", pan.x, pan.y);
    }
} LayoutProjectionDynamic;

typedef struct {
    POINT2D offset = {0, 0};
    POINT2D scale = {1, 1};
    POINT2D screen_px_corner = {0,0};
    POINT2D real_corner = {0,0};
    double screenPixelsPerMeter = 1;
    LayoutProjectionDynamic dyn;

    void print() {
        printf("projection.offset: %f, %f\n", offset.x, offset.y);
        printf("projection.scale: %f, %f\n", scale.x, scale.y);
        printf("projection.screen_px_corner: %f, %f\n", screen_px_corner.x, screen_px_corner.y);
        printf("projection.real_corner: %f, %f\n", real_corner.x, real_corner.y);
        printf("projection.screenPixelsPerMeter: %f\n", screenPixelsPerMeter);
        dyn.print();
    }
} LayoutProjection;

typedef struct {
    bool structure = true; // NOTE: this has never been tested when false
    bool locations = true;
    bool texture = false;
    bool presence = false;
    bool userLocation = false;
    bool skeletons = true;
} LayoutRenderMode;


class LayoutRenderer {
public:
    // ~LayoutRenderer();

    void attachLayout(Layout* layout) { this->layout = layout; };
    void attachFonts(ofTrueTypeFont* fontMain, ofTrueTypeFont* fontMapNameLabel) {
        this->fontMain = fontMain;
        this->fontMapNameLabel = fontMapNameLabel;
    }

    void setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter);
    void reloadProjection();
    void render(LayoutRenderMode& renderMode, GEVisualizer& dataStore, float transition=0);

    Layout* layout;
    LayoutProjection projection;

    bool hoverClosestLocationExists     = false;
    unsigned int hoverClosestLocationID = 0;
    float hoverClosestLocationDistance  = 0;


private:
    // void recalculateTexture(GEVisualizer& store);
    void mouseTestRecalculateTexture();

    ofTrueTypeFont*  fontMain;
    ofTrueTypeFont*  fontMapNameLabel;


    ofTexture        texture;
    unsigned int     textureSize[2] = {0,0};
    unsigned char *  textureData = NULL;
};
