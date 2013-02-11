//
//  ofxSimpleGuiMulti2dSlider.h
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/11.
//
//
#pragma once
#include "ofxSimpleGuiControl.h"


class ofxSimpleGuiMulti2dSlider : public ofxSimpleGuiControl {
public:
	ofPoint * pValues;
    ofPoint * pSliderPoint;
	ofPoint	min, max;
    float aspect;
    
	ofxSimpleGuiMulti2dSlider(string name, int nBlock, int pointNum, ofPoint * values, float xmin, float xmax, float ymin, float ymax, float sliderAspectWbyH = 1.0f, bool bBgTransparent = false);
	void setup();
	void setValue(int num, float x, float y);
	void setMin(float x, float y);
	void setMax(float x, float y);
	void onPress(int x, int y, int button);
	void onDragOver(int x, int y, int button);
	void onDragOutside(int x, int y, int button);
	void onRelease();
	void update();
	void draw(float x, float y);
private:
    float sliderWidth;
    float sliderHeight;
    float sliderTextHeight;
    bool  bBgTrans;
    int nPointNum;
    int targetNo;
};
