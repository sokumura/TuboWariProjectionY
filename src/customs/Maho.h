//
//  Maho.h
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/17.
//
//

#pragma once
#include "seraNoise.h"
#include "myLine2.h"
#include "ofMain.h"
#include "grotes.h"
#include "lastParticles.h"

//==================================================================
class hoshi {
public:
    hoshi();
    void draw();
    
    ofPoint pos;
    int lifeTime;
    int counter;
private:
    ofFloatColor color;
    float angle;
};

class Kirakira {
public:
    void update(float x, float y);
    void draw();
    void clear();
private:
    vector<hoshi> hoshis;
    ofPoint eriaPos;
    ofPoint exEriaPos;
};




//==================================================================
class bunshi {
public:
    bunshi();
    float update(ofPoint kasoku);
    void draw();
    ofFloatColor c;
    int lifeTime;
private:
    ofPoint     pos;
    ofPoint     speed;
    //    ofPoint     kasoku;
    float       energy;//dousei
    float       radius;
    
};
//==================================================================
class myLine {
public:
    ofPoint accel;//x:near, y:far
    float alphaSpeed;
    float angle;
    ofPoint linePoint;//x:near, y:far
    myLine(float _angle);
    void update(ofPoint & _accel);
    void draw();
private:
    ofFloatColor c;
    ofPoint speed;//x:near, y:far
    
};
//==================================================================
class noiseBlob {
public:
    void toggleStart(int noiseBlobPointNum);
    void update();
    void draw();
private:
    vector<ofPoint> blob;
    vector<ofPoint> vecs;
    ofColor c;
};
//==================================================================
class Maho{
public:
    void update();
    void draw( float x, float y);
    void toggleSpark();
    void toggleKeen();

    
private:
    vector<myLine> lines;
    vector<myLine2> lines2;
    vector<bunshi> bunshis;
    int phase;
    noiseBlob b;
    noiseBlob b2;
    float rotateSpeed;
    ofColor color;
    int fadeCount;
    bool drawing;
    //sera
    seraNoise sera;
    bool seraDid;
};
//==================================================================


