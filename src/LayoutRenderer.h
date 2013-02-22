#pragma once

#include "ofMain.h"
#include "Layout.h"
#include "GEVisualizer.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

typedef struct {
    POINT2D offset = {0, 0};
    POINT2D scale = {1, 1};
    POINT2D screen_px_corner = {0,0};
    POINT2D real_corner = {0,0};
    double screenPixelsPerMeter = 1;
} LayoutProjection;

typedef struct {
    bool structure = true; // NOTE: this has never been tested when false
    bool locations = true;
    bool texture = false;
    bool presence = false;
    bool userLocation = false;
} LayoutRenderMode;


class LayoutRenderer {
public:
    // TODO: free memory
    // ~LayoutRenderer() { delete[] textureData; }



    enum RenderMode {
        RenderStructure,
        RenderStructureData
    };
    
    void attachLayout(Layout* layout) { this->layout = layout; };
    void attachFonts(ofTrueTypeFont* fontMain, ofTrueTypeFont* fontMapNameLabel) {
        this->fontMain = fontMain;
        this->fontMapNameLabel = fontMapNameLabel;
    }

    void setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter);
    void render(LayoutRenderMode& renderMode, GEVisualizer& dataStore, float transition=0);

    Layout* layout;

private:
    void recalculateTexture(GEVisualizer& store);
    void mouseTestRecalculateTexture();

    LayoutProjection projection;
    ofTrueTypeFont*  fontMain;
    ofTrueTypeFont*  fontMapNameLabel;

    ofTexture        texture;
    unsigned int     textureSize[2] = {0,0};
    unsigned char *  textureData = NULL;
};
