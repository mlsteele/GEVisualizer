// uiViewControl
// UI pane for LayoutApp
// Handles transformation controls and view options toggle

#include "LayoutApp.h"
#include <boost/lexical_cast.hpp>

void LayoutApp::setupUI_ViewControl() {
    ofPtr<ofxUICanvas> guiViewControl(new ofxUICanvas(
        ofGetWidth() - UI_VC_WIDTH, UI_TOP_BAR_HEIGHT,
        UI_VC_WIDTH, UI_VC_HEIGHT));
    uiColors.apply(*guiViewControl);
    viewGuis["ViewControl"] = guiViewControl;
    ofAddListener(guiViewControl->newGUIEvent, this, &LayoutApp::guiEvent_ViewControl);

    // guiViewControl.setFont(&fontVerd10)
    // guiViewControl->setWidgetSpacing(100);

    guiViewControl->addWidget(new ofxUIRotarySlider(
        UI_VC_ROTATION_X, UI_VC_ROTATION_Y, UI_VC_ROTATION_RAD, (float) 0, (float) 360, (float) 0, "Rotation"));
    ((ofxUIRotarySlider*) guiViewControl->getWidget("Rotation"))->getLabel()->setFont(&fontVerd10);

    guiViewControl->addWidget(new ofxUISlider(
        "Zoom", 1, 5, 1, UI_VC_ZOOM_W, UI_VC_ZOOM_H, UI_VC_ZOOM_X, UI_VC_ZOOM_Y));
    ((ofxUISlider*) guiViewControl->getWidget("Zoom"))->getLabel()->setFont(&fontVerd10);

    float button_side_padding = (UI_VC_WIDTH - UI_VC_BTN_W * 2) /3.;

    guiViewControl->addWidget(new ofxUILabelButton(
        "Reset View", (bool) false, UI_VC_BTN_W, UI_VC_BTN_H,
        button_side_padding, UI_VC_BTNS_Y, OFX_UI_FONT_MEDIUM));

    guiViewControl->addWidget(new ofxUILabelToggle(
        "View Options", (bool) false, UI_VC_BTN_W, UI_VC_BTN_H,
        UI_VC_BTN_W + button_side_padding * 2, UI_VC_BTNS_Y, OFX_UI_FONT_MEDIUM));
}

void LayoutApp::guiEvent_ViewControl(ofxUIEventArgs &e) {
    if (e.widget->getName() == "Rotation") {
        float val = (*(ofxUIRotarySlider*) e.widget).getScaledValue();
        layoutRenderTransform.zRotation = val;
    }

    if (e.widget->getName() == "Zoom") {
        float val = (*(ofxUISlider*) e.widget).getScaledValue();
        layoutRenderTransform.zoomFactor = val;
    }

    if (e.widget->getName() == "Reset View") {
        layoutRenderTransform = LayoutProjectionDynamic();

        BOOST_FOREACH(viewGui_t viewGuiPair, viewGuis) {
            ofxUIWidget* spinner = viewGuiPair.second->getWidget("Rotation");
            if (spinner != NULL) ((ofxUIRotarySlider*) spinner)->setValue(0);

            ofxUIWidget* zoomer = viewGuiPair.second->getWidget("Zoom");
            if (zoomer != NULL) ((ofxUISlider*) zoomer)->setValue(0);
        }
    }
}
