#pragma once

#include "ofMain.h"
#include "Layout.h"
#include "GEVisualizer.h"

typedef struct {
    POINT2D offset = {0, 0};
    POINT2D scale = {1, 1};
} LayoutProjection;

class LayoutRenderer {
public:
    // TODO: free memory
    // ~LayoutRenderer() { delete[] textureData; }

    void attachLayout(Layout* layout) { this->layout = layout; };
    void attachResources(ofTrueTypeFont* font);

    void setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter);
    void render(GEVisualizer& store);

    Layout* layout;

private:
    LayoutProjection projection;
    ofTrueTypeFont*  mainFont;

    unsigned int     textureSize[2] = {20, 20};
    ofTexture        texture;
    unsigned char *  textureData;
};
