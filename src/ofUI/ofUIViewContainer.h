#pragma once
#include "ofUISubView.h"
#include "ofUIFrameView.h"
#include "ofUIScrollView.h"
#include "ofUITabView.h"

class ofUIViewContainer
{
public:
    ofUIViewContainer();
    ~ofUIViewContainer();
    
    void set(ofUISubView view);
    void set(ofUIFrameView view);
    void set(ofUIScrollView view);
    void set(ofUITabView view);
    bool update();
	bool draw(float x,float y);
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
    bool getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows);
    bool getSubViewVisibleState(string subViewID,bool &state);
    ofUISubView* getSubViewPointer(string subViewID);
    ofUIScrollView* getScrollViewPointer(string scrollViewID);
    ofUIButton* getUIButtonPointer(string buttonID);
    
    //Setters
    bool setSubViewVisibleState(string subViewID,bool state);
    bool setFrameViewVisibleState(string frameViewID,bool state);
    bool setTextFieldValue(string textFieldID,float value);
    bool setTextFieldValue(string textFieldID,string text);
    bool setSliderValue(string sliderID,float value);
    bool setMappedSliderValue(string sliderID,float value);
    bool setButtonText(string buttonID,string text);
    
    unsigned int viewType;
    ofUISubView subView;
    ofUIFrameView frameView;
    ofUIScrollView scrollView;
    ofUITabView tabView;
    
    enum ViewTypes{NOT_SET=0,SUB_VIEW,FRAME_VIEW,SCROLL_VIEW,TAB_VIEW};
};
