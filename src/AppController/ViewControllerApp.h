#pragma once
#include <boost/foreach.hpp>

#include "ofMain.h"
#include "ProxyManyApp.h"
#include "ViewAppBase.h"

class ViewControllerApp : public ProxyManyApps {
public: typedef ViewAppBase MemberApp;
public:
    void registerApp(string name, ofPtr<MemberApp> app) {
        apps[name] = app;
        apps[name]->setup();
        apps[name]->disable();
    }

    void disableAll() {
        typedef pair<string, ofPtr<MemberApp> > apps_kv;
        BOOST_FOREACH (apps_kv pr, apps) {
            pr.second->disable();
        }
    }

    void enableApp(string name) {
        validate_name(name);
        apps[name]->enable();
        if (proxyApps.find(name) != proxyApps.end()) { // if it is not already active
            printf("WARN: app \"%s\" is already being proxied to\n", name.c_str());
        } else {
            proxyApps[name] = apps[name];
        }
    }

    void disableApp(string name) {
        validate_name(name);
        apps[name]->disable();
        proxyApps.erase(name);
    }

    // TODO FIXME this should be a const method
    MemberApp& getApp(string name) {
        validate_name(name);
        return *apps[name];
    }

private:
    void validate_name(string name) {
        if (apps.find(name) != apps.end()) {
        } else {
          printf("ERR: ViewControllerApp: unknown app name \"%s\"\n", name.c_str());
        }
    }

protected:
    map<string, ofPtr<MemberApp> > apps;
};
