#pragma once
#include "ofUIView.h"
#include "ofUIViewContainer.h"

class ofUIMainView : public ofUIView
{
public:
	ofUIMainView();
	~ofUIMainView(void);

    //Setup functions
    void init(void *mainAppHandle,ofRectangle bounds,ofColor backgroundColor,ofColor foregroundColor);
    void addSubView(ofUIFrameView frameView);
    void addSubView(ofUIScrollView srollView);
    void addSubView(ofUITabView tabView);
    void addSubView(ofUISubView subView);
    
    //Clear functions
    void clearAllSubViews();
    
    //Main GUI Interface Methods
    bool update();
	bool draw(float parentX,float parentY);
    void mouseMovedEvent(float x,float y);
    void mousePressedEvent(float x,float y);
    void mouseReleasedEvent(float x,float y);
    void mouseDraggedEvent(float x,float y);
    void keyPressedEvent(int key);
    
    //Getters
    float getScreenX();
    float getScreenY();
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
    
    bool showBorder;
    unsigned int borderWidth;
    ofColor borderColor;
    vector < ofUIViewContainer > childViews;
};
