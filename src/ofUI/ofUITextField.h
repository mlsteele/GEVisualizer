#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include "ofMain.h"
#include "ofUIObject.h"

class ofUITextField;

typedef void (*TextFieldCallbackFunction) (ofUITextField *textField,void *appPointer);

class ofUITextField : public ofUIObject
{
public:
	ofUITextField();
	~ofUITextField(void);

	void init(string textFieldID,ofRectangle bounds,string text,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,bool isNumberBox = false);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool keyPressedEvent(int key);
    
    //Getters
    bool isSelected(){ return selected; }
    bool isNumberBox(){ return numberBox; }
    float getValueAsFloat();
    string getValueAsText();
    ofTrueTypeFont getFont(){ return font; }
    
    //Setters
    void setFont(ofTrueTypeFont font);
    void setCallback(TextFieldCallbackFunction function);
    void setSelected(bool selectionState);
    void setIsNumberBox(bool numberBox){ this->numberBox = numberBox; }
    bool setTextFieldValue(float value);
    bool setTextFieldValue(string text);
    
    bool triggerCallback(void *appPointer);
    
    template <class T> bool fromString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&)){
        std::istringstream iss(s);
        return !(iss >> f >> t).fail();
    }
    
    string toString(float f){
        stringstream converter;
        converter << f;
        return converter.str();
    }
    
    string textFieldID;
	
	std::string text;
	ofColor backgroundColor;
	ofColor foregroundColor;
    ofColor selectedColor;
    
private:
    bool selected;
    bool numberBox;
    ofTrueTypeFont font;
    TextFieldCallbackFunction callbackFunction;
};
