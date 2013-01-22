#pragma once

#include <iostream>
#include <string>
#include "ofUIObject.h"
#include "ofMain.h"
#include "ofUIView.h"

//Forward declaration
class ofUIButton;

typedef void (*ButtonCallbackFunction) (ofUIButton *button,void *appPointer);

class ofUIButton : public ofUIObject
{
public:
    ofUIButton();
	~ofUIButton(void);

    void init(string buttonID,ofRectangle bounds,string text,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,bool buttonVisible = true);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    
    //Getters
    string getButtonID(){ return buttonID; }
    string getText(){ return text; }
    bool isButtonVisible(){ return buttonVisible; }
    bool isButtonNormalState(){ return state==NORMAL_BUTTON; }
    bool isButtonRollOverState(){ return state==ROLLOVER_BUTTON; }
    bool isButtonDownState(){ return state==BUTTON_DOWN; }
    bool mouseMovedEvent(float mouseX,float mouseY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    bool mouseDraggedEvent(float mouseX,float mouseY);
    
    //Setters
    void setIsButtonVisible(bool buttonVisible){ this->buttonVisible = buttonVisible; }
    void setText(string text){ this->text = text; }
    void setFont(ofTrueTypeFont font);
    void setCallback(ButtonCallbackFunction function);
    bool setButtonText(string buttonID,string text);
    bool triggerCallback(void *appPointer);
	
	std::string buttonID;
	ofColor backgroundColor;
	ofColor foregroundColor;
    ofColor selectedColor;
    
private:
    std::string text;
    unsigned int state;
    bool buttonVisible;
    ofTrueTypeFont font;
    ButtonCallbackFunction callbackFunction;
    enum ButtonStates{NORMAL_BUTTON=0,ROLLOVER_BUTTON,BUTTON_DOWN};
};
