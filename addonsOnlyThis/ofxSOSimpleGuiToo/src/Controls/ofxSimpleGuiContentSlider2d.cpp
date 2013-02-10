
#include "ofxSimpleGuiContentSlider2d.h"


ofxSimpleGuiContentSlider2d::ofxSimpleGuiContentSlider2d(string name, int nBlock, ofBaseDraws & content, ofPoint& value, float xmin, float xmax, float ymin, float ymax) : ofxSimpleGuiControl(name) {
    this->content = &content;
	this->fixwidth  = nBlock * config->width + (nBlock - 1) * config->padding.x;
	controlType = "ContentSlider2D";
    
	min.set(xmin, ymin);
	max.set(xmax, ymax);
	this->value = &value;
	
    width = fixwidth;
    if (xmin > xmax) std::swap(xmin, xmax);
    if (ymin > ymax) std::swap(ymin, ymax);
    sliderTextHeight = config->slider2DTextHeight;
    if (nBlock == 0) {
        ofLog(OF_LOG_ERROR, "ofxSimpleGuiContentSlider2d で nBlockに0が入っている。1以上にすべし。\n");
    }
    if (nBlock > 6) {
        ofLog(OF_LOG_WARNING, "ofxSimpleGuiContentSlider2d で nBlockに7以上の数字が入ってます。大き過ぎて画面に入らないと思います。\n");
    }
    
	setup();
}

void ofxSimpleGuiContentSlider2d::setup() {
    fixheight = fixwidth * content->getHeight()/content->getWidth();
    sliderHeight = fixheight;
	setSize(fixwidth, fixheight + sliderTextHeight);
    
	point.x = ofMap((*value).x, min.x, max.x, 0.0f, fixwidth);
	point.y = ofMap((*value).y, min.y, max.y, 0.0f, fixheight);
}

#ifndef OFXMSAGUI_DONT_USE_XML
void ofxSimpleGuiContentSlider2d::loadFromXML(ofxXmlSettings &XML) {
	value->set(XML.getValue(controlType + "_" + key + ":valueX", 0.0f), XML.getValue(controlType + "_" + key + ":valueY", 0.0f));
    point.x = ofMap((*value).x, min.x, max.x, 0.0f, fixwidth);
	point.y = ofMap((*value).y, min.y, max.y, 0.0f, fixheight);
    setFix((bool)(XML.getValue(controlType + "_" + key + ":isFixed", 0)));
    
}


void ofxSimpleGuiContentSlider2d::saveToXML(ofxXmlSettings &XML) {
	XML.addTag(controlType + "_" + key);
	XML.pushTag(controlType + "_" + key);
	XML.addValue("name", name);
	XML.addValue("valueX", value->x);
	XML.addValue("valueY", value->y);
    XML.addValue("isFixed", isFixed());
	XML.popTag();
}
#endif

void ofxSimpleGuiContentSlider2d::setValue(float x, float y) {
	(*value).x = x;
	(*value).y = y;
}

void ofxSimpleGuiContentSlider2d::setMin(float x, float y) {
	min.x = x;
	min.y = y;
}

void ofxSimpleGuiContentSlider2d::setMax(float x, float y) {
	max.x = x;
	max.y = y;
}

void ofxSimpleGuiContentSlider2d::onPress(int x, int y, int button) {
    bool isOnFixButton = x - this->x > width - fixboxWidth && y - this->y < fixboxWidth && bDrawFixButton;
    if (isOnFixButton) {
        toggleFix();
    } else if (!isFixed()) {
        lock = true;
        point.set(x - this->x, y - this->y - sliderTextHeight);
    }
}

void ofxSimpleGuiContentSlider2d::onDragOver(int x, int y, int button) {
    bool isOnFixButton = x - this->x > width - fixboxWidth && y - this->y < fixboxWidth && bDrawFixButton;
    if (isOnFixButton) {
        toggleFix();
    } else if (!isFixed()) {
        if (lock) point.set(x - this->x, y - this->y - sliderTextHeight);
    }
	
}

