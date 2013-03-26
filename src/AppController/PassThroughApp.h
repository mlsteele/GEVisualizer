#pragma once

#include "ofMain.h"

class PassThroughApp : public ofBaseApp {
public:
    PassThroughApp() {}
    PassThroughApp(ofPtr<ofBaseApp> proxyApp) : proxyApp(proxyApp) {}
    setApp(ofPtr<ofBaseApp> app) { this->proxyApp = app; }

public:
    void PassThroughApp::setup()                                 {  if (proxyApp) proxyApp->setup();                     }
    void PassThroughApp::update()                                {  if (proxyApp) proxyApp->update();                    }
    void PassThroughApp::draw()                                  {  if (proxyApp) proxyApp->draw();                      }
    void PassThroughApp::exit()                                  {  if (proxyApp) proxyApp->exit();                      }
    void PassThroughApp::keyPressed(int key)                     {  if (proxyApp) proxyApp->keyPressed(key);             }
    void PassThroughApp::keyReleased(int key)                    {  if (proxyApp) proxyApp->keyReleased(key);            }
    void PassThroughApp::mouseMoved(int x, int y )               {  if (proxyApp) proxyApp->mouseMoved(x, y );           }
    void PassThroughApp::mouseDragged(int x, int y, int button)  {  if (proxyApp) proxyApp->mouseDragged(x, y, button);  }
    void PassThroughApp::mousePressed(int x, int y, int button)  {  if (proxyApp) proxyApp->mousePressed(x, y, button);  }
    void PassThroughApp::mouseReleased(int x, int y, int button) {  if (proxyApp) proxyApp->mouseReleased(x, y, button); }
    void PassThroughApp::windowResized(int w, int h)             {  if (proxyApp) proxyApp->windowResized(w, h);         }
    void PassThroughApp::dragEvent(ofDragInfo dragInfo)          {  if (proxyApp) proxyApp->dragEvent(dragInfo);         }
    void PassThroughApp::gotMessage(ofMessage msg)               {  if (proxyApp) proxyApp->gotMessage(msg);             }

protected:
    ofPtr<ofBaseApp> proxyApp;
};
