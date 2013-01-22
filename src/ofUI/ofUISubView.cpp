#include "ofUISubView.h"

ofUISubView::ofUISubView()
{
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	backgroundColor.a = 255;
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	foregroundColor.a = 255;
    showBorder = true;
    drawBackground = true;
    borderWidth = 1;
    borderColor.r = 255;
	borderColor.g = 255;
	borderColor.b = 255;
	borderColor.a = 255;
    subViewID = "";
    objectType = "UI_SUB_VIEW";
}

ofUISubView::~ofUISubView(){
    
}

void ofUISubView::init(string subViewID,ofRectangle bounds,ofColor backgroundColor = ofColor(255,255,255),ofColor foregroundColor = ofColor(255,255,255),bool drawBackground){
    this->subViewID = subViewID;
    this->bounds = bounds;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    this->drawBackground = drawBackground;
    buttons.clear();
    textFields.clear();
    radioButtons.clear();
    sliders.clear();
    dropDownMenus.clear();
    imageButtons.clear();
    staticTexts.clear();
    frameViews.clear();
    scrollViews.clear();
}

bool ofUISubView::update(){
    
    //Update any buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        buttons[i].update();
    }
    
    //Update any text fileds
    for(unsigned int i=0; i<textFields.size(); i++){
        textFields[i].update();
    }
    
    //Update any text fileds
    for(unsigned int i=0; i<radioButtons.size(); i++){
        radioButtons[i].update();
    }
    
    //Update any sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        sliders[i].update();
    }
    
    //Update any image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        imageButtons[i].update();
    }
    
    //Update any popup menus (make sure this is last as it can grow in size on mouse over)
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        dropDownMenus[i].update();
    }
    
    //Update any static text
    for(unsigned int i=0; i<staticTexts.size(); i++){
        staticTexts[i].update();
    }
    
    //Update any static text
    for(unsigned int i=0; i<frameViews.size(); i++){
        frameViews[i].update();
    }
    
    //Update any static text
    for(unsigned int i=0; i<scrollViews.size(); i++){
        scrollViews[i].update();
    }
    return true;
}

