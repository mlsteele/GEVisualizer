#pragma once

#include <iostream>
#include <string>
#include "ofUIObject.h"
#include "ofMain.h"
#include "ofUIView.h"

class ofUIStaticText : public ofUIObject
{
public:
    ofUIStaticText();
	~ofUIStaticText(void);

    void init(string staticTextID,ofRectangle bounds,string text,ofColor txtColor,unsigned int alignmentMode = LEFT_ALIGN);
	
    //Main GUI Interface Methods
    void draw(float parentX,float parentY);
    
    //Getters
    string getStaticTextID(){ return staticTextID; }
    string getText(){ return text; }
    
    //Setters
    void setText(string text){ this->text = text; }
    void setFont(ofTrueTypeFont font);
	
	std::string staticTextID;
	ofColor txtColor;
    enum AlignmentModes{LEFT_ALIGN=0,CENTER_ALIGN,RIGHT_ALIGN};
    
private:
    std::string text;
    ofTrueTypeFont font;
    unsigned int alignmentMode;
    
};
