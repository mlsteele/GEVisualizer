#include "ofUIMainView.h"

ofUIMainView::ofUIMainView()
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
    objectType = "UI_MAIN_VIEW";
}

ofUIMainView::~ofUIMainView(){
    
}

void ofUIMainView::init(void *mainAppHandle,ofRectangle bounds,ofColor backgroundColor = ofColor(255,255,255),ofColor foregroundColor = ofColor(255,255,255)){
    this->mainAppHandle = mainAppHandle;
    this->bounds = bounds;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    childViews.clear();
}

bool ofUIMainView::update(){
    
    if( childViews.size() > 0 ){
        for(unsigned int i=0; i<childViews.size(); i++){
            childViews[i].update();
        }
    }
    
    return true;
}

bool ofUIMainView::draw(float parentX,float parentY){
    
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
        float currentLineWidth = render->getLineWidth();
        ofSetLineWidth( borderWidth );
        ofNoFill();
        ofSetColor(borderColor);
        ofRect(originX + bounds.x,originY + bounds.y,bounds.width,bounds.height);
        ofSetLineWidth( currentLineWidth );
    }
    
    if( childViews.size() > 0 ){
        for(unsigned int i=0; i<childViews.size(); i++){
            childViews[i].draw(originX + bounds.x,originY + bounds.y);
        }
    }
    
    return true;
}

void ofUIMainView::addSubView(ofUIFrameView frameView){
    
    //Set the parent of the frame and main app handle
    frameView.setMainAppHandle(mainAppHandle);
    
    //Package up the frameview in a view container and add it to the child views
    ofUIViewContainer container;
    container.set( frameView );
    
    childViews.push_back( container );
}

void ofUIMainView::addSubView(ofUIScrollView scrollView){
    //Set the parent of the frame and main app handle
    scrollView.setMainAppHandle(mainAppHandle);
    
    //Package up the frameview in a view container and add it to the child views
    ofUIViewContainer container;
    container.set( scrollView );
    
    childViews.push_back( container );
}

void ofUIMainView::addSubView(ofUITabView tabView){

}

void ofUIMainView::addSubView(ofUISubView subView){
    //Set the subview main app handle
    subView.setMainAppHandle(mainAppHandle);
    
    //Package up the subview in a view container and add it to the child views
    ofUIViewContainer container;
    container.set( subView );
    
    childViews.push_back( container );
}

void ofUIMainView::clearAllSubViews(){
    childViews.clear();
}

void ofUIMainView::mouseMovedEvent(float x,float y){
    for(unsigned int i=0; i<childViews.size(); i++){
        childViews[i].mouseMovedEvent(x, y);
    }
}

void ofUIMainView::mousePressedEvent(float x, float y){
    for(unsigned int i=0; i<childViews.size(); i++){
        childViews[i].mousePressedEvent(x, y);
    }
}

void ofUIMainView::mouseReleasedEvent(float x,float y){
    for(unsigned int i=0; i<childViews.size(); i++){
        childViews[i].mouseReleasedEvent(x, y);
    }
}

void ofUIMainView::mouseDraggedEvent(float x,float y){
    for(unsigned int i=0; i<childViews.size(); i++){
        childViews[i].mouseDraggedEvent(x, y);
    }
}

void ofUIMainView::keyPressedEvent(int key){
    for(unsigned int i=0; i<childViews.size(); i++){
        childViews[i].keyPressedEvent(key);
    }
}

float ofUIMainView::getScreenX(){
    return getX();
}

float ofUIMainView::getScreenY(){
    return getY();
}

bool ofUIMainView::getValueFromTextFieldAsFloat(string textFieldID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getValueFromTextFieldAsFloat(textFieldID,value) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::getValueFromTextFieldAsString(string textFieldID,string &text){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getValueFromTextFieldAsString(textFieldID,text) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::getValueFromSlider(string sliderID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getValueFromSlider(sliderID,value) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::getMappedValueFromSlider(string sliderID,float &value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getMappedValueFromSlider(sliderID,value) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::getStateOfRadioButton(string radioButtonID,bool &state){
    bool radioButtonFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getStateOfRadioButton(radioButtonID,state) ){
            radioButtonFound = true;
            break;
        }
    }
    return radioButtonFound;
}

bool ofUIMainView::getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem){
    bool dropDownMenuFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getCurrentItemOfDropDownMenu(dropDownMenuID,currentItem) ){
            dropDownMenuFound = true;
            break;
        }
    }
    return dropDownMenuFound;
}

bool ofUIMainView::getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows){
    bool scrollViewFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getNumberOfRowsInScrollView(scrollViewID,numRows) ){
            scrollViewFound = true;
            break;
        }
    }
    return scrollViewFound;
}

bool ofUIMainView::getSubViewVisibleState(string subViewID,bool &state){
    bool subViewFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].getSubViewVisibleState(subViewID,state) ){
            subViewFound = true;
            break;
        }
    }
    return subViewFound;
}

ofUISubView* ofUIMainView::getSubViewPointer(string subViewID){
    ofUISubView *subViewPtr = NULL;
    for(unsigned int i=0; i<childViews.size(); i++){
        subViewPtr = childViews[i].getSubViewPointer(subViewID);
        if( subViewPtr != NULL ){
            return subViewPtr;
        }
    }
    return subViewPtr;
}

ofUIScrollView* ofUIMainView::getScrollViewPointer(string scrollViewID){
    ofUIScrollView *scrollViewPtr = NULL;
    for(unsigned int i=0; i<childViews.size(); i++){
        scrollViewPtr = childViews[i].getScrollViewPointer(scrollViewID);
        if( scrollViewPtr != NULL ){
            return scrollViewPtr;
        }
    }
    return scrollViewPtr;
}

ofUIButton* ofUIMainView::getUIButtonPointer(string buttonID){
    ofUIButton *buttonPtr = NULL;
    for(unsigned int i=0; i<childViews.size(); i++){
        buttonPtr = childViews[i].getUIButtonPointer(buttonID);
        if( buttonPtr != NULL ){
            return buttonPtr;
        }
    }
    return buttonPtr;
}

bool ofUIMainView::setSubViewVisibleState(string subViewID,bool state){
    bool subViewFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setSubViewVisibleState(subViewID,state) ){
            subViewFound = true;
            break;
        }
    }
    return subViewFound;
}

bool ofUIMainView::setFrameViewVisibleState(string frameViewID,bool state){
    bool frameViewFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setFrameViewVisibleState(frameViewID,state) ){
            frameViewFound = true;
            break;
        }
    }
    return frameViewFound;
}

bool ofUIMainView::setTextFieldValue(string textFieldID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setTextFieldValue(textFieldID,value) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::setTextFieldValue(string textFieldID,string text){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setTextFieldValue(textFieldID,text) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::setSliderValue(string sliderID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setSliderValue(sliderID,value) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;

}

bool ofUIMainView::setMappedSliderValue(string sliderID,float value){
    bool textFieldFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setMappedSliderValue(sliderID,value) ){
            textFieldFound = true;
            break;
        }
    }
    return textFieldFound;
}

bool ofUIMainView::setButtonText(string buttonID,string text){
    bool buttonFound = false;
    for(unsigned int i=0; i<childViews.size(); i++){
        if( childViews[i].setButtonText(buttonID,text) ){
            buttonFound = true;
            break;
        }
    }
    return buttonFound;
}
