#pragma once

#include "ofUIView.h"
#include "ofUIButton.h"
#include "ofUITextField.h"
#include "ofUIRadioButton.h"
#include "ofUISlider.h"
#include "ofUIDropDownMenu.h"
#include "ofUIImageButton.h"
#include "ofUIStaticText.h"

class ofUIFrameView : public ofUIView
{
public:
	ofUIFrameView();
	~ofUIFrameView(void);

    //Setup functions
    void init(string frameViewID,ofRectangle bounds,ofColor backgroundColor,ofColor foregroundColor);
    void addButton(ofUIButton button);
    void addTextField(ofUITextField textField);
    void addRadioButton(ofUIRadioButton radioButton);
    void addSlider(ofUISlider slider);
    void addDropDownMenu(ofUIDropDownMenu dropDownMenu);
    void addImageButton(ofUIImageButton imageButton);
    void addStaticText(ofUIStaticText staticText);
    
    //Main GUI Interface Methods
    bool update();
	bool draw(float parentX,float parentY);
    bool mouseMovedEvent(float mouseX,float mouseY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    bool mouseDraggedEvent(float mouseX,float mouseY);
    bool keyPressedEvent(int key);
    
    //Getters
    bool getValueFromTextFieldAsFloat(string textFieldID,float &value);
    bool getValueFromTextFieldAsString(string textFieldID,string &text);
    bool getValueFromSlider(string sliderID,float &value);
    bool getMappedValueFromSlider(string sliderID,float &value);
    bool getStateOfRadioButton(string radioButtonID,bool &state);
    bool getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem);
    ofUIButton* getUIButtonPointer(string buttonID);
    
    //Setters
    bool setTextFieldValue(string textFieldID,float value);
    bool setTextFieldValue(string textFieldID,string text);
    bool setSliderValue(string sliderID,float value);
    bool setMappedSliderValue(string sliderID,float value);
    void setMainAppHandle(void *mainAppHandle);
    bool setButtonText(string buttonID,string text);
    
    bool showBorder;
    unsigned int borderWidth;
    ofColor borderColor;
    
    string frameViewID;
    vector< ofUIButton > buttons;
    vector< ofUITextField > textFields;
    vector< ofUIRadioButton > radioButtons;
    vector< ofUISlider > sliders;
    vector< ofUIDropDownMenu > dropDownMenus;
    vector< ofUIImageButton > imageButtons;
    vector< ofUIStaticText > staticTexts;
    
};
