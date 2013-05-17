#include "ofMain.h"
#include "LayoutApp.h"
#include "ViewControllerApp.h"
#include "ofAppGlutWindow.h"

int main( ){
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);

    // ofSetVerticalSync(true);
    // ofPtr<DummyApp> dummy(new DummyApp());
    // ofRunApp(new ClickThroughApp(dummy));

    //  ViewControllerApp vc;
    //  vc.addApp("layouts", ofPtr<ofBaseApp>(new LayoutApp()));
    //  vc.addApp("dummy", ofPtr<ofBaseApp>(new DummyApp()));
    //  vc.selectApp("dummy");
    //  ofRunApp(&vc);

    ViewControllerApp* vc = new ViewControllerApp();
    vc->registerApp("layouts", ofPtr<ViewControllerApp::MemberApp>(new LayoutApp()));
    vc->enableApp("layouts");
    ofRunApp(vc);
}
