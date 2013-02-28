//
//  myLine2.h
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/19.
//
//
#pragma once
#include "ofMain.h"

//==================================================================
class myLine2 {
public:
    myLine2(float _angle);
    ofPoint accel;//x:near, y:far
    ofPoint linePoint;//x:near, y:far
    
    void update();
    void draw();
private:
    ofFloatColor c;
    ofPoint completePoint;//x:near, y:far
    int lifeTime;
    int lifeTimeAmt;
    float angle;
    bool complete;
};