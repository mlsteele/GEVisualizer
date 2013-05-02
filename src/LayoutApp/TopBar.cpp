#include "LayoutApp.h"
#include <boost/lexical_cast.hpp>

void LayoutApp::setupUI_TopBar() {
    ofPtr<ofxUICanvas> guiTopBar(new ofxUICanvas(0, 0, ofGetWidth(), UI_TOP_BAR_HEIGHT));
    uiColors.apply(*guiTopBar);
    viewGuis["TopBar"] = guiTopBar;
    ofAddListener(guiTopBar->newGUIEvent, this, &LayoutApp::guiEvent_TopBar);

    guiTopBar->setWidgetSpacing((UI_TOP_BAR_HEIGHT - UI_BUTTON_TB_H) / 2.f - 2);

    guiTopBar->addWidgetRight(new ofxUILabelButton(
        "Connect", (bool) false, UI_BUTTON_TB_W, UI_BUTTON_TB_H,
        0, 0, OFX_UI_FONT_MEDIUM));

    guiTopBar->addWidgetRight(new ofxUILabelButton(
        "Server Info", (bool) false, UI_BUTTON_TB_W, UI_BUTTON_TB_H,
        0, 0, OFX_UI_FONT_MEDIUM));

    guiTopBar->addWidgetRight(new ofxUISpacer(
        0, 0,
        UI_TB_MID_SPACEX, 0));

    guiTopBar->addWidgetRight(new ofxUILabelButton(
        "Server IP", (bool) false, UI_BUTTON_TB_SERVER_IP_W, UI_BUTTON_TB_H,
        0, 0, OFX_UI_FONT_MEDIUM));

    guiTopBar->addWidgetRight(new ofxUILabelButton(
        "Client Port", (bool) false, UI_BUTTON_TB_CLI_PORT_W, UI_BUTTON_TB_H,
        0, 0, OFX_UI_FONT_MEDIUM));

    guiTopBar->addWidgetRight(new ofxUILabelButton(
        "Sensor Count", (bool) false, UI_BUTTON_TB_SENSC_W, UI_BUTTON_TB_H,
        0, 0, OFX_UI_FONT_MEDIUM));

    refreshUI_TopBar();
}

void LayoutApp::refreshUI_TopBar() {
    ofPtr<ofxUICanvas> guiTopBar(viewGuis["TopBar"]);

    ((ofxUILabelButton*) guiTopBar->getWidget("Server IP"))->setLabelText("Server IP: ???.???.???.???:?????");
    ((ofxUILabelButton*) guiTopBar->getWidget("Client Port"))->setLabelText("Client Port: ?????");
    ((ofxUILabelButton*) guiTopBar->getWidget("Sensor Count"))->setLabelText("Sensors: ?");

    // visConfig.ge_server_host
    // visConfig.ge_server_port
    // visConfig.ge_client_port
    ((ofxUILabelButton*) guiTopBar->getWidget("Server IP"))->setLabelText(
        "Server IP: " + lexical_cast<string>(visConfig.ge_server_host) +
        ":" + lexical_cast<string>(visConfig.ge_server_port));

    ((ofxUILabelButton*) guiTopBar->getWidget("Client Port"))->setLabelText(
        "Client Port: " + lexical_cast<string>(visConfig.ge_client_port));

    ((ofxUILabelButton*) guiTopBar->getWidget("Sensor Count"))->setLabelText(
        "Sensors: " + lexical_cast<string>(gelink.getLocationInfo().size()));
}

void LayoutApp::guiEvent_TopBar(ofxUIEventArgs &e) {
    if (e.widget->getKind() == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton& btn = *((ofxUILabelButton*)e.widget);
        if (btn.getValue()) {
            printf("%i %i %s\n", btn.getKind(), btn.getValue(), btn.getName().c_str());

            if (btn.getName() == "Connect" || btn.getName() == "Reconnect") {
                printf("guiEvent.TopBar.Connect||Reconnect\n");
                gelink.connect(visConfig.ge_server_host, visConfig.ge_server_port, visConfig.ge_client_port);
                gelink.sendLocationInfo();
                gelink.streamUserPresenceData(true);
                gelink.streamUserLocationData(true);
                gelink.streamUserJointData(true);

                // TODO fix this
                btn.setLabelText("Reconnect");
                btn.setName("Reconnect");
            }

            if (btn.getName() == "Server Info") {
                // TODO implement server info
                printf("WARN: Server info not yet implemented\n");
            }
        }
    }
}
