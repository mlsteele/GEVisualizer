#pragma once

#include <iostream>
#include <string>
#include "ofMain.h"
#include "ofUIObject.h"
#include "ofUIView.h"

//Forward declaration
class ofUIRadioButton;

typedef void (*RadioButtonCallbackFunction) (ofUIRadioButton *radioButton,void *appPointer);

class ofUIRadioButton : public ofUIObject
{
public:
    ofUIRadioButton();
	~ofUIRadioButton(void);

    void init(string radioButtonID,ofRectangle bounds,ofColor backgroundColor,ofColor foregroundColor);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    bool mousePressedEvent(float mouseX,float mouseY);
    
    //Getters
    bool getState(){ return state; }
    string getRadioButtonID(){ return radioButtonID; }
    bool getStateOfRadioButton(string radioButtonID,bool &state);

    
    //Setters
    void setCallback(RadioButtonCallbackFunction function);
    void setState(bool state){ this->state = state; }
	    
    bool triggerCallback(void *appPointer);
	
	string radioButtonID;
	ofColor backgroundColor;
	ofColor foregroundColor;
    
private:
    bool state;
    RadioButtonCallbackFunction callbackFunction;
};
