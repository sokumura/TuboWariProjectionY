//
//  ofxSimpleGuiMulti2dSlider.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/11.
//
//

#include "ofxSimpleGuiMulti2dSlider.h"


ofxSimpleGuiMulti2dSlider::ofxSimpleGuiMulti2dSlider(string name, int nBlock, int pointNum, ofPoint * values, float xmin, float xmax, float ymin, float ymax, float sliderAspectWbyH, bool bBgTransparent) : ofxSimpleGuiControl(name) {
    pSliderPoint = (ofPoint *)malloc(sizeof(ofPoint) * pointNum);
    pValues = values;
    
	this->width = nBlock * config->width + (nBlock - 1) * config->padding.x;
	controlType = "ContentSlider2D";
    bBgTrans = bBgTransparent;
    aspect = sliderAspectWbyH;
    if (xmin > xmax) std::swap(xmin, xmax);
    if (ymin > ymax) std::swap(ymin, ymax);
	min.set(xmin, ymin);
	max.set(xmax, ymax);
    
    for (int i = 0; i < nPointNum; i++) {
        pValues[i].x = ofClamp(pValues[i].x, min.x, max.x);
        pValues[i].y = ofClamp(pValues[i].y, min.y, max.y);
    }
    
    sliderHeight = width / aspect;
    sliderTextHeight = config->slider2DTextHeight;
    
	setSize(width, sliderHeight + sliderTextHeight);
    nPointNum = pointNum;

    
    if (nBlock == 0) {
        ofLog(OF_LOG_ERROR, "ofxSimpleGuiContentSlider2d で nBlockに0が入っている。1以上にすべし。\n");
    }
    if (nBlock > 6) {
        ofLog(OF_LOG_WARNING, "ofxSimpleGuiContentSlider2d で nBlockに7以上の数字が入ってます。大き過ぎて画面に入らないと思います。\n");
    }
    
	setup();
}

void ofxSimpleGuiMulti2dSlider::setup() {
    for (int i = 0; i < nPointNum; i++) {
        pSliderPoint[i].x = ofMap(pValues[i].x, min.x, max.x, 0.0f, width);
        pSliderPoint[i].y = ofMap(pValues[i].y, min.y, max.y, 0.0f, sliderHeight);
    }
}

void ofxSimpleGuiMulti2dSlider::setMin(float x, float y) {
	min.x = x;
	min.y = y;
}

void ofxSimpleGuiMulti2dSlider::setMax(float x, float y) {
	max.x = x;
	max.y = y;
}

void ofxSimpleGuiMulti2dSlider::onPress(int x, int y, int button) {
    targetNo = nPointNum;
    ofPoint p = ofPoint(x,y - sliderTextHeight);
    bool isOnFixButton = x - this->x > width - fixboxWidth && y - this->y < fixboxWidth && bDrawFixButton;
    bool isOnSlider = (y - this->y) > sliderTextHeight;
    if (isOnFixButton) {
        toggleFix();
    } else if (!isFixed() && isOnSlider) {
        float dist = width;
        for (int i = 0; i < nPointNum; i++) {
            float d = p.distance(pValues[i]);
            if (dist > d) {
                targetNo = i;
                dist = d;
            }
        }
        if (targetNo != nPointNum) {
            lock = true;
            pSliderPoint[targetNo] = p;
        }
    }
}

void ofxSimpleGuiMulti2dSlider::onDragOver(int x, int y, int button) {
}

void ofxSimpleGuiMulti2dSlider::onDragOutside(int x, int y, int button) {
    if (!isFixed()) {
        lock = false;
    }
}

void ofxSimpleGuiMulti2dSlider::onRelease() {
    if (!isFixed()) {
        lock = false;
    }
}

void ofxSimpleGuiMulti2dSlider::update() {
    
    if (lock) {
        pSliderPoint[targetNo].x = ofClamp(pSliderPoint[targetNo].x, 0.0f, width);
        pSliderPoint[targetNo].y = ofClamp(pSliderPoint[targetNo].y, 0.0f, sliderHeight);
        pValues[targetNo].x = ofMap(pSliderPoint[targetNo].x, 0, width, min.x, max.x);
        pValues[targetNo].y = ofMap(pSliderPoint[targetNo].y, 0, sliderHeight, min.y, max.y);
    }
}