void ofxSimpleGuiContentSlider2d::onDragOutside(int x, int y, int button) {
	bool isOnFixButton = x - this->x > width - fixboxWidth && y - this->y < fixboxWidth && bDrawFixButton;
    if (isOnFixButton) {
        toggleFix();
    } else if (!isFixed()) {
        if (lock) point.set(x - this->x, y - this->y - sliderTextHeight);
    }
}

void ofxSimpleGuiContentSlider2d::onRelease() {
    bool isOnFixButton = x - this->x > width - fixboxWidth && y - this->y < fixboxWidth && bDrawFixButton;
    if (isOnFixButton) {
        toggleFix();
    } else if (!isFixed()) {
        lock = false;
    }
}

void ofxSimpleGuiContentSlider2d::update() {
    //Âà§ÂÆö
    point.x = ofClamp(point.x, 0.0f, fixwidth);
    point.y = ofClamp(point.y, 0.0f, fixheight);
	
	if(lock){
		(*value).x = ofMap(point.x, 0, width, min.x, max.x);
		(*value).y = ofMap(point.y, 0, sliderHeight, min.y, max.y);
	}
}

void ofxSimpleGuiContentSlider2d::draw(float x, float y) {
	setPos(x, y);
	ofEnableAlphaBlending();
	glPushMatrix();
	glTranslatef(x, y, 0);
    
    ofFill();
    glColor4f(0, 0, 0, 1.0f);
	ofRect(0, sliderTextHeight, width, sliderHeight);
	
	ofSetColor(255, 255, 255, 255);
	content->draw(0, sliderTextHeight, fixwidth, fixheight);
    
    setTextBGColor(!isFixed());
    ofRect(0, 0, width, sliderTextHeight);
    
    setTextColor(!isFixed());
    char vs[36];
    std::sprintf(vs, "\nx:%.2f\ny:%.2f", value->x,value->y);
    
    ofDrawBitmapString(name + vs , 6, 15);
    //////////
//    ofSetColor(0, 0, 0,0);
//    setContent2DSliderBGColor(!isFixed());
//    if (!isFixed()) ofRect(0, sliderTextHeight, fixwidth, fixheight);

    ofTranslate((int)0.0f, (int)sliderTextHeight);
    if (!isFixed()) {
        ofSetHexColor(0xFFFFFF);
        ofCircle(point.x, point.y, 2);
        ofLine(point.x, 0, point.x, fixheight);
        ofLine(0, point.y,width, point.y);
    
        ofRectangle maxxB = base64GetStringBoundingBox("X:" + ofToString(max.x));
        char valueString[36];
        std::sprintf(valueString,"( %.2f , %.2f )",(*value).x, (*value).y);
        ofRectangle valueStringBox = base64GetStringBoundingBox(valueString);
        base64DrawBitmapString("(" + ofToString(min.x) + "," + ofToString(min.y) + ")", 1, 2);
        base64DrawBitmapString("X:" + ofToString(max.x), width - maxxB.width - 1, 2);
        base64DrawBitmapString("Y:" + ofToString(max.y), 1, fixheight - maxxB.height -1);
        if (width/2 > point.x && fixheight/2 > (point.y - y)){ //Â∑¶‰∏ä„Å´„ÅÇ„Çã„Å®„Åç
            base64DrawBitmapString(valueString, point.x + 3, point.y + 3);
        } else if (width/2 < point.x && fixheight/2 > (point.y)) {//Âè≥‰∏ä
            base64DrawBitmapString(valueString, point.x - valueStringBox.width - 3, point.y + 3);
        } else if (width/2 > point.x && fixheight/2 < point.y) {//Â∑¶‰∏ã
            base64DrawBitmapString(valueString, point.x + 3, point.y - valueStringBox.height - 3);
        } else {//Âè≥‰∏ã
            base64DrawBitmapString(valueString, point.x - valueStringBox.width - 3, point.y - valueStringBox.height - 3);
        }
    }
    ofTranslate(0, 0 - (int)sliderTextHeight);
    fixButtonDrawOfContentSlider2d();
	glPopMatrix();
	ofDisableAlphaBlending();
	
}