//
//  myLine2.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/19.
//
//

#include "myLine2.h"

extern ofFloatColor mainPink;
extern float lineColorAmt;
extern float mahoLine2MinLength;
extern float mahoLine2MaxLength;
extern float mahoLine2AngleAmt;
extern float line2speed;
extern float lengthNoiseAmt;
extern int currentMahoTime;
extern float line2width;
extern bool trFadeOutLastMaho;

//==================================================================
myLine2::myLine2(float _angle){
    angle = _angle;
    lifeTime = 2000 + round(ofRandomf() * 39);
    c = mainPink + fabs(ofRandomf()) * lineColorAmt;
    linePoint = ofPoint(0.0f, 0.0f);
    completePoint = ofPoint( mahoLine2MinLength + ofRandomf() * lengthNoiseAmt, mahoLine2MaxLength + ofRandomf() * lengthNoiseAmt);
    complete = false;
}
void myLine2::update(){
    angle += mahoLine2AngleAmt;
    linePoint.x += (completePoint.x - linePoint.x) * line2speed;
    linePoint.y += (completePoint.y - linePoint.y) * line2speed;
    //c.a *= 0.265f;
    if (trFadeOutLastMaho) {
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
    }
}
void myLine2::draw(){
    //if (lifeTime < currentMahoTime) return;
    ofPushStyle();
    ofPushMatrix();
    ofNoFill();
    //c = mainPink + ofRandomf() * lineColorAmt;
    ofRotateZ(angle);
    ofSetColor(c);
    //ofSetColor(mainPink);
    ofSetLineWidth(line2width);
    ofLine(linePoint.x, 0.0f, linePoint.y, 0.0f);
    ofPopMatrix();
    ofPopStyle();
}