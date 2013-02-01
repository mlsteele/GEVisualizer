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
    void attachLayout(Layout* layout) { this->layout = layout; };
    void attachMainFont(ofTrueTypeFont* font) { mainFont = font; }

    void setupProjection(POINT2D screen_px_corner, POINT2D real_corner, double screenPixelsPerMeter);
    void render(GEVisualizer& store);

    Layout* layout;

private:
    LayoutProjection projection;
    ofTrueTypeFont* mainFont;
};
