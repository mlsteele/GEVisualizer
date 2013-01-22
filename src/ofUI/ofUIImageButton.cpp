#include "ofUIImageButton.h"
ofUIImageButton::ofUIImageButton():callbackFunction(NULL){
    bounds.x = 0;
	bounds.y = 0;
	bounds.width = 0;
	bounds.height = 0;
	buttonID = "";
    
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
    state = NORMAL_BUTTON;
    objectType = "UI_IMAGE_BUTTON";
}

ofUIImageButton::~ofUIImageButton(void)
{
}

void ofUIImageButton::init(string buttonID,ofRectangle bounds,ofImage buttonImage,ofImage rolloverImage,ofImage buttonDownImage){
    this->buttonID = buttonID;
    this->bounds = bounds;
    this->buttonImage = buttonImage;
    this->rolloverImage = rolloverImage;
    this->buttonDownImage = buttonDownImage;
}

void ofUIImageButton::draw(float parentX,float parentY)
{
	if( visible ){
        
        originX = parentX;
        originY = parentY;

        ofRectangle rect;
        rect.x = originX + bounds.x;
        rect.y = originY + bounds.y;
        rect.width = bounds.width;
        rect.height = bounds.height;
        
        ofSetColor(255, 255, 255);
        ofFill();
        ofRect(rect);
        switch ( state ) {
            case NORMAL_BUTTON:
                buttonImage.draw(rect);
                break;
            case ROLLOVER_BUTTON:
                rolloverImage.draw(rect);
                break;
            case BUTTON_DOWN:
                buttonDownImage.draw(rect);
                break;
            default:
                break;
        }
        

	}
}

void ofUIImageButton::setImage(ofImage image){
    buttonImage = image;
}

void ofUIImageButton::setRolloverImage(ofImage image){
    rolloverImage = image;
}

void ofUIImageButton::setButtonDownImage(ofImage image){
    buttonDownImage = image;
}

void ofUIImageButton::setCallback(ImageButtonCallbackFunction function){
    callbackFunction = function;
}

bool ofUIImageButton::triggerCallback(void *appPointer){
    if( callbackFunction != NULL ){
        callbackFunction(this,appPointer);
        return true;
    }
    return false;
}

bool ofUIImageButton::mouseMovedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
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

bool ofUIImageButton::mousePressedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        state = BUTTON_DOWN;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUIImageButton::mouseReleasedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    if( state == BUTTON_DOWN )
    {
        state = NORMAL_BUTTON;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

