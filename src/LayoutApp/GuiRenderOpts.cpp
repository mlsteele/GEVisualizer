#include "LayoutApp.h"

vector<string> generate_toggleable_attributes() {
    vector<string> toggleable_attributes;
    toggleable_attributes.push_back("Locations");
    // toggleable_attributes.push_back("Texture");
    toggleable_attributes.push_back("Presence");
    toggleable_attributes.push_back("Users");
    return toggleable_attributes;
}

vector<string> toggleable_attributes = generate_toggleable_attributes();

void LayoutApp::setupUIRenderOpts(){
    ofPtr<ofxUICanvas> renderOptsGui(new ofxUICanvas(0, 0, 200, 200));
    viewGuis.push_back(renderOptsGui);
    ofAddListener(renderOptsGui->newGUIEvent, this, &LayoutApp::guiEventRenderOpts);

    ofxUIWidget* cursor = NULL;

    for (string attr : toggleable_attributes) {
        // ofxUILabelToggle (
        //     string _name, bool _value, float w=0, float h=0, float x=0, float y=0, int _size=OFX_UI_FONT_MEDIUM)

        renderOptsGui->addWidgetDown(new ofxUILabelToggle(
            attr, (bool) false, 124, 40, 0, 0, OFX_UI_FONT_SMALL));
    }
}

// void LayoutApp::UIRenderOptsRefresh() {
//     setValue
// }

void LayoutApp::guiEventRenderOpts(ofxUIEventArgs &e) {
    ofxUILabelToggle* toggle = (ofxUILabelToggle*) e.widget;

    for (string attr : toggleable_attributes) {
        if (e.widget->getName() == attr) {
            printf("guiEventRenderOpts button event %s", toggle->getName().c_str());
        }
    }

    if (e.widget->getName() == "Locations") {
        mainRenderMode.locations = toggle->getValue();
        gelink.streamUserPresenceData(toggle->getValue());
    }

    if (e.widget->getName() == "Users") {
        mainRenderMode.userLocation = toggle->getValue();
        gelink.streamUserLocationData(toggle->getValue());
    }

    if (e.widget->getName() == "Skeletons") {
        gelink.streamUserJointData(toggle->getValue());
    }
}
