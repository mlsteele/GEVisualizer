#include "ofUIRadioButton.h"
ofUIRadioButton::ofUIRadioButton():callbackFunction(NULL){
    radioButtonID = "";
    bounds.x = 0;
	bounds.y = 0;
	bounds.width = 0;
	bounds.height = 0;
    
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	backgroundColor.a = 255;
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	foregroundColor.a = 255;
	visible = true;
	enabled = true;
    editable = true;
    state = false;
    objectType = "UI_RADIO_BUTTON";
}

ofUIRadioButton::~ofUIRadioButton(void)
{
}

void ofUIRadioButton::init(string radioButtonID,ofRectangle bounds,ofColor backgroundColor = ofColor(255,255,255),ofColor foregroundColor = ofColor(0,0,0)){
    this->radioButtonID = radioButtonID;
    this->bounds = bounds;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
}

void ofUIRadioButton::draw(float parentX,float parentY)
{
	if( visible ){
        originX = parentX;
        originY = parentY;

		//Draw the button
		if( enabled ) ofSetColor(backgroundColor.r,backgroundColor.g,backgroundColor.b,backgroundColor.a);
		else{
			int r = backgroundColor.r;
			int g = backgroundColor.g;
			int b = backgroundColor.b;
			int a = backgroundColor.a;
			if( r > 50 ) r -= 50;
			if( g > 50 ) g -= 50;
			if( b > 50 ) b -= 50;
			if( a > 50 ) a -= 50;

			ofSetColor(r,g,b,a);
		}
		ofFill();
		ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);

		if( enabled ) ofSetColor(foregroundColor.r,foregroundColor.g,foregroundColor.b,foregroundColor.a);
		else{
			int r = foregroundColor.r;
			int g = foregroundColor.g;
			int b = foregroundColor.b;
			int a = foregroundColor.a;
			if( r >= 50 ) r -= 50;
			if( g >= 50 ) g -= 50;
			if( b >= 50 ) b -= 50;
			if( a >= 50 ) a -= 50;

			ofSetColor(r,g,b,a);
		}
        
        ofNoFill();
		ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);
        
        float x = 0;
        float y = 0;
        float xx = 0;
        float yy = 0;
        
        if( state ){
            x = bounds.width/100.0*10.0;
            y = bounds.height/100.0*10.0;
            xx = bounds.width - (bounds.width/100.0*10.0);
            yy = bounds.height - (bounds.height/100.0*10.0);
            ofLine(originX + bounds.x + x,originY + bounds.y + y,originX + bounds.x +  xx, originY + bounds.y + yy);
            
            ofLine(originX + bounds.x + xx,originY + bounds.y + y,originX + bounds.x +  x, originY + bounds.y + yy);
        }
	}
}

void ofUIRadioButton::setCallback(RadioButtonCallbackFunction function){
    callbackFunction = function;
}

bool ofUIRadioButton::triggerCallback(void *appPointer){
    if( callbackFunction != NULL ){
        callbackFunction(this,appPointer);
        return true;
    }
    return false;
}

bool ofUIRadioButton::mousePressedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        state = !state;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUIRadioButton::getStateOfRadioButton(string radioButtonID,bool &state){
    if( radioButtonID == this->radioButtonID ){
        state = this->state;
        return true;
    }
    return false;
}
