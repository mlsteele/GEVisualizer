#include "ofUIView.h"

ofUIView::ofUIView()
{
    hSpacer = 10;
    vSpacer = 10;
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	backgroundColor.a = 255;
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	foregroundColor.a = 255;
    objectType = "UI_VIEW";
}