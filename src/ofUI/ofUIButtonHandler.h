#pragma once

#include "ofMain.h"
#include "ofUIButton.h"
#include <iostream>
#include <vector>


class ofUIButtonHandler
{
public:
	ofUIButtonHandler();
	~ofUIButtonHandler(void);

	bool checkMouseClickEvent(unsigned int mouseX,unsigned int mouseY);
	void addButton(ofUIButton *button);
	bool eraseButton(ofUIButton *button);
	void clearButtonBuffer();
	void setAllButtonStatesTo(bool state);
	void deleteAllButtons();

	vector< ofUIButton* > buttonBuffer;
	int numButtons;
	void *appPointer;
};
