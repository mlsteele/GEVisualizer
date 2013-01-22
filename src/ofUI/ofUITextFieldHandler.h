#pragma once

#include "ofMain.h"
#include "ofUITextField.h"
#include <iostream>

class ofUITextFieldHandler
{
public:
	ofUITextFieldHandler();
	~ofUITextFieldHandler(void);

	bool checkMouseClickEvent(int mouseX,int mouseY);
	void addTextField(ofUITextField *textField);
	bool eraseTextField(ofUITextField *textField);
	void clearTextFieldBuffer();
	bool keyPressed(int key);
	void deleteKeyPressed();
	void setAllTextFieldsStatesTo(bool state);
	void deleteAllTextFields();

	ofUITextField** textFieldBuffer;
	int numTextFields;
	void *appPointer;
};