bool ofUISubView::draw(float parentX,float parentY){
    
    if( !visible ){
        return true;
    }
    
    originX = parentX;
    originY = parentY;
    
    //Draw the view
    if( drawBackground ){ 
        ofFill();
        ofSetColor(backgroundColor);
        ofRect(originX + bounds.x,originY + bounds.y,bounds.width,bounds.height);
        
        if( showBorder ){
            ofPtr<ofGLRenderer> render = ofGetGLRenderer();
            // float currentLineWidth = render->getLineWidth();
            // hard default -> line width
            float currentLineWidth = 2;
            ofSetLineWidth( borderWidth );
            ofNoFill();
            ofSetColor(borderColor);
            ofRect(originX + bounds.x,originY + bounds.y,bounds.width,bounds.height);
            ofSetLineWidth( currentLineWidth );
        }
    }
    
    //Draw any scroll views
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].isVisible() ){
            scrollViews[i].draw(originX + bounds.x, originY + bounds.y);
        }
    }
    
    //Draw any frame views
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].isVisible() ){
            frameViews[i].draw(originX + bounds.x, originY + bounds.y);
        }
    }
    
    //Draw any buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].isVisible() ){
            buttons[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    //Draw any text fileds
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].isVisible() ){
            textFields[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    //Draw any text fileds
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].isVisible()  ){
            radioButtons[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    //Draw any sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].isVisible()  ){
            sliders[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    //Draw any image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        if( imageButtons[i].isVisible()  ){
            imageButtons[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    //Draw any popup menus (make sure this is last as it can grow in size on mouse over)
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].isVisible()  ){
            dropDownMenus[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    //Draw any static text
    for(unsigned int i=0; i<staticTexts.size(); i++){
        if( staticTexts[i].isVisible() ){
            staticTexts[i].draw(originX + bounds.x, originY + bounds.y);
        }
    }
    
    return true;
}

void ofUISubView::addButton(ofUIButton button){
    buttons.push_back( button );
}

void ofUISubView::addTextField(ofUITextField textField){
    textFields.push_back( textField );
}

void ofUISubView::addRadioButton(ofUIRadioButton radioButton){
    radioButtons.push_back( radioButton );
}

void ofUISubView::addSlider(ofUISlider slider){
    sliders.push_back( slider );
}

void ofUISubView::addDropDownMenu(ofUIDropDownMenu dropDownMenu){
    dropDownMenus.push_back( dropDownMenu );
}

void ofUISubView::addImageButton(ofUIImageButton imageButton){
    imageButtons.push_back( imageButton );
}

void ofUISubView::addStaticText(ofUIStaticText staticText){
    staticTexts.push_back( staticText );
}

void ofUISubView::addFrameView(ofUIFrameView frameView){
    frameViews.push_back( frameView );
}


void ofUISubView::addScrollView(ofUIScrollView scrollView){
    scrollViews.push_back( scrollView );
}


bool ofUISubView::mouseMovedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    
    bool moveActioned = false;
    
    //Check the buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check the text fields
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check the radio buttons
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check the sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check the popup menus
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check any image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        if( imageButtons[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check the frame views
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    //Check the scroll views
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    
    return moveActioned;
}

bool ofUISubView::mousePressedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    
    bool clickActioned = false;
    
    //Check the buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the text fields
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }//Check to make sure none of the text fields are still selected
        else if( textFields[i].isSelected() ){
            textFields[i].setSelected(false);
        }
    }
    
    //Check the radio buttons
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the popup menus
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        if( imageButtons[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the frame views
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the scroll views
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    return clickActioned;
}

bool ofUISubView::mouseReleasedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    
    bool actioned = false;
    
    //Check the buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the text fields
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the radio buttons
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the popup menus
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
        
    //Check the image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        if( imageButtons[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the frame views
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the scroll views
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    return actioned;
    
}
bool ofUISubView::mouseDraggedEvent(float mouseX,float mouseY){
    
    if( !visible || !enabled ){
        return false;
    }
    
    bool actioned = false;
    
    //Check the buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the text fields
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the radio buttons
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the popup menus
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        if( imageButtons[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the frame views
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    //Check the scroll views
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    
    return actioned;
}
    
bool ofUISubView::keyPressedEvent(int key){
    if( !visible || !enabled ){
        return false;
    }
    
    //Check the text fields
    bool keyActioned = false;
    
    //Check the buttons
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the text fields
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the radio buttons
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the sliders
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the popup menus
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the image buttons
    for(unsigned int i=0; i<imageButtons.size(); i++){
        if( imageButtons[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the frame views
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    //Check the scroll views
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    
    return keyActioned;
}

bool ofUISubView::getValueFromTextFieldAsFloat(string textFieldID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            value = textFields[i].getValueAsFloat();
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].getValueFromTextFieldAsFloat(textFieldID,value) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getValueFromTextFieldAsFloat(textFieldID,value) ){
            textFieldFound = true;
        }
    }
    
    return textFieldFound;
}

bool ofUISubView::getValueFromTextFieldAsString(string textFieldID,string &text){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            text = textFields[i].getValueAsText();
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].getValueFromTextFieldAsString(textFieldID,text) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getValueFromTextFieldAsString(textFieldID,text) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUISubView::getValueFromSlider(string sliderID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            value = sliders[i].getValue();
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].getValueFromSlider(sliderID,value) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getValueFromSlider(sliderID,value) ){
            textFieldFound = true;
        }
    }
    
    return textFieldFound;
}

bool ofUISubView::getMappedValueFromSlider(string sliderID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            value = sliders[i].getMappedValue();
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].getMappedValueFromSlider(sliderID,value) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getMappedValueFromSlider(sliderID,value) ){
            textFieldFound = true;
        }
    }
    
    return textFieldFound;
}

