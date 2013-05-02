#include "LayoutApp.h"
#include <boost/lexical_cast.hpp>

void LayoutApp::setupUI_ViewOptions() {
    ofPtr<ofxUICanvas> guiViewOptions(new ofxUICanvas(
        ofGetWidth() - UI_VC_WIDTH - UI_VOPTS_WIDTH, UI_TOP_BAR_HEIGHT,
        UI_VOPTS_WIDTH, UI_VOPTS_HEIGHT));
    uiColors.apply(*guiViewOptions);
    viewGuis["ViewOptions"] = guiViewOptions;
    ofAddListener(guiViewOptions->newGUIEvent, this, &LayoutApp::guiEvent_ViewOptions);

    guiViewOptions->setWidgetSpacing((UI_VOPTS_WIDTH - UI_VOPTS_BTN_W) / 2.);

    guiViewOptions->addWidgetDown(new ofxUILabel(
        "View Options", OFX_UI_FONT_MEDIUM));

    vector<pair<string, string> > toggleable_attributes;
    toggleable_attributes.push_back(pair<string, string>("locations", "Show Locations"));
    toggleable_attributes.push_back(pair<string, string>("presence", "Show Presence"));
    toggleable_attributes.push_back(pair<string, string>("users", "Show Users"));
    toggleable_attributes.push_back(pair<string, string>("skels", "Show Skeletons"));

    typedef pair<string, string> attr_t;
    BOOST_FOREACH (attr_t attrs, toggleable_attributes) {
        string display_name = attrs.second;
        string internal_name = attrs.first;
        ofxUILabelToggle* button = new ofxUILabelToggle(
            internal_name, (bool) false, UI_VOPTS_BTN_W, 40, 0, 0, OFX_UI_FONT_SMALL);

        guiViewOptions->addWidgetDown(button);

        button->getLabel()->setFont(&fontVerd10);
        button->setLabelText(display_name);
        button->setValue(true);
    }
}

void LayoutApp::refreshUI_ViewOptions() {
    // show/hide ViewOptions panel
    if (((ofxUIToggle*) viewGuis["ViewControl"]->getWidget("View Options"))->getValue()) {
           viewGuis["ViewOptions"]->enable();
    } else viewGuis["ViewOptions"]->disable();
}

void LayoutApp::guiEvent_ViewOptions(ofxUIEventArgs &e) {
    // TODO debounce buttons
    printf("%i %s\n", e.widget->getKind(), e.widget->getName().c_str());

    if (e.widget->getKind() == OFX_UI_WIDGET_LABELTOGGLE) {
        ofxUILabelButton& btn = *((ofxUILabelButton*)e.widget);
        printf("%i %i %s\n", btn.getKind(), btn.getValue(), btn.getName().c_str());

        string mode = btn.getName();
        bool state = btn.getValue();

        if (mode == "locations") {
            mainRenderMode.locations = state;
        }
        else if (mode == "presence") {
            mainRenderMode.presence = state;
        }
        else if (mode == "users") {
            mainRenderMode.userLocation = state;
        }
        else if (mode == "skels") {
            mainRenderMode.skeletons = state;
        }
    }
}
