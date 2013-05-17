// uiMapSelect
// UI pane for LayoutApp
// Handles layout selection

// TODO add building selection

#include "LayoutApp.h"
#include <boost/lexical_cast.hpp>

void LayoutApp::setupUI_MapSelect() {

    // ofPtr<ofxUIScrollableCanvas> guiMapSelectScroll(new ofxUIScrollableCanvas(
    //     ofGetWidth() - UI_MAPS_WIDTH, UI_TOP_BAR_HEIGHT + UI_VC_HEIGHT + 10,
    //     UI_MAPS_WIDTH, UI_MAPS_HEIGHT));

    ofPtr<ofxUICanvas> guiMapSelect(new ofxUICanvas(
        ofGetWidth() - UI_MAPS_WIDTH, UI_TOP_BAR_HEIGHT + UI_VC_HEIGHT,
        UI_MAPS_WIDTH, UI_MAPS_HEIGHT));
    uiColors.apply(*guiMapSelect);
    viewGuis["MapSelect"] = guiMapSelect;
    ofAddListener(guiMapSelect->newGUIEvent, this, &LayoutApp::guiEvent_MapSelect);

    // TODO make scrollable for many floors
    // ((ofxUIScrollableCanvas*) &*guiMapSelect)->setScrollAreaToScreenHeight();
    // guiMapSelectScroll->setScrollArea(
    //     ofGetWidth() - UI_MAPS_WIDTH, UI_TOP_BAR_HEIGHT + UI_VC_HEIGHT + 10,
    //     UI_MAPS_WIDTH, UI_MAPS_HEIGHT);

    vector<string> layoutNames;
    BOOST_FOREACH (LayoutRenderer& layoutRenderer, layoutRenderers) {
        layoutNames.push_back(layoutRenderer.layout->layoutName);
    }

    guiMapSelect->addWidgetDown(new ofxUIRadio("Floors", layoutNames, 0, 50, 50));
}

void LayoutApp::guiEvent_MapSelect(ofxUIEventArgs &e) {
    if (e.widget->getKind() == OFX_UI_WIDGET_TOGGLE) {
        for (int i = 0; i < layoutRenderers.size(); i++) {
            Layout& layout = *layoutRenderers[i].layout;
            if (boost::ends_with(e.widget->getName(), layout.layoutName)) {
                renderers_active_i = i;
                return;
            }
        }
        printf("WARN: failed to find matching layout for button\n");
    }
}