void ofxSimpleGuiMulti2dSlider::draw(float x, float y) {
	setPos(x, y);
	ofEnableAlphaBlending();
	glPushMatrix();
	glTranslatef(x, y, 0);
    
    ofFill();
//    glColor4f(0, 0, 0, 1.0f);//黒 - 背景
//	ofRect(0, sliderTextHeight, width, sliderHeight);
	
    
    ofSetColor(255, 255, 255, 255);
    setTextBGColor(!isFixed());
    ofRect(0, 0, width, sliderTextHeight);
    
    
    char vs[nPointNum][64];
    for (int i = 0; i < nPointNum; i++) {
        if (i != targetNo) setTextColor(!isFixed());
        else setTextColor2();
        std::sprintf(vs[i], "%u\nx:%.2f\ny:%.2f", i, pValues[i].x,pValues[i].y);
        ofDrawBitmapString(vs[i] , 6 + i * 100.0f, 15.0f);
    }
    ofTranslate((int)0.0f, (int)sliderTextHeight);
    for (int i = 0; i < nPointNum; i++) {
        if (i == targetNo) {
            ofSetHexColor(0xFFFF00);
            ofLine(pValues[i].x, 0, pValues[i].x, sliderHeight);
            ofLine(0, pValues[i].y,width, pValues[i].y);
        } else ofSetHexColor(0xFFFFFF);
        
        ofCircle(pSliderPoint[i].x, pSliderPoint[i].y, 3);
        
        ofSetHexColor(0xFFFFFF);
        ofRectangle maxxB = base64GetStringBoundingBox("X:" + ofToString(max.x));
        string valueString = "(" + ofToString(pValues[i].x) + "," + ofToString(pValues[i].y) + ")";
        ofRectangle valueStringBox = base64GetStringBoundingBox(ofToString(i));
        
        base64DrawBitmapString("(" + ofToString(min.x) + "," + ofToString(min.y) + ")", 1, 2);
        base64DrawBitmapString("X:" + ofToString(max.x), width - maxxB.width - 1, 2);
        base64DrawBitmapString("Y:" + ofToString(max.y), 1, sliderHeight - maxxB.height -1);
        
        if (width/2 > pSliderPoint[i].x && sliderHeight/2 > (pSliderPoint[i].y - y)){
            base64DrawBitmapString(ofToString(i), pSliderPoint[i].x + 3, pSliderPoint[i].y + 3);
        } else if (width/2 < pSliderPoint[i].x && sliderHeight/2 > pSliderPoint[i].y) {//
            base64DrawBitmapString(ofToString(i), pSliderPoint[i].x - valueStringBox.width - 3, pSliderPoint[i].y + 3);
        } else if (width/2 > pSliderPoint[i].x && sliderHeight/2 < pSliderPoint[i].y) {//
            base64DrawBitmapString(ofToString(i), pSliderPoint[i].x + 3, pSliderPoint[i].y - valueStringBox.height - 3);
        } else {//
            base64DrawBitmapString(ofToString(i), pSliderPoint[i].x - valueStringBox.width - 3, pSliderPoint[i].y - valueStringBox.height - 3);
        }
    }
    

    ofTranslate(0, 0 - (int)sliderTextHeight);
    fixButtonDrawOfContentSlider2d();
    
//    if (!fixed) {
//        ofFill();
//        ofSetHexColor(config->emptyColor);
//        ofRect(secBtnPos.x, 1.0f, secondPointBoxWidth, secondPointBoxWidth);
//        ofSetHexColor(0xFFFF00);
//    }
//    else {
//        ofSetHexColor(config->borderColor);
//        ofNoFill();
//    }
//    ofSetLineWidth(0.5);
//    if (bSecondPoint) {
//        ofFill();
//    } else ofNoFill();
//    ofCircle(secBtnPos.x + secondPointBoxWidth / 2.0f, secondPointBoxWidth / 2 + 1.0f, secondPointBoxWidth / 2);
//    ofNoFill();
//    ofSetHexColor(config->borderColor);
//    ofRect(secBtnPos.x - 0.5, 0.5f, secondPointBoxWidth + 1.0f, secondPointBoxWidth + 1.0f);
//    
//    
	glPopMatrix();
	ofDisableAlphaBlending();
	
}