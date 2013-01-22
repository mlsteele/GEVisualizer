#pragma once

#include <iostream>
#include <string>
#include "ofMain.h"
#include "ofUIObject.h"

class ofUISlider;

typedef void (*SliderCallbackFunction) (ofUISlider *slider,void *appPointer);

class ofUISlider : public ofUIObject
{
public:
	ofUISlider();
	~ofUISlider(void);

	void init(string sliderID,ofRectangle bounds,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,float sliderValue,float sliderMinTargetValue,float sliderMaxTargetValue);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    bool mouseDraggedEvent(float mouseX,float mouseY);
    
    //Getters
    float getValue();
    float getMappedValue();
    bool isSelected(){ return selected; }
    
    //Setters
    void setCallback(SliderCallbackFunction function);
    void setSelected(bool selected){ this->selected = selected; }
    bool setValue(float v);
    bool setMappedValue(float v);
    void setMinTargetValue(float v){ sliderMinTargetValue = v; }
    void setMaxTargetValue(float v){ sliderMaxTargetValue = v; }
    
    bool triggerCallback(void *appPointer);
    
    string sliderID;
	ofColor backgroundColor;
	ofColor foregroundColor;
    ofColor selectedColor;
    
private:
    float sliderValue;
    float sliderMinTargetValue;
    float sliderMaxTargetValue;
    bool selected;
    SliderCallbackFunction callbackFunction;
};
