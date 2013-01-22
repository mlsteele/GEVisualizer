#include "ofUISlider.h"

ofUISlider::ofUISlider():callbackFunction(NULL){
    sliderID = "";
	bounds.x = 0;
	bounds.y = 0;
	bounds.width = 0;
	bounds.height = 0;
	selected = false;
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	backgroundColor.a = 255;
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	foregroundColor.a = 255;
    selectedColor.r = 0;
	selectedColor.g = 0;
	selectedColor.b = 0;
	selectedColor.a = 255;
	visible = true;
	enabled = true;
    editable = true;
    sliderValue = 0;
    sliderMinTargetValue = 0;
    sliderMaxTargetValue = 1;
    objectType = "UI_SLIDER";
}

ofUISlider::~ofUISlider(void){

}

void ofUISlider::init(string sliderID,ofRectangle bounds,ofColor fieldColor,ofColor textColor,ofColor selectedColor,float sliderValue = 0.5,float sliderMinTargetValue = 0,float sliderMaxTargetValue = 1.0){
    this->sliderID = sliderID;
    this->bounds = bounds;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    this->selectedColor = selectedColor;
    this->sliderValue = sliderValue;
    this->sliderMinTargetValue = sliderMinTargetValue;
    this->sliderMaxTargetValue = sliderMaxTargetValue;
}

void ofUISlider::draw(float parentX,float parentY){

	if( visible ){
        originX = parentX;
        originY = parentY;
        
		//Draw the background
		ofSetColor(backgroundColor.r,backgroundColor.g,backgroundColor.b,backgroundColor.a);
        ofFill();
		ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);
		
        //Set the text color
        if( selected ) ofSetColor(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a);
        else ofSetColor(foregroundColor.r,foregroundColor.g,foregroundColor.b,foregroundColor.a);
        
        //Draw the border
        ofNoFill();
		ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);
        
        //Draw the slider line
        float x = bounds.width/100.0*5.0;
        float y = bounds.height/2;
        float xx = bounds.width - (bounds.width/100.0*5.0);
        float yy = y;
        ofLine(originX + bounds.x + x, originY + bounds.y + y, originX + bounds.x + xx, originY + bounds.y + yy);
        
        //Draw the slider pos
        float sliderX = ofMap(sliderValue,0.0,1.0,x,xx,true);
        float sliderY = y;
        float sliderHeight = bounds.height/100.0*80.0;
        ofFill();
        ofEllipse(originX + bounds.x + sliderX,originY + bounds.y + sliderY, sliderHeight, sliderHeight);
	}
}

void ofUISlider::setCallback(SliderCallbackFunction function){
    callbackFunction = function;
}

bool ofUISlider::triggerCallback(void *appPointer){
    if( callbackFunction != NULL ){
        callbackFunction(this,appPointer);
        return true;
    }
    return false;
}

bool ofUISlider::mousePressedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        selected = !selected;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUISlider::mouseReleasedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( selected ){
        selected = false;
        return true;
    }
    return false;
}

bool ofUISlider::mouseDraggedEvent(float mouseX,float mouseY){
    if( !enabled || !selected || !editable ) return false;

    mouseX -= originX;
    mouseY -= originY;
    float x = bounds.width/100.0*5.0;
    float xx = bounds.width - (bounds.width/100.0*5.0);
    
    //Set the slider pos
    sliderValue = ofMap(mouseX,bounds.x + x,bounds.x + xx,0,1.0,true);

    //Trigger the callback
    triggerCallback(mainAppHandle);
    return true;
}

float ofUISlider::getValue(){
    return sliderValue;
}

float ofUISlider::getMappedValue(){
    return ofMap(sliderValue,0,1,sliderMinTargetValue,sliderMaxTargetValue);
}

bool ofUISlider::setValue(float v){
    if( v >=0 && v <= 1.0 ){
        sliderValue = v;
        return true;
    }
    return false;
}

bool ofUISlider::setMappedValue(float v){
    if( v >=sliderMinTargetValue && v <= sliderMaxTargetValue ){
        sliderValue = ofMap(v, sliderMinTargetValue, sliderMaxTargetValue, 0, 1,true);
        return true;
    }
    return false;
}