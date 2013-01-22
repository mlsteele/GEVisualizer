#include "ofUIButton.h"
ofUIButton::ofUIButton():callbackFunction(NULL){
    bounds.x = 0;
	bounds.y = 0;
	bounds.width = 0;
	bounds.height = 0;
	text = "";
	buttonID = "";
    
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
    buttonVisible = true;
    state = NORMAL_BUTTON;
    objectType = "UI_BUTTON";
}

ofUIButton::~ofUIButton(void)
{
}

void ofUIButton::init(string buttonID,ofRectangle bounds,string text,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,bool buttonVisible){
    this->buttonID = buttonID;
    this->bounds = bounds;
    this->text = text;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    this->selectedColor = selectedColor;
    this->buttonVisible = buttonVisible;
}

void ofUIButton::draw(float parentX,float parentY)
{
	if( visible ){
		std::string buttonText = text;
        
        originX = parentX;
        originY = parentY;
        
        if( buttonVisible ){

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
            
            switch( state ){
                case NORMAL_BUTTON:
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
                    break;
                case ROLLOVER_BUTTON:
                    ofSetColor(selectedColor);
                    break;
                case BUTTON_DOWN:
                    ofSetColor(selectedColor);
                    break;
            }
            
            ofNoFill();
            ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);
            
        }

        if( font.isLoaded() ){
            float textX = originX + bounds.x;
            float textY = originY + bounds.y;
            ofRectangle rect = font.getStringBoundingBox(buttonText, 0,0);
            //If the text is smaller than the box then we try and center it within the box
            if( rect.width < bounds.width )
                textX = (textX+(bounds.width/2)) - (rect.width/2);
            if( rect.height < bounds.height )
                textY = (textY+(bounds.height/2)) + (rect.height/2);
            
            font.drawString(buttonText, textX, textY);
        }
	}
}

void ofUIButton::setFont(ofTrueTypeFont font){
	this->font = font;
}

void ofUIButton::setCallback(ButtonCallbackFunction function){
    callbackFunction = function;
}

bool ofUIButton::triggerCallback(void *appPointer){
    if( callbackFunction != NULL ){
        callbackFunction(this,appPointer);
        return true;
    }
    return false;
}

bool ofUIButton::mouseMovedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        state = ROLLOVER_BUTTON;
        return true;
    }
    if( state == ROLLOVER_BUTTON ){
        state = NORMAL_BUTTON;
    }
    return false;
}

bool ofUIButton::mousePressedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        state = BUTTON_DOWN;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUIButton::mouseReleasedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( state == BUTTON_DOWN )
    {
        state = NORMAL_BUTTON;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUIButton::mouseDraggedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {

        return true;
    }
    return false;
}

bool ofUIButton::setButtonText(string buttonID,string text){
    if( this->buttonID == buttonID ){
        this->text = text;
        return true;
    }
    return false;
}