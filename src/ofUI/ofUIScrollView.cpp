#include "ofUIScrollView.h"

ofUIScrollView::ofUIScrollView()
{
    scrollViewID = "";
    rowHeight = 0;
    currentTopViewRowIndex = 0;
    scrollVisible = false;
    drawTableHeader = false;
    showBorder = true;
    borderWidth = 1;
    borderColor.r = 255;
	borderColor.g = 255;
	borderColor.b = 255;
	borderColor.a = 255;
    objectType = "UI_SCROLL_VIEW";
    
    sliderButton.init("",ofRectangle(0,0,1,1),"",ofColor(0, 0, 0),ofColor(0, 0, 0),ofColor(0, 0, 0),false);
}

ofUIScrollView::~ofUIScrollView(){
    
}

void ofUIScrollView::init(string scrollViewID,ofRectangle bounds,float rowHeight,ofColor backgroundColor,ofColor foregroundColor,ofColor selectedColor,vector< ofColor > rowBackgroundColors){
    this->scrollViewID = scrollViewID;
    this->bounds = bounds;
    this->rowHeight = rowHeight;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    this->selectedColor = selectedColor;
    this->rowBackgroundColors = rowBackgroundColors;
    rowFrameData.clear();
    scrollVisible = false;
    currentTopViewRowIndex = 0;
}

bool ofUIScrollView::update(){
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        rowFrameData[i].update();
    }
}

bool ofUIScrollView::draw(float parentX,float parentY){
    
    if( !visible ){
        return true;
    }
    
    originX = parentX;
    originY = parentY;
    float xPos = 0;
    float yPos = 0;
    
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
    
    float viewHeight = bounds.height;
    
    if( drawTableHeader ){
        xPos = 0;
        yPos = 0;
        ofRectangle headerBounds(xPos,yPos,tableHeader.getWidth(),tableHeader.getHeight());
        tableHeader.setBounds( headerBounds );
        tableHeader.draw(originX + bounds.x,originY + bounds.y);
        yPos += rowHeight;
        viewHeight -= tableHeader.getHeight();
    }
    
    //Draw the scroll contents
    //Work out if the number of rows in the rowFrameData buffer will take up more space than we have to draw it
    if( rowFrameData.size() * rowHeight > viewHeight){
        scrollVisible = true;
    }else scrollVisible = false;
    
    //Work out which row index we should stop drawing at
    unsigned int maxViewRowIndex = currentTopViewRowIndex + (unsigned int)floor(viewHeight/rowHeight);
    if( maxViewRowIndex > rowFrameData.size() ){
        maxViewRowIndex = rowFrameData.size();
    }
    
    for(unsigned int i=currentTopViewRowIndex; i<maxViewRowIndex; i++){
        unsigned int k = i;//maxViewRowIndex-1-i;
        //Draw the contents of the row
        ofRectangle rowBounds(xPos,yPos,rowFrameData[k].getWidth(),rowFrameData[k].getHeight());
        rowFrameData[k].backgroundColor = rowBackgroundColors[ k % rowBackgroundColors.size() ];
        rowFrameData[k].setBounds( rowBounds );
        rowFrameData[k].draw(originX + bounds.x,originY + bounds.y);
        yPos += rowHeight;
    }
    
    if( scrollVisible ){
        //Draw the slider
        float sliderBorderX = originX + bounds.x + bounds.width - 20;
        float sliderBorderY = originY + bounds.y;
        float sliderBorderW = 20;
        float sliderBorderH = bounds.height;
        float sliderTipH = 20;
        
        if( drawTableHeader ){
            sliderBorderY += tableHeader.getHeight(); 
            sliderBorderH -= tableHeader.getHeight(); 
        }
        
        ofRectangle scrollBounds = sliderButton.getBounds();
        //If this is the first draw loop then set the size of the scroll slider button
        if( scrollBounds.width == 1 && scrollBounds.height == 1 ){
            scrollBounds.x = sliderBorderX + 1;
            scrollBounds.y = sliderBorderY + sliderTipH + 1;
            scrollBounds.width = sliderBorderW - 2;
            scrollBounds.height = 50;
            sliderButton.setBounds( scrollBounds );
        }
        
        //Draw the slider background
        ofFill();
        ofSetColor(backgroundColor);
        ofRect(sliderBorderX,sliderBorderY,sliderBorderW,sliderBorderH);
        
        //Draw the slider header and footer
        ofFill();
        ofSetColor(foregroundColor);
        ofRect(sliderBorderX,sliderBorderY,sliderBorderW,sliderTipH);
        ofRect(sliderBorderX,sliderBorderY+sliderBorderH-sliderTipH,sliderBorderW,sliderTipH);
        
        //Call the slider button draw function does not really draw anything (as it's invisible) but it does set the buttons origins
        sliderButton.draw(0,0);
        
        //Draw the slider button
        ofFill();
        if( sliderButton.isButtonRollOverState() || sliderButton.isButtonDownState() ) ofSetColor(selectedColor);
        else ofSetColor(foregroundColor);
        ofRect(scrollBounds.x,scrollBounds.y,scrollBounds.width,scrollBounds.height);
        
    }
    
    return true;
}

