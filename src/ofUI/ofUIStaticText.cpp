#include "ofUIStaticText.h"
ofUIStaticText::ofUIStaticText(){
	text = "";
	staticTextID = "";
    
	txtColor.r = 255;
	txtColor.g = 255;
	txtColor.b = 255;
	txtColor.a = 255;
	visible = true;
	enabled = true;
    editable = false;
    alignmentMode = LEFT_ALIGN;
    objectType = "UI_STATIC_TEXT";
}

ofUIStaticText::~ofUIStaticText(void)
{
}

void ofUIStaticText::init(string staticTextID,ofRectangle bounds,string text,ofColor txtColor,unsigned int alignmentMode){
    this->staticTextID = staticTextID;
    this->bounds = bounds;
    this->text = text;
    this->txtColor = txtColor;
    this->alignmentMode = alignmentMode;
}

void ofUIStaticText::draw(float parentX,float parentY)
{
	if( visible ){
		std::string staticText = text;
        
        originX = parentX;
        originY = parentY;

        if( font.isLoaded() ){
            //ofSetColor(200, 100, 0);
            //ofFill();
            //ofRect(originX + bounds.x,originY + bounds.y,bounds.width,bounds.height );
            
            ofSetColor(txtColor);
            float textX = originX + bounds.x;
            float textY = originY + bounds.y;
            ofRectangle rect = font.getStringBoundingBox(staticText, 0,0);
            
            switch (alignmentMode) {
                case LEFT_ALIGN:
                    //Do nothing
                    break;
                case CENTER_ALIGN:
                    textX = (textX+(bounds.width/2)) - (rect.width/2);
                    break;
                case RIGHT_ALIGN:
                    textX = textX + bounds.width - rect.width;
                    break;
                default:
                    printf("ERROR: ofUIStaticText::draw(float parentX,float parentY). Unknown alignmentMode!\n");
                    break;
            }
            
            if( rect.height < bounds.height )
                textY = (textY+(bounds.height/2)) + (rect.height/2);
            
            font.drawString(staticText, textX, textY);
        }
	}
}

void ofUIStaticText::setFont(ofTrueTypeFont font){
	this->font = font;
}

