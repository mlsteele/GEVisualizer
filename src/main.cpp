#include "ofMain.h"
#include "LayoutApp.h"
#include "ViewControllerApp.h"
#include "ofAppGlutWindow.h"

int main( ){
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);

    // ViewControllerApp is used here because of a suspicion that
    // LayoutApp might be combined with other OF apps.
    // It could be replaced with just
    // ofRunApp(new LayoutApp())

    ViewControllerApp* vc = new ViewControllerApp();
    vc->registerApp("layouts", ofPtr<ViewControllerApp::MemberApp>(new LayoutApp()));
    vc->enableApp("layouts");
    ofRunApp(vc);
}
