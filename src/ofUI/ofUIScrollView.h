#pragma once

#include "ofUIFrameView.h"

class ofUIScrollView : public ofUIView
{
public:
	ofUIScrollView();
	~ofUIScrollView(void);

    //Setup functions
    void init(string scrollViewID,ofRectangle bounds,float rowHeight, ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor, vector< ofColor > rowBackgroundColors);
    
    //Main GUI Interface Methods
    bool update();
	bool draw(float parentX,float parentY);
    bool addRow( ofUIFrameView newRow, int rowIndex = -1 );
    bool mouseMovedEvent(float mouseX,float mouseY);
    bool mousePressedEvent(float mouseX,float mouseY);
    bool mouseReleasedEvent(float mouseX,float mouseY);
    bool mouseDraggedEvent(float mouseX,float mouseY);
    bool keyPressedEvent(int key);

    //Getters
    string getScrollViewID(){ return scrollViewID; }
    unsigned int getNumRows(){ return rowFrameData.size(); }
    bool getValueFromTextFieldAsFloat(string textFieldID,float &value);
    bool getValueFromTextFieldAsString(string textFieldID,string &text);
    bool getValueFromSlider(string sliderID,float &value);
    bool getMappedValueFromSlider(string sliderID,float &value);
    bool getStateOfRadioButton(string radioButtonID,bool &state);
    bool getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem);
    bool getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows);
    ofUIScrollView* getScrollViewPointer(string scrollViewID);
    
    //Setters
    void setMainAppHandle(void *mainAppHandle);
    bool setTableHeader( ofUIFrameView tableHeader );
    bool setTextFieldValue(string textFieldID,float value);
    bool setTextFieldValue(string textFieldID,string text);
    bool setSliderValue(string sliderID,float value);
    bool setMappedSliderValue(string sliderID,float value);
    bool setButtonText(string buttonID,string text);
    
    bool showBorder;
    unsigned int borderWidth;
    ofColor borderColor;
    string scrollViewID;
    float rowHeight;
    ofUIButton sliderButton;
    unsigned int currentTopViewRowIndex;
    ofUIFrameView tableHeader;
    vector< ofUIFrameView > rowFrameData;
    bool scrollVisible;
    bool drawTableHeader;
    ofColor selectedColor;
    vector< ofColor > rowBackgroundColors;
    
};