bool ofUIScrollView::addRow( ofUIFrameView newRow, int rowIndex ){
    
    //If the row index is -1 then we insert this new row in at the end
    if( rowIndex == -1 ) rowIndex = rowFrameData.size();
    if( rowIndex > rowFrameData.size() ) return false;
    
    //Set the main app handle for the new row
    newRow.setMainAppHandle(mainAppHandle);
    
    //Add the new row
    vector< ofUIFrameView >::iterator insertPos = rowFrameData.begin() + rowIndex;
    rowFrameData.insert( insertPos , newRow );
    
    return true;
}

bool ofUIScrollView::mouseMovedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    bool moveActioned = false;
    
    //Check the scroll slider button
    if( scrollVisible ){
        if( sliderButton.isButtonDownState() ){
            moveActioned = true;
        }
    }
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].mouseMovedEvent(mouseX,mouseY) ){
            moveActioned = true;
        }
    }
    return moveActioned;
}

bool ofUIScrollView::mousePressedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    bool clickActioned = false;
    
    //Check the scroll slider button
    if( scrollVisible ){
        if( sliderButton.mousePressedEvent(mouseX, mouseY) ){
            clickActioned = true;
        }
    }
    
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].mousePressedEvent(mouseX,mouseY) ){
            clickActioned = true;
        }
    }
    
    return clickActioned;
}

bool ofUIScrollView::mouseReleasedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    
    bool actioned = false;
    
    //Check the scroll slider button
    if( scrollVisible ){
        if( sliderButton.mouseReleasedEvent(mouseX, mouseY) ){
            actioned = true;
        }
    }
    
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].mouseReleasedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    return actioned;
}

bool ofUIScrollView::mouseDraggedEvent(float mouseX,float mouseY){
    if( !visible || !enabled ){
        return false;
    }
    
    bool actioned = false;
    
    //Check the scroll slider button
    if( scrollVisible ){
        if( sliderButton.mouseDraggedEvent(mouseX, mouseY) ){
            actioned = true;
            
            //Work out which row should be at the top of the scroll view
            float sliderBorderY = originY + bounds.y;
            float sliderBorderH = bounds.height;
            float sliderTipH = 20;
            
            ofRectangle scrollBounds = sliderButton.getBounds();
            unsigned int numRowsInView = (unsigned int)floor(bounds.height/rowHeight);
            unsigned int N = (getNumRows()-1) < numRowsInView ? getNumRows()-1 : getNumRows()- 1 - 5;
            
            if( drawTableHeader ){
                sliderBorderY += tableHeader.getHeight(); 
                sliderBorderH -= tableHeader.getHeight(); 
                N -= 1;
            }
            
            float sliderPos = ofMap(mouseY-sliderBorderY,sliderTipH,sliderBorderH-sliderTipH,0,1.0,true);
            float index = ofMap(sliderPos,0,1.0,0,N,true);
            
            //Update the currentTopViewRowIndex
            currentTopViewRowIndex = (unsigned int)index;
            
            //Update the slider button
            scrollBounds.y = sliderBorderY + ofMap(sliderPos,0,1.0,1,sliderBorderH-scrollBounds.height,true);
            
            //Constrain the slider button to account for the slider tips
            if( scrollBounds.y < sliderBorderY + sliderTipH + 1 ){
                scrollBounds.y = sliderBorderY + sliderTipH + 1;
            }else if( scrollBounds.y + scrollBounds.height > sliderBorderY + sliderBorderH - sliderTipH + 1 ){
                scrollBounds.y = sliderBorderY + sliderBorderH - sliderTipH - 1 - scrollBounds.height;
            }
            sliderButton.setBounds( scrollBounds );
        }
    }
    
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].mouseDraggedEvent(mouseX,mouseY) ){
            actioned = true;
        }
    }
    return actioned;
}