bool ofUISubView::getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows){
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getScrollViewID() == scrollViewID ){
            numRows = scrollViews[i].getNumRows();
            return true;
        }
    }
    return false;
}

bool ofUISubView::getStateOfRadioButton(string radioButtonID,bool &state){
    bool radioButtonFound = false;
    
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].getRadioButtonID() == radioButtonID ){
            state = radioButtons[i].getState();
            radioButtonFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].getStateOfRadioButton(radioButtonID,state) ){
            radioButtonFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getStateOfRadioButton(radioButtonID,state) ){
            radioButtonFound = true;
        }
    }
    
    return radioButtonFound;
}

bool ofUISubView::getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem){
    bool dropDownMenuFound = false;
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getCurrentItemOfDropDownMenu(dropDownMenuID,currentItem) ){
            dropDownMenuFound = true;
        }
    }
    return dropDownMenuFound;
}

bool ofUISubView::getSubViewVisibleState(string subViewID,bool &state){
    if( this->subViewID == subViewID ){
        state = isVisible();
        return true;
    }
    return false;
}

ofUIScrollView* ofUISubView::getScrollViewPointer(string scrollViewID){
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getScrollViewID() == scrollViewID ){
            return &scrollViews[i];
        }
    }
    return NULL;
}

ofUIButton* ofUISubView::getUIButtonPointer(string buttonID){
    for(unsigned int i=0; i<buttons.size(); i++){
        if( buttons[i].getButtonID() == buttonID ){
            return &buttons[i];
        }
    }
    return NULL;
}

bool ofUISubView::setTextFieldValue(string textFieldID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            textFields[i].setTextFieldValue(value);
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].setTextFieldValue(textFieldID,value) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].setTextFieldValue(textFieldID,value) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUISubView::setTextFieldValue(string textFieldID,string text){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            textFields[i].setTextFieldValue(text);
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].setTextFieldValue(textFieldID,text) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].setTextFieldValue(textFieldID,text) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUISubView::setSliderValue(string sliderID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            sliders[i].setValue(value);
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].setSliderValue(sliderID,value) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].setSliderValue(sliderID,value) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUISubView::setMappedSliderValue(string sliderID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            sliders[i].setMappedValue(value);
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].setMappedSliderValue(sliderID,value) ){
            textFieldFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].setMappedSliderValue(sliderID,value) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

void ofUISubView::setMainAppHandle(void *mainAppHandle){ 
    this->mainAppHandle = mainAppHandle; 
    
    //Set the main app handle for all the buttons, text fields, radio buttons, etc.
    for(unsigned int i=0; i<buttons.size(); i++){ 
        buttons[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<textFields.size(); i++){
        textFields[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<radioButtons.size(); i++){
        radioButtons[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<sliders.size(); i++){
        sliders[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        dropDownMenus[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<imageButtons.size(); i++){
        imageButtons[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        frameViews[i].setMainAppHandle(mainAppHandle);
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        scrollViews[i].setMainAppHandle(mainAppHandle);
    }
}

bool ofUISubView::addRowToScrollView(string scrollViewID,ofUIFrameView newRow){
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].getScrollViewID() == scrollViewID ){
            scrollViews[i].addRow( newRow );
            return true;
        }
    }
    return false;
}

bool ofUISubView::setButtonText(string buttonID,string text){
    bool buttonFound = false;
    for(unsigned int i=0; i<buttons.size(); i++){ 
        if( buttons[i].setButtonText(buttonID,text) ){
            buttonFound = true;
        }
    }
    for(unsigned int i=0; i<frameViews.size(); i++){
        if( frameViews[i].setButtonText(buttonID,text) ){
            buttonFound = true;
        }
    }
    for(unsigned int i=0; i<scrollViews.size(); i++){
        if( scrollViews[i].setButtonText(buttonID,text) ){
            buttonFound = true;
        }
    }
    return buttonFound;
}