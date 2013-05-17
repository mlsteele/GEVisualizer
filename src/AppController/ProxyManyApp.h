/*
    ProxyManyApps

    Proxies all calls to several ofBaseApp's identified by name.
*/

#pragma once
#include <boost/foreach.hpp>

#include "ofMain.h"

class ProxyManyApps : public ofBaseApp {
private: typedef pair<string, ofPtr<ofBaseApp> > proxyApps_kv;
public:
    void setup()                                 { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->setup();                     } }
    void update()                                { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->update();                    } }
    void draw()                                  { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->draw();                      } }
    void exit()                                  { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->exit();                      } }
    void keyPressed(int key)                     { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->keyPressed(key);             } }
    void keyReleased(int key)                    { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->keyReleased(key);            } }
    void mouseMoved(int x, int y )               { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->mouseMoved(x, y );           } }
    void mouseDragged(int x, int y, int button)  { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->mouseDragged(x, y, button);  } }
    void mousePressed(int x, int y, int button)  { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->mousePressed(x, y, button);  } }
    void mouseReleased(int x, int y, int button) { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->mouseReleased(x, y, button); } }
    void windowResized(int w, int h)             { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->windowResized(w, h);         } }
    void dragEvent(ofDragInfo dragInfo)          { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->dragEvent(dragInfo);         } }
    void gotMessage(ofMessage msg)               { BOOST_FOREACH (proxyApps_kv pr, proxyApps) { pr.second->gotMessage(msg);             } }

protected:
    map<string, ofPtr<ofBaseApp> > proxyApps;
};
