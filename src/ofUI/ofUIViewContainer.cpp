#include "ofUIViewContainer.h"

ofUIViewContainer::ofUIViewContainer(){
    viewType = NOT_SET;
}

ofUIViewContainer::~ofUIViewContainer(void){
	
}

void ofUIViewContainer::set(ofUISubView view){
    subView = view;
    viewType = SUB_VIEW;
}

void ofUIViewContainer::set(ofUIFrameView view){
    frameView = view;
    viewType = FRAME_VIEW;
}

void ofUIViewContainer::set(ofUIScrollView view){
    scrollView = view;
    viewType = SCROLL_VIEW;
}

void ofUIViewContainer::set(ofUITabView view){
    tabView = view;
    viewType = TAB_VIEW;
}

bool ofUIViewContainer::update(){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.update();
            break;
        case FRAME_VIEW:
            return frameView.update();
            break;
        case SCROLL_VIEW:
            return scrollView.update();
            break;
        case TAB_VIEW:
            return tabView.update();
            break;
        default:
            printf("ERROR: ofUIViewContainer::update(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::draw(float x,float y){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.draw(x,y);
            break;
        case FRAME_VIEW:
            return frameView.draw(x,y);
            break;
        case SCROLL_VIEW:
            return scrollView.draw(x,y);
            break;
        case TAB_VIEW:
            return tabView.draw(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::draw(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::mouseMovedEvent(float mouseX,float mouseY){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.mouseMovedEvent(mouseX,mouseY);
            break;
        case FRAME_VIEW:
            return frameView.mouseMovedEvent(mouseX,mouseY);
            break;
        case SCROLL_VIEW:
            return scrollView.mouseMovedEvent(mouseX,mouseY);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::checkMouseClickEvent(). Unkown View Type\n");
            return false;
            break;
    }

}

bool ofUIViewContainer::mousePressedEvent(float mouseX,float mouseY){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.mousePressedEvent(mouseX,mouseY);
            break;
        case FRAME_VIEW:
            return frameView.mousePressedEvent(mouseX,mouseY);
            break;
        case SCROLL_VIEW:
            return scrollView.mousePressedEvent(mouseX,mouseY);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::checkMouseClickEvent(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::mouseReleasedEvent(float mouseX,float mouseY){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.mouseReleasedEvent(mouseX,mouseY);
            break;
        case FRAME_VIEW:
            return frameView.mouseReleasedEvent(mouseX,mouseY);
            break;
        case SCROLL_VIEW:
            return scrollView.mouseReleasedEvent(mouseX,mouseY);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::checkMouseClickEvent(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::mouseDraggedEvent(float mouseX,float mouseY){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.mouseDraggedEvent(mouseX,mouseY);
            break;
        case FRAME_VIEW:
            return frameView.mouseDraggedEvent(mouseX,mouseY);
            break;
        case SCROLL_VIEW:
            return scrollView.mouseDraggedEvent(mouseX,mouseY);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::checkMouseClickEvent(). Unkown View Type\n");
            return false;
            break;
    }
    
}

bool ofUIViewContainer::keyPressedEvent(int key){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.keyPressedEvent(key);
            break;
        case FRAME_VIEW:
            return frameView.keyPressedEvent(key);
            break;
        case SCROLL_VIEW:
            return scrollView.keyPressedEvent(key);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getValueFromTextFieldAsFloat(string textFieldID,float &value){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getValueFromTextFieldAsFloat(textFieldID,value);
            break;
        case FRAME_VIEW:
            return frameView.getValueFromTextFieldAsFloat(textFieldID,value);
            break;
        case SCROLL_VIEW:
            return scrollView.getValueFromTextFieldAsFloat(textFieldID,value);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getValueFromTextFieldAsString(string textFieldID,string &text){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getValueFromTextFieldAsString(textFieldID,text);
            break;
        case FRAME_VIEW:
            return frameView.getValueFromTextFieldAsString(textFieldID,text);
            break;
        case SCROLL_VIEW:
            return scrollView.getValueFromTextFieldAsString(textFieldID,text);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getValueFromSlider(string sliderID,float &value){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getValueFromSlider(sliderID,value);
            break;
        case FRAME_VIEW:
            return frameView.getValueFromSlider(sliderID,value);
            break;
        case SCROLL_VIEW:
            return scrollView.getValueFromSlider(sliderID,value);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getMappedValueFromSlider(string sliderID,float &value){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getMappedValueFromSlider(sliderID,value);
            break;
        case FRAME_VIEW:
            return frameView.getMappedValueFromSlider(sliderID,value);
            break;
        case SCROLL_VIEW:
            return scrollView.getMappedValueFromSlider(sliderID,value);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getStateOfRadioButton(string radioButtonID,bool &state){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getStateOfRadioButton(radioButtonID,state);
            break;
        case FRAME_VIEW:
            return frameView.getStateOfRadioButton(radioButtonID,state);
            break;
        case SCROLL_VIEW:
            return scrollView.getStateOfRadioButton(radioButtonID,state);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getCurrentItemOfDropDownMenu(string dropDownMenuID,unsigned int &currentItem){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getCurrentItemOfDropDownMenu(dropDownMenuID,currentItem);
            break;
        case FRAME_VIEW:
            return frameView.getCurrentItemOfDropDownMenu(dropDownMenuID,currentItem);
            break;
        case SCROLL_VIEW:
            return scrollView.getCurrentItemOfDropDownMenu(dropDownMenuID,currentItem);
            break;
        case TAB_VIEW:
            return false; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getNumberOfRowsInScrollView(string scrollViewID,unsigned int &numRows){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getNumberOfRowsInScrollView(scrollViewID,numRows);
            break;
        case FRAME_VIEW:
            return false;
            break;
        case SCROLL_VIEW:
            return scrollView.getNumberOfRowsInScrollView(scrollViewID,numRows);
            break;
        case TAB_VIEW:
            return false;
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::getSubViewVisibleState(string subViewID,bool &state){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.getSubViewVisibleState(subViewID,state);
            break;
        case FRAME_VIEW:
            return false;
            break;
        case SCROLL_VIEW:
            return false;
            break;
        case TAB_VIEW:
            return false;
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return false;
            break;
    }

}

ofUISubView* ofUIViewContainer::getSubViewPointer(string subViewID){
    ofUISubView *subViewPtr = NULL;
    switch(viewType){
        case SUB_VIEW:
            if( subView.getSubViewID() == subViewID ){
                subViewPtr = &subView;
                return subViewPtr;
            }
            return subViewPtr;
            break;
        default:
            return subViewPtr;
            break;
    }
    
}

ofUIScrollView* ofUIViewContainer::getScrollViewPointer(string scrollViewID){
    switch(viewType){
        case NOT_SET:
            return NULL;
            break;
        case SUB_VIEW:
            return subView.getScrollViewPointer(scrollViewID);
            break;
        case FRAME_VIEW:
            return NULL;
            break;
        case SCROLL_VIEW:
            return scrollView.getScrollViewPointer(scrollViewID);
            break;
        case TAB_VIEW:
            return NULL; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::handleKeyPress(). Unkown View Type\n");
            return NULL;
            break;
    }
    return NULL;
}

ofUIButton* ofUIViewContainer::getUIButtonPointer(string buttonID){
    switch(viewType){
        case NOT_SET:
            return NULL;
            break;
        case SUB_VIEW:
            return subView.getUIButtonPointer(buttonID);
            break;
        case FRAME_VIEW:
            return subView.getUIButtonPointer(buttonID);
            break;
        case SCROLL_VIEW:
            //return scrollView.getUIButtonPointer(buttonID);
            return NULL;
            break;
        case TAB_VIEW:
            return NULL; //tabView.checkMouseClickEvent(x,y);
            break;
        default:
            printf("ERROR: ofUIViewContainer::getUIButtonPointer(). Unkown View Type\n");
            return NULL;
            break;
    }
    return NULL;
}

bool ofUIViewContainer::setSubViewVisibleState(string subViewID,bool state){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            if( subView.getSubViewID() == subViewID ){
                subView.setVisible( state );
                return true;
            }
            return false;
            break;
        case FRAME_VIEW:
            return false;
            break;
        case SCROLL_VIEW:
            return false; 
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setTextFieldValue(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::setFrameViewVisibleState(string frameViewID,bool state){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return false;
            break;
        case FRAME_VIEW:
            if( frameView.frameViewID == frameViewID ){
                frameView.setVisible(state);
                return true;
            }
            return false;
            break;
        case SCROLL_VIEW:
            return false; 
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setTextFieldValue(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::setTextFieldValue(string textFieldID,float value){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.setTextFieldValue(textFieldID,value);
            break;
        case FRAME_VIEW:
            return frameView.setTextFieldValue(textFieldID,value);
            break;
        case SCROLL_VIEW:
            return scrollView.setTextFieldValue(textFieldID,value); 
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setTextFieldValue(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::setTextFieldValue(string textFieldID,string text){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.setTextFieldValue(textFieldID,text);
            break;
        case FRAME_VIEW:
            return frameView.setTextFieldValue(textFieldID,text);
            break;
        case SCROLL_VIEW:
            return scrollView.setTextFieldValue(textFieldID,text);
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setTextFieldValue(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::setSliderValue(string sliderID,float value){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.setSliderValue(sliderID,value);
            break;
        case FRAME_VIEW:
            return frameView.setSliderValue(sliderID,value);
            break;
        case SCROLL_VIEW:
            return scrollView.setSliderValue(sliderID,value);
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setSliderValue(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::setMappedSliderValue(string sliderID,float value){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.setMappedSliderValue(sliderID,value);
            break;
        case FRAME_VIEW:
            return frameView.setMappedSliderValue(sliderID,value);
            break;
        case SCROLL_VIEW:
            return scrollView.setMappedSliderValue(sliderID,value);
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setMappedSliderValue(). Unkown View Type\n");
            return false;
            break;
    }
}

bool ofUIViewContainer::setButtonText(string buttonID,string text){
    switch(viewType){
        case NOT_SET:
            return false;
            break;
        case SUB_VIEW:
            return subView.setButtonText(buttonID,text);
            break;
        case FRAME_VIEW:
            return frameView.setButtonText(buttonID,text);
            break;
        case SCROLL_VIEW:
            return scrollView.setButtonText(buttonID,text);
            break;
        case TAB_VIEW:
            return false; 
            break;
        default:
            printf("ERROR: ofUIViewContainer::setMappedSliderValue(). Unkown View Type\n");
            return false;
            break;
    }
}

