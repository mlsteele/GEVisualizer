#pragma once

#include "ofUIFrameView.h"

class ofUITabView : public ofUIView
{
public:
	ofUITabView();
	~ofUITabView(void);

    void init(unsigned int numTabs,ofRectangle bounds,ofColor backgroundColor,ofColor foregroundColor);
    bool update();
	bool draw(float parentX,float parentY);
    unsigned int numTabs(){ return tabs.size(); }
    
    bool showBorder;
    unsigned int borderWidth;
    ofColor borderColor;
    
    vector< ofUIFrameView > tabs;
    
    
};
