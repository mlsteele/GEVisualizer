#include "ofUITextField.h"

ofUITextField::ofUITextField():callbackFunction(NULL){
	textFieldID = "";
    bounds.x = 0;
	bounds.y = 0;
	bounds.width = 0;
	bounds.height = 0;
	text = "";
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	backgroundColor.a = 255;
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	foregroundColor.a = 255;
    selectedColor.r = 0;
	selectedColor.g = 0;
	selectedColor.b = 0;
	selectedColor.a = 255;
	visible = true;
	enabled = true;
    selected = false;
    editable = true;
    numberBox = false;
    objectType = "UI_TEXT_FIELD";
}

ofUITextField::~ofUITextField(void){

}

void ofUITextField::init(string textFieldID,ofRectangle bounds,string text,ofColor fieldColor,ofColor textColor,ofColor selectedColor,bool isNumberBox){
    this->textFieldID = textFieldID;
    this->bounds = bounds;
    this->text = text;
    this->backgroundColor = backgroundColor;
    this->foregroundColor = foregroundColor;
    this->selectedColor = selectedColor;
    this->numberBox = isNumberBox;
}

void ofUITextField::draw(float parentX,float parentY){

	if( visible ){
        originX = parentX;
        originY = parentY;
        
		//Draw the background
		ofSetColor(backgroundColor.r,backgroundColor.g,backgroundColor.b,backgroundColor.a);
        ofFill();
		ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);
		
        //Set the text color
        if( selected ) ofSetColor(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a);
        else ofSetColor(foregroundColor.r,foregroundColor.g,foregroundColor.b,foregroundColor.a);
        
        //Draw the border
        ofNoFill();
		ofRect(originX + bounds.x, originY + bounds.y, bounds.width, bounds.height);

        //Draw the text
		float textX = originX + bounds.x;
		float textY = originY + bounds.y;
        string txt = text;
		ofRectangle rect = font.getStringBoundingBox(txt, 0,0);
        
        if( numberBox == false ){
            //If the text is smaller than the box then we try and center it within the box
            if( rect.width < bounds.width ){
                textX = (textX+(bounds.width/2)) - (rect.width/2);
            }else{
                //Shorten the text so it fits in the box
                float letterWidth = rect.width/txt.length();
                float numLetters = floor(bounds.width/letterWidth) - 1;
                txt = "";
                for(unsigned int i=0; i<numLetters; i++){
                    txt += text[ text.length()-1-numLetters+i ];
                }
            }
            if( rect.height < bounds.height )
                textY = (textY+(bounds.height/2)) + (rect.height/2);
        }else{
            //If the text is smaller than the box then we try and center it within the box
            if( rect.width < bounds.width ){
                textX = textX + 5;
            }else{
                textX = textX + 5;
                //Shorten the text so it fits in the box
                float letterWidth = rect.width/txt.length();
                float numLetters = floor(bounds.width/letterWidth) - 1;
                txt = "";
                for(unsigned int i=0; i<numLetters; i++){
                    txt += text[ i ];
                }
            }
            if( rect.height < bounds.height )
                textY = (textY+(bounds.height/2)) + (rect.height/2);

        }
        
		font.drawString(txt, textX, textY);
	}
}

void ofUITextField::setSelected(bool selectionState){
    if( selectionState ){
        selected = true;
    }else{
        //If the text field has been selected then we need to unselect it and trigger the callback
        if( selected ){
            selected = false;
            triggerCallback(NULL);
        }
    }
}

void ofUITextField::setCallback(TextFieldCallbackFunction function){
    callbackFunction = function;
}

bool ofUITextField::triggerCallback(void *appPointer){
    if( callbackFunction != NULL ){
        callbackFunction(this,appPointer);
        return true;
    }
    return false;
}

void ofUITextField::setFont(ofTrueTypeFont font){
	this->font = font;
}

bool ofUITextField::mousePressedEvent(float mouseX,float mouseY){
    if( !enabled || !editable ) return false;
    mouseX -= originX;
    mouseY -= originY;
    if( mouseX >= bounds.x && mouseX <= (bounds.x + bounds.width) &&
       mouseY >= bounds.y && mouseY <= (bounds.y + bounds.height) )
    {
        selected = !selected;
        triggerCallback(mainAppHandle);
        return true;
    }
    return false;
}

bool ofUITextField::keyPressedEvent(int key){
    if( !selected || !enabled || !editable ) return false;
    
    char c = static_cast<char>( key );
    
    switch(key){
        case(OF_KEY_BACKSPACE):
            if( text.length() > 1 ){
                text.erase(text.end()-1,text.end());
            }else text = "";
            break;
        case(OF_KEY_RETURN):
            selected = false;
            triggerCallback(mainAppHandle);
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
            text += c;
            break;
    }
    
    return true;
    
}

float ofUITextField::getValueAsFloat(){
    float value = 0;
    if( text == "" ) return INFINITY;
    if( !fromString<float>(value, std::string(text), std::dec) )
    {
        value = INFINITY;
    }
    return value;
}

string ofUITextField::getValueAsText(){
    return text;
}

bool ofUITextField::setTextFieldValue(float value){
    text = toString(value);
    return true;
}

bool ofUITextField::setTextFieldValue(string text){
    this->text = text;
    return true;
}

