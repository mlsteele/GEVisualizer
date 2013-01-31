#pragma once

#include "ofMain.h"
#include "Layout.h"
#include "GEVisualizer.h"

class LayoutRenderer {
public:
    void attachLayout(Layout* layout) {
        printf("assigning layout\n");
        this->layout = layout;
    };

    void render(GEVisualizer& store);

    Layout* layout;
};
