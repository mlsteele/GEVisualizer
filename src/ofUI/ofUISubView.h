#pragma once

#include "ofUIView.h"
#include "ofUIFrameView.h"
#include "ofUIScrollView.h"

class ofUISubView : public ofUIView
{
public:
	ofUISubView();
	~ofUISubView(void);

    //Setup functions
    void init(string subViewID,ofRectangle bounds,ofColor backgroundColor,ofColor foregroundColor,bool drawBackground = true);
    void addButton(ofUIButton button);
    void addTextField(ofUITextField textField);
    void addRadioButton(ofUIRadioButton radioButton);
    void addSlider(ofUISlider slider);
    void addDropDownMenu(ofUIDropDownMenu dropDownMenu);
    void addImageButton(ofUIImageButton imageButton);
    void addStaticText(ofUIStaticText staticText);
    void addFrameView(ofUIFrameView frameView);
    void addScrollView(ofUIScrollView scrollView);
    bool addRowToScrollView(string scrollViewID,ofUIFrameView newRow);
    
    //Main GUI Interface Methods
    bool update();
	bool draw(float parentX,float parentY);
    bool mouseMovedEvent(float mouseX,float mouseY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    bool mouseDraggedEvent(float mouseX,float mouseY);
    bool keyPressedEvent(int key);
    
    //Getters
    string getSubViewID(){ return subViewID; }
    bool getValueFromTextFieldAsFloat(string textFieldID,float &value);
    bool getValueFromTextFieldAsString(string textFieldID,string &text);
    bool getValueFromSlider(string sliderID,float &value);
    bool getMappedValueFromSlider(string sliderID,float &value);
    bool getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows);
    bool getStateOfRadioButton(string radioButtonID,bool &state);
    bool getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem);
    bool getSubViewVisibleState(string subViewID,bool &state);
    ofUIScrollView* getScrollViewPointer(string scrollViewID);
    ofUIButton* getUIButtonPointer(string buttonID);

    //Setters
    bool setTextFieldValue(string textFieldID,float value);
    bool setTextFieldValue(string textFieldID,string text);
    bool setSliderValue(string sliderID,float value);
    bool setMappedSliderValue(string sliderID,float value);
    void setMainAppHandle(void *mainAppHandle);
    bool setButtonText(string buttonID,string text);
    
    bool showBorder;
    bool drawBackground;
    unsigned int borderWidth;
    ofColor borderColor;
    
    string subViewID;
    vector< ofUIButton > buttons;
    vector< ofUITextField > textFields;
    vector< ofUIRadioButton > radioButtons;
    vector< ofUISlider > sliders;
    vector< ofUIDropDownMenu > dropDownMenus;
    vector< ofUIImageButton > imageButtons;
    vector< ofUIStaticText > staticTexts;
    vector< ofUIFrameView > frameViews;
    vector< ofUIScrollView > scrollViews;
    
};
