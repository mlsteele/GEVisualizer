#pragma once

#include <iostream>
#include <string>
#include "ofUIObject.h"
#include "ofMain.h"
#include "ofUIView.h"

//Forward declaration
class ofUIImageButton;

typedef void (*ImageButtonCallbackFunction) (ofUIImageButton *button,void *appPointer);

class ofUIImageButton : public ofUIObject
{
public:
    ofUIImageButton();
	~ofUIImageButton(void);

    void init(string buttonID,ofRectangle bounds,ofImage buttonImage,ofImage rolloverImage,ofImage buttonDownImage);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    bool mouseMovedEvent(float mouseX,float mouseY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    
    //Getters
    unsigned int getState(){ return state; }
    
    //Setters
    void setImage(ofImage image);
    void setRolloverImage(ofImage image);
    void setButtonDownImage(ofImage image);
    void setCallback(ImageButtonCallbackFunction function);
    
    bool triggerCallback(void *appPointer);
	
	std::string text;
	std::string buttonID;
	ofColor backgroundColor;
	ofColor foregroundColor;
    
private:
    unsigned int state;
    ofImage buttonImage;
    ofImage rolloverImage;
    ofImage buttonDownImage;
    ImageButtonCallbackFunction callbackFunction;
    enum ButtonStates{NORMAL_BUTTON=0,ROLLOVER_BUTTON,BUTTON_DOWN};
};
