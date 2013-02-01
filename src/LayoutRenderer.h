#pragma once

#include "ofMain.h"
#include "Layout.h"
#include "GEVisualizer.h"

typedef struct {
    POINT2D offset = {0, 0};
    POINT2D scale = {1, 1};
    POINT2D screen_px_corner = {0,0};
    POINT2D real_corner = {0,0};
    double screenPixelsPerMeter = 1;
} LayoutProjection;

class LayoutRenderer {
public:
    // TODO: free memory
    // ~LayoutRenderer() { delete[] textureData; }

    void attachLayout(Layout* layout) { this->layout = layout; };
    void attachFont(ofTrueTypeFont* font) { mainFont = font; }

    void setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter);
    void render(GEVisualizer& store);

    Layout* layout;

private:
    void recalculateTexture(GEVisualizer& store);

    LayoutProjection projection;
    ofTrueTypeFont*  mainFont;

    ofTexture        texture;
    unsigned int     textureSize[2] = {0,0};
    unsigned char *  textureData = NULL;
};
