#pragma once

#include <iostream>
#include <string>
#include "ofMain.h"

class ofUIObject
{
public:
    ofUIObject(){
        bounds.x = 0;
        bounds.y = 0;
        bounds.width = 0;
        bounds.height = 0;
        originX = 0;
        originY = 0;
        visible = true;
        enabled = true;
        editable = true;
        mainAppHandle = NULL;
        objectType = "UI_OBJECT";
    }
	~ofUIObject(void){}

    //Main GUI Interface Methods (these should be overridden in the inherited class if you want to action them)
    void update(){}
    void draw(float x,float y){}
    bool mouseMovedEvent(float mouseX,float mouseY){ return false; }
    bool mousePressedEvent(float mouseX,float mouseY){ return false; }
    bool mouseReleasedEvent(float x,float y){ return false; }
    bool mouseDraggedEvent(float x,float y){ return false; }
    bool keyPressedEvent(int key){ return false; }
    bool keyReleasedEvent(int key){ return false; }
    
    //Getters
    bool isEnabled(){ return enabled; }
    bool isVisible(){ return visible; }
    bool isEditable(){ return editable; }
    ofRectangle getBounds(){ return bounds; }
    float getX(){ return bounds.x; }
    float getY(){ return bounds.y; }
    float getWidth(){ return bounds.width; }
    float getHeight(){ return bounds.height; }
    float getScreenX(){ return (originX+bounds.x); }
    float getScreenY(){ return (originY+bounds.y); }
    string getObjectType(){ return objectType; }
    bool isMainAppHandleSet(){ if( mainAppHandle == NULL ){ return false; } return true; }
	
    //Setters
    void setPos(float x,float y){ bounds.x = x; bounds.y = y; }
    void setBounds(ofRectangle bounds){ this->bounds = bounds; }
    void setEnabled(bool enabled){ this->enabled = enabled; }
	void setVisible(bool visible){ this->visible = visible; }
    void setEditable(bool editable){ this->editable = editable; }
    void setMainAppHandle(void *mainAppHandle){ this->mainAppHandle = mainAppHandle; }
    
protected:
    ofRectangle bounds;
    string objectType;
    bool enabled;
	bool visible;
    bool editable;
    float originX;
    float originY;
    void *mainAppHandle;
	
};
