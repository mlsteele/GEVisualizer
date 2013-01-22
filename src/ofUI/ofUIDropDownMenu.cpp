#include "ofUIDropDownMenu.h"
ofUIDropDownMenu::ofUIDropDownMenu():callbackFunction(NULL){
    bounds.x = 0;
	bounds.y = 0;
	bounds.width = 0;
	bounds.height = 0;
	dropDownMenuID = "";
    
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
	selected = false;
    currentItem = 0;
    objectType = "UI_DROP_DOWN_MENU";
}

ofUIDropDownMenu::~ofUIDropDownMenu(void)
{
}

void ofUIDropDownMenu::init(string dropDownMenuID,ofRectangle bounds,vector< string > textList,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,unsigned int currentItem){
    this->dropDownMenuID = dropDownMenuID;
    this->bounds = bounds;
    this->textList = textList;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    this->selectedColor = selectedColor;
    this->currentItem = currentItem;
}

void ofUIDropDownMenu::draw(float parentX,float parentY)
{
	if( visible ){
        
        originX = parentX;
        originY = parentY;
        
        if( selected ){
            string text = textList[ currentItem ]; 
            ofRectangle rect = font.getStringBoundingBox(text, 0,0);
            float textHeight = rect.height * 1.5;
            float menuWidth = bounds.width;
            float menuHeight = (textHeight * textList.size()) + (rect.height/2.0);
            ofSetColor(backgroundColor.r,backgroundColor.g,backgroundColor.b,backgroundColor.a);
            ofFill();
            ofRect(originX + bounds.x, originY + bounds.y, menuWidth, menuHeight);
            
            //Draw the border
            ofSetColor(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a);
            ofNoFill();
            ofRect(originX + bounds.x, originY + bounds.y, menuWidth, menuHeight);
            
            //Draw the selected item
            ofSetColor(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a);
            ofNoFill();
            ofRect(originX + bounds.x, originY + bounds.y, menuWidth, menuHeight);
            
            for(unsigned int i=0; i<textList.size(); i++){
                if( i == currentItem ) ofSetColor(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a);
                else ofSetColor(foregroundColor.r,foregroundColor.g,foregroundColor.b,foregroundColor.a);
                float textX = originX + bounds.x + 5;
                float textY = originY + bounds.y + + (bounds.height/2.0) + (rect.height/2.0) + (textHeight * i);
                text = textList[i];
                //If the text is smaller than the box then we try and center it within the box
                
                font.drawString(text, textX, textY);
            }
            
        }else{
            std::string buttonText = textList[ currentItem ];
            
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
            
            float textX = originX + bounds.x;
            float textY = originY + bounds.y;
            ofRectangle rect = font.getStringBoundingBox(buttonText, 0,0);
            //If the text is smaller than the box then we try and center it within the box
            if( rect.width < bounds.width )
                textX = textX+ 5;
            if( rect.height < bounds.height )
                textY = (textY+(bounds.height/2)) + (rect.height/2);
            
            font.drawString(buttonText, textX, textY);
            
        }
	}
}

void ofUIDropDownMenu::setFont(ofTrueTypeFont font){
	this->font = font;
}

void ofUIDropDownMenu::setCallback(DropDownMenuCallbackFunction function){
    callbackFunction = function;
}

bool ofUIDropDownMenu::triggerCallback(void *appPointer){
    if( callbackFunction != NULL ){
        callbackFunction(this,appPointer);
        return true;
    }
    return false;
}

bool ofUIDropDownMenu::mousePressedEvent(float mouseX,float mouseY){
    if( !enabled ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        selected = true;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUIDropDownMenu::mouseReleasedEvent(float mouseX,float mouseY){
    if( !enabled ) return false;
    mouseX -= originX;
    mouseY -= originY;
    selected = false;
    return true;
}

bool ofUIDropDownMenu::mouseDraggedEvent(float mouseX,float mouseY){
    if( !enabled || !selected ) return false;
    mouseX -= originX;
    mouseY -= originY;
    
    string text = "Test"; 
    ofRectangle rect = font.getStringBoundingBox(text, 0,0);
    float textHeight = rect.height * 1.5;
    float menuHeight = textHeight * textList.size();

    //Set the slider pos
    float itemID = ofMap(mouseY,bounds.y,bounds.y + menuHeight,0,textList.size()-1,true);
    currentItem = (unsigned int)floor(itemID);
    
    return true;
}

bool ofUIDropDownMenu::setCurrentItem(unsigned int item){
    if( item <= textList.size() -1 ){
        currentItem = item;
        return true;
    }
    return false;
}
