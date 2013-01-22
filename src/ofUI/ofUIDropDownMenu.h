#pragma once

#include <iostream>
#include <string>
#include "ofMain.h"
#include "ofUIObject.h"
#include "ofUIView.h"

//Forward declaration
class ofUIDropDownMenu;

typedef void (*DropDownMenuCallbackFunction) (ofUIDropDownMenu *menu,void *appPointer);

class ofUIDropDownMenu : public ofUIObject
{
public:
    ofUIDropDownMenu();
	~ofUIDropDownMenu(void);

    void init(string dropDownMenuID,ofRectangle bounds,vector< string > textList,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,unsigned int currentItem = 0);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    bool mouseDraggedEvent(float mouseX,float mouseY);
    
    //Getters
    string getDropDownMenuID(){ return dropDownMenuID; }
    bool isSelected(){ return selected; }
    unsigned int getCurrentItem(){ return currentItem; }
    
    //Setters
    void setFont(ofTrueTypeFont font);
    void setCallback(DropDownMenuCallbackFunction function);
    void setSelected(bool selected){ this->selected = selected; }
    bool setCurrentItem(unsigned int item);
    
    bool triggerCallback(void *appPointer);
    
	vector< string > textList;
	std::string dropDownMenuID;
    
	ofColor backgroundColor;
	ofColor foregroundColor;
    ofColor selectedColor;
    
private:
    unsigned int currentItem;
    bool selected;
    ofTrueTypeFont font;
    DropDownMenuCallbackFunction callbackFunction;
};
