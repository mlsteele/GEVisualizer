#pragma once

#include "ofMain.h"

class ProxyManyApps : public ofBaseApp {
public:

public:
    void setup()                                 {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->setup();                     } }
    void update()                                {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->update();                    } }
    void draw()                                  {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->draw();                      } }
    void exit()                                  {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->exit();                      } }
    void keyPressed(int key)                     {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->keyPressed(key);             } }
    void keyReleased(int key)                    {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->keyReleased(key);            } }
    void mouseMoved(int x, int y )               {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->mouseMoved(x, y );           } }
    void mouseDragged(int x, int y, int button)  {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->mouseDragged(x, y, button);  } }
    void mousePressed(int x, int y, int button)  {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->mousePressed(x, y, button);  } }
    void mouseReleased(int x, int y, int button) {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->mouseReleased(x, y, button); } }
    void windowResized(int w, int h)             {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->windowResized(w, h);         } }
    void dragEvent(ofDragInfo dragInfo)          {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->dragEvent(dragInfo);         } }
    void gotMessage(ofMessage msg)               {  for (pair<string, ofPtr<ofBaseApp> > pr : proxyApps) { pr.second->gotMessage(msg);             } }

protected:
    map<string, ofPtr<ofBaseApp> > proxyApps;
};
