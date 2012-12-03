#pragma once

#include "ofxSimpleGuiValueControl.h"


class ofxSimpleGuiButton : public ofxSimpleGuiValueControl<bool> {
public:
	bool			oldValue;
	bool			isPressed;
	
	ofxSimpleGuiButton(string name, bool &value);	
	void setup();
#ifndef OFXMSAGUI_DONT_USE_XML
	void loadFromXML(ofxXmlSettings &XML);	
	void saveToXML(ofxXmlSettings &XML);
#endif
	
    void keyPressed( int key );

	bool getValue();
	void setValue(bool b);
	void toggle();
	
	void setToggleMode(bool b);
	
	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void draw(float x, float y);
private:
    int counter;
    
};
