#include "ofUITextFieldHandler.h"

ofUITextFieldHandler::ofUITextFieldHandler():textFieldBuffer(NULL),numTextFields(0),appPointer(NULL){

}

ofUITextFieldHandler::~ofUITextFieldHandler(void){
	clearTextFieldBuffer();
}

void ofUITextFieldHandler::clearTextFieldBuffer(){
	if( textFieldBuffer != NULL ){
		delete textFieldBuffer;
		textFieldBuffer = NULL;
		numTextFields = 0;
	}
}

bool ofUITextFieldHandler::checkMouseClickEvent(int mouseX,int mouseY){
	
	bool textFieldPressed = false;

	for(int i=0; i<numTextFields; i++){
		if( mouseX >= textFieldBuffer[i]->getX() &&
		    mouseX <= (textFieldBuffer[i]->getX() + textFieldBuffer[i]->getWidth()) &&
			mouseY >= textFieldBuffer[i]->getY() &&
			mouseY <= (textFieldBuffer[i]->getY() + textFieldBuffer[i]->getHeight()) &&
			textFieldBuffer[i]->isEnabled()
		){
			//Check to see if any other text field is selected
			//If so then activate it's event and then set it's value to false
			for(int j=0; j<numTextFields; j++){
				if( i!=j ){
					if( textFieldBuffer[j]->isSelected() ){
						textFieldBuffer[j]->triggerCallback(appPointer);
						textFieldBuffer[j]->setSelected( false );
					}
				}
			}

			//Now activate this text fileds event
			textFieldBuffer[i]->setSelected( true );
			textFieldBuffer[i]->triggerCallback(appPointer);
			textFieldPressed = true;
		}
	}

	//Make sure there is not a pending event
	if( !textFieldPressed ){
		    //Check to see if any text field is selected
			//If so then activate it's event and then set it's value to false
			for(int i=0; i<numTextFields; i++){
				if( textFieldBuffer[ i ]->isSelected() ){
					textFieldBuffer[ i ]->triggerCallback(appPointer);
					textFieldBuffer[ i ]->setSelected( false );
				}
			}
	}

	return textFieldPressed;
}

void ofUITextFieldHandler::addTextField(ofUITextField *textField){
	if( textFieldBuffer == NULL ){
		textFieldBuffer = new ofUITextField*[1];
		numTextFields = 1;
		textFieldBuffer[0] = textField;
	}else{
		ofUITextField** tempBuffer = new ofUITextField*[numTextFields+1];
		//Copy the current values in the buttonBuffer into the temp buffer
		for(int i=0; i<numTextFields; i++) tempBuffer[i] = textFieldBuffer[i];
		//Add the new button to the end
		tempBuffer[numTextFields++] = textField;
		//Finally overwrite the old buffer
		textFieldBuffer = tempBuffer;
	}
}

bool ofUITextFieldHandler::eraseTextField(ofUITextField *textField){
	bool buttonRemoved = false;

	return buttonRemoved;
}

bool ofUITextFieldHandler::keyPressed(int key){
	bool keyActioned = false;
	if( textFieldBuffer != NULL ){
		for(int i=0; i<numTextFields; i++){
			if( textFieldBuffer[i]->isSelected() && textFieldBuffer[i]->isEnabled() ){
				char c = static_cast<char>( key );
				keyActioned = true;

				switch(key){
					case(OF_KEY_BACKSPACE):
						if( textFieldBuffer[i]->text.length() > 1 ){
							textFieldBuffer[i]->text.erase(textFieldBuffer[i]->text.end()-1,textFieldBuffer[i]->text.end());
						}else textFieldBuffer[i]->text = "";
						break;
					case(OF_KEY_RETURN):
						textFieldBuffer[i]->setSelected( false );
						textFieldBuffer[i]->triggerCallback(appPointer);
						break;
					case(OF_KEY_F1):
						break;
					case(OF_KEY_F2):
						break;
					case(OF_KEY_F3):
						break;
					case(OF_KEY_F4):
						break;
					case(OF_KEY_F5):
						break;
					case(OF_KEY_F6):
						break;
					case(OF_KEY_F7):
						break;
					case(OF_KEY_F8):
						break;
					case(OF_KEY_F9):
						break;
					case(OF_KEY_F10):
						break;
					case(OF_KEY_F11):
						break;
					case(OF_KEY_F12):
						break;
					case(OF_KEY_UP):
						break;
					case(OF_KEY_LEFT):
						break;
					case(OF_KEY_RIGHT):
						break;
					case(OF_KEY_DOWN):
						break;
					case(OF_KEY_PAGE_UP):
						break;
					case(OF_KEY_PAGE_DOWN):
						break;
					case(OF_KEY_HOME):
						break;
					case(OF_KEY_END):
						break;
					case(OF_KEY_INSERT ):
						break;
					default:
						textFieldBuffer[i]->text += c;
						break;
				}

			}
		}
	}
	return keyActioned;
}

void ofUITextFieldHandler::setAllTextFieldsStatesTo(bool state){

	for(int i=0; i<numTextFields; i++){
		textFieldBuffer[i]->setEnabled( state );
	}

}

void ofUITextFieldHandler::deleteAllTextFields(){

	for(int i=0; i<numTextFields; i++){
		delete textFieldBuffer[i];
		textFieldBuffer[i] = NULL;
	}
	delete[] textFieldBuffer;
	textFieldBuffer = NULL;
	numTextFields = 0;

}