bool ofUIScrollView::keyPressedEvent(int key){
    if( !visible || !enabled ){
        return false;
    }
    
    //Check the text fields
    bool keyActioned = false;
    
    //Check the buttons
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].keyPressedEvent(key) ){
            keyActioned = true;
        }
    }
    return keyActioned;
}

bool ofUIScrollView::getValueFromTextFieldAsFloat(string textFieldID,float &value){
    bool textFieldFound = false;

    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].getValueFromTextFieldAsFloat(textFieldID,value) ){
            textFieldFound = true;
        }
    }
    
    return textFieldFound;
}

bool ofUIScrollView::getValueFromTextFieldAsString(string textFieldID,string &text){
    bool textFieldFound = false;
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].getValueFromTextFieldAsString(textFieldID,text) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIScrollView::getValueFromSlider(string sliderID,float &value){
    bool sliderFound = false;
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].getValueFromSlider(sliderID,value) ){
            sliderFound = true;
        }
    }
    
    return sliderFound;
}

bool ofUIScrollView::getMappedValueFromSlider(string sliderID,float &value){
    bool sliderFound = false;

    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].getMappedValueFromSlider(sliderID,value) ){
            sliderFound = true;
        }
    }
    
    return sliderFound;
}

bool ofUIScrollView::getStateOfRadioButton(string radioButtonID,bool &state){
    bool radioButtonFound = false;
    
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].getStateOfRadioButton(radioButtonID,state) ){
            radioButtonFound = true;
        }
    }
    
    return radioButtonFound;
}

bool ofUIScrollView::getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem){
    bool dropDownMenuFound = false;
    
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].getCurrentItemOfDropDownMenu(dropDownMenuID,currentItem) ){
            dropDownMenuFound = true;
        }
    }
    return dropDownMenuFound;
}

bool ofUIScrollView::getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows){
    if( this->scrollViewID == scrollViewID ){
        numRows = getNumRows();
        return true;
    }
    return false;
}

ofUIScrollView* ofUIScrollView::getScrollViewPointer(string scrollViewID){

    ofUIScrollView *scrollViewPtr = NULL;
    
    //Check this scroll view
    if( this->scrollViewID == scrollViewID ){
        scrollViewPtr = this;
        return scrollViewPtr;
    }
    
    return scrollViewPtr;
}

void ofUIScrollView::setMainAppHandle(void *mainAppHandle){ 
    this->mainAppHandle = mainAppHandle; 
    
    //Set the main app handle for all the buttons, text fields, radio buttons, etc.
    for(unsigned int i=0; i<rowFrameData.size(); i++){ 
        rowFrameData[i].setMainAppHandle(mainAppHandle);
    }
    
}

bool ofUIScrollView::setTableHeader( ofUIFrameView tableHeader ){
    this->tableHeader = tableHeader;
    drawTableHeader = true;
}

bool ofUIScrollView::setTextFieldValue(string textFieldID,float value){
    bool textFieldFound = false;

    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].setTextFieldValue(textFieldID,value) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIScrollView::setTextFieldValue(string textFieldID,string text){
    bool textFieldFound = false;
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].setTextFieldValue(textFieldID,text) ){
            textFieldFound = true;
        }
    }
    return textFieldFound;
}

bool ofUIScrollView::setSliderValue(string sliderID,float value){
    bool sliderFound = false;
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].setSliderValue(sliderID,value) ){
            sliderFound = true;
        }
    }
    return sliderFound;
}

bool ofUIScrollView::setMappedSliderValue(string sliderID,float value){
    bool sliderFound = false;
    //Check the frame data
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].setMappedSliderValue(sliderID,value) ){
            sliderFound = true;
        }
    }
    return sliderFound;
}

bool ofUIScrollView::setButtonText(string buttonID,string text){
    bool buttonFound = false;
    for(unsigned int i=0; i<rowFrameData.size(); i++){
        if( rowFrameData[i].setButtonText(buttonID,text) ){
            buttonFound = true;        
        }
    }
    return buttonFound;
}