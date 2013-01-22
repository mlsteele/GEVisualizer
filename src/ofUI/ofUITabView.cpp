#include "ofUITabView.h"

ofUITabView::ofUITabView()
{
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	backgroundColor.a = 255;
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	foregroundColor.a = 255;
    showBorder = true;
    borderWidth = 1;
    borderColor.r = 255;
	borderColor.g = 255;
	borderColor.b = 255;
	borderColor.a = 255;
    objectType = "UI_TAB_VIEW";
}

ofUITabView::~ofUITabView(){
    
}

void ofUITabView::init(unsigned int numTabs,ofRectangle bounds,ofColor backgroundColor = ofColor(255,255,255),ofColor foregroundColor = ofColor(255,255,255)){
    this->bounds = bounds;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    
    //Setup the tabs
    tabs.resize( numTabs );
}

bool ofUITabView::update(){
    
    return true;
}

bool ofUITabView::draw(float parentX,float parentY){
    
    if( !visible ){
        return true;
    }
    
    unsigned int headerX = 0;
    unsigned int headerY = 0;
    unsigned int headerWidth = 0;
    unsigned int headerHeight = 0; 
    unsigned int tabX = 0;
    unsigned int tabY = 0;
    unsigned int tabWidth = 0;
    unsigned int tabHeight = 0;
    
    originX = parentX;
    originY = parentY;
    
    //Draw the main tab view
    ofFill();
    ofSetColor(backgroundColor);
    ofRect(originX + bounds.x,originY + bounds.y,bounds.width,bounds.height);
    
    if( showBorder ){
        ofPtr<ofGLRenderer> render = ofGetGLRenderer();
        float currentLineWidth = render->getLineWidth();
        ofSetLineWidth( borderWidth );
        ofNoFill();
        ofSetColor(borderColor);
        ofRect(originX ,originY,bounds.width,bounds.height);
        ofSetLineWidth( currentLineWidth );
    }
    
    //Draw the tab buttons
    headerX = originX + bounds.x + hSpacer;
    headerY = originY + bounds.y + vSpacer;
    headerWidth = bounds.width - (hSpacer*2);
    headerHeight = vSpacer*3;
    ofFill();
    ofSetColor(255,255,255);
    ofRect(headerX,headerY,headerWidth,headerHeight);
    
    //Draw the active tab
    tabX = originX + bounds.x + hSpacer;
    tabY = headerY + + bounds.y + headerHeight + vSpacer;
    tabWidth = bounds.width - (hSpacer*2);
    tabHeight = bounds.height - (vSpacer*3) - headerHeight;
    ofFill();
    ofSetColor(255,255,255);
    ofRect(tabX,tabY,tabWidth,tabHeight);

    
    return true;
}

