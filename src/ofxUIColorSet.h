#pragma once

typedef struct {
    ofColor color_back = OFX_UI_COLOR_BACK;
    ofColor color_outline = OFX_UI_COLOR_OUTLINE;
    ofColor color_outline_highlight = OFX_UI_COLOR_OUTLINE_HIGHLIGHT;
    ofColor color_fill = OFX_UI_COLOR_FILL;
    ofColor color_fill_highlight = OFX_UI_COLOR_FILL_HIGHLIGHT;
    ofColor color_padded_rect = OFX_UI_COLOR_PADDED;
    ofColor color_padded_rect_outline = OFX_UI_COLOR_PADDED_OUTLINE;     

    void apply(ofxUICanvas& c) const {
        c.setColorBack(color_back);
        c.setColorOutline(color_outline);
        c.setColorOutlineHighlight(color_outline_highlight);
        c.setColorFill(color_fill);
        c.setColorFillHighlight(color_fill_highlight);
        c.setColorPadded(color_padded_rect);
        c.setColorPaddedOutline(color_padded_rect_outline);
    }
} ofxUIColorSet;
