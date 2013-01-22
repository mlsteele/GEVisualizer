#pragma once
#include "ofUIObject.h"
#include "ofUIObject.h"
#include <iostream>
#include <string>
#include "ofMain.h"

class ofUIView : public ofUIObject
{
public:
	ofUIView();
	~ofUIView(void){}

    void init(){};
    bool update(){};
	bool draw(float parentX,float parentY){};
    
    unsigned int hSpacer;
    unsigned int vSpacer;
    ofColor backgroundColor;
	ofColor foregroundColor;
};
