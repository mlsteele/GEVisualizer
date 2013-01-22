#include "ofUIButtonHandler.h"

ofUIButtonHandler::ofUIButtonHandler():numButtons(0),appPointer(NULL){

}

ofUIButtonHandler::~ofUIButtonHandler(void){
	clearButtonBuffer();
}

void ofUIButtonHandler::clearButtonBuffer(){
	buttonBuffer.clear();
    numButtons = 0;
}

bool ofUIButtonHandler::checkMouseClickEvent(unsigned int mouseX,unsigned int mouseY){
	
	bool buttonPressed = false;
    printf("Checking for mouse click event...\n");
	for(unsigned int i=0; i<numButtons; i++){
        ofUIButton *button;
        button = buttonBuffer[i];
        unsigned int buttonX = button->getScreenX();
        unsigned int buttonY = button->getScreenY();
        unsigned int buttonWidth = button->getWidth();
        unsigned int buttonHeight = button->getHeight();
        string txt = button->getText();
        printf("Button %i Text: %s x: %i y: %i width: %i height %i\n",i,txt.c_str(),buttonX,buttonY,buttonWidth,buttonHeight);
        
		if( mouseX >= buttonX && mouseX <= (buttonX + buttonWidth) &&
			mouseY >= buttonY && mouseY <= (buttonY + buttonHeight) 
		){
			if( buttonBuffer[i]->isEnabled() ){
                printf("BUTTON HIT\n");
				buttonBuffer[i]->triggerCallback(appPointer);
				buttonPressed = true;
			}
		}
	}

	return buttonPressed;
}

void ofUIButtonHandler::addButton(ofUIButton *button){
	numButtons++;
	buttonBuffer.push_back( button );
}

bool ofUIButtonHandler::eraseButton(ofUIButton *button){
	bool buttonRemoved = false;

	return buttonRemoved;
}

void ofUIButtonHandler::setAllButtonStatesTo(bool state){

	for(int i=0; i<numButtons; i++){
		buttonBuffer[i]->setEnabled(state);
	}

}

void ofUIButtonHandler::deleteAllButtons(){

    numButtons = 0;
    buttonBuffer.clear();

}

