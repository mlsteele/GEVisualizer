#include "ofUIFrameView.h"

ofUIFrameView::ofUIFrameView()
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
    borderWidth = 1;
    borderColor.r = 255;
	borderColor.g = 255;
	borderColor.b = 255;
	borderColor.a = 255;
    objectType = "UI_FRAME_VIEW";
}

ofUIFrameView::~ofUIFrameView(){
    
}

void ofUIFrameView::init(string frameViewID,ofRectangle bounds,ofColor backgroundColor = ofColor(255,255,255),ofColor foregroundColor = ofColor(255,255,255)){
    this->frameViewID = frameViewID;
    this->bounds = bounds;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    buttons.clear();
    textFields.clear();
    radioButtons.clear();
    sliders.clear();
    dropDownMenus.clear();
    imageButtons.clear();
    staticTexts.clear();
}

bool ofUIFrameView::update(){
    
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
    return true;
}

bool ofUIFrameView::draw(float parentX,float parentY){
    
    if( !visible ){
        return true;
    }
    
    originX = parentX;
    originY = parentY;
    
    //Draw the view
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

void ofUIFrameView::addButton(ofUIButton button){
    buttons.push_back( button );
}

void ofUIFrameView::addTextField(ofUITextField textField){
    textFields.push_back( textField );
}

void ofUIFrameView::addRadioButton(ofUIRadioButton radioButton){
    radioButtons.push_back( radioButton );
}

void ofUIFrameView::addSlider(ofUISlider slider){
    sliders.push_back( slider );
}

void ofUIFrameView::addDropDownMenu(ofUIDropDownMenu dropDownMenu){
    dropDownMenus.push_back( dropDownMenu );
}

void ofUIFrameView::addImageButton(ofUIImageButton imageButton){
    imageButtons.push_back( imageButton );
}

void ofUIFrameView::addStaticText(ofUIStaticText staticText){
    staticTexts.push_back( staticText );
}

bool ofUIFrameView::mouseMovedEvent(float mouseX,float mouseY){
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
    
    return moveActioned;
}

bool ofUIFrameView::mousePressedEvent(float mouseX,float mouseY){
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
    
    return clickActioned;
}

bool ofUIFrameView::mouseReleasedEvent(float mouseX,float mouseY){
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
    
    return actioned;
    
}
bool ofUIFrameView::mouseDraggedEvent(float mouseX,float mouseY){
    
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
    
    return actioned;
}
    
bool ofUIFrameView::keyPressedEvent(int key){
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
    
    return keyActioned;
}

bool ofUIFrameView::getValueFromTextFieldAsFloat(string textFieldID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            value = textFields[i].getValueAsFloat();
            textFieldFound = true;
        }
    }
    
    return textFieldFound;
}

bool ofUIFrameView::getValueFromTextFieldAsString(string textFieldID,string &text){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            text = textFields[i].getValueAsText();
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIFrameView::getValueFromSlider(string sliderID,float &value){
    bool sliderFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            value = sliders[i].getValue();
            sliderFound = true;
        }
    }
    
    return sliderFound;
}

bool ofUIFrameView::getMappedValueFromSlider(string sliderID,float &value){
    bool sliderFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            value = sliders[i].getMappedValue();
            sliderFound = true;
        }
    }
    
    return sliderFound;
}

bool ofUIFrameView::getStateOfRadioButton(string radioButtonID,bool &state){
    bool radioButtonFound = false;
    
    for(unsigned int i=0; i<radioButtons.size(); i++){
        if( radioButtons[i].getStateOfRadioButton(radioButtonID,state) ){
            radioButtonFound = true;
        }
    }
    
    return radioButtonFound;
}

bool ofUIFrameView::getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem){
    bool dropDownMenuFound = false;
    
    for(unsigned int i=0; i<dropDownMenus.size(); i++){
        if( dropDownMenus[i].getDropDownMenuID() == dropDownMenuID ){
            currentItem = dropDownMenus[i].getCurrentItem(); 
            dropDownMenuFound = true;
        }
    }
    return dropDownMenuFound;
}

ofUIButton* ofUIFrameView::getUIButtonPointer(string buttonID){
    
    for(unsigned int i=0; i< buttons.size(); i++){
        if( buttons[i].getButtonID() == buttonID ){
            return &buttons[i];
        }
    }
    return NULL;
}

bool ofUIFrameView::setTextFieldValue(string textFieldID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            textFields[i].setTextFieldValue(value);
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIFrameView::setTextFieldValue(string textFieldID,string text){
    bool textFieldFound = false;
    for(unsigned int i=0; i<textFields.size(); i++){
        if( textFields[i].textFieldID == textFieldID ){
            textFields[i].setTextFieldValue(text);
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIFrameView::setSliderValue(string sliderID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            sliders[i].setValue(value);
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIFrameView::setMappedSliderValue(string sliderID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<sliders.size(); i++){
        if( sliders[i].sliderID == sliderID ){
            sliders[i].setMappedValue(value);
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

void ofUIFrameView::setMainAppHandle(void *mainAppHandle){ 
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
}

bool ofUIFrameView::setButtonText(string buttonID,string text){
    bool buttonFound = false;
    for(unsigned int i=0; i<buttons.size(); i++){ 
        if( buttons[i].setButtonText(buttonID,text) ){
            buttonFound = true;
        }
    }
    return buttonFound;
}
