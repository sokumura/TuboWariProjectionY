//
//  magicTest.h
//  testScetch2
//
//  Created by Shuya Okumura on 2013/02/13.
//
//

#pragma once
#include "ofMain.h"
#include "MACROS.h"

extern int startMahoTime, currentMahoTime;

extern int magicIndex;
extern ofFloatColor mainPink;
extern ofPoint magicPos[2];

extern ofPoint hondanaPoints[HONDANA_INDEX_NUM];
extern float hondanaNoiseAmt;
//line//
extern float accelAmt;
extern ofFloatColor mainPink;
extern float lineColorAmt;

//bunshi
extern int bunshiNum;
extern float mainParticleSize;
extern float radiusAmt;
extern float radiusMin;
extern float radiusMax;
extern float energyAmt;
extern float kasokuAmt;
extern float alphaAmt;
extern int mainBunshiLifeTime;
//line2
extern float maxLength;
extern float minLength;
extern float lengthNoiseAmt;
extern float line2speed;
extern float angleAmt;
extern float line2width;
//line3
extern bool bLine1;
extern bool bBunshi;
extern bool bLine2;
//noiseBlob
extern bool bBlob;
extern int blobPointNum;//1 - 100 = 10
extern float sparkRangeMin;
extern float sparkRangeMax;
//keen
extern bool trKeen;
extern float keenWidth;
////////////////
class myLine2 {
public:
    ofPoint accel;//x:near, y:far
    ofPoint linePoint;//x:near, y:far
    myLine2(float _angle){
        angle = _angle;
        lifeTime = 1000 + round(ofRandomf() * 39);
        c = mainPink + fabs(ofRandomf()) * lineColorAmt;
        linePoint = ofPoint(0.0f, 0.0f);
        completePoint = ofPoint( minLength + ofRandomf() * lengthNoiseAmt, maxLength + ofRandomf() * lengthNoiseAmt);
        complete = false;
    };
    void update(){
        angle += angleAmt;
        linePoint.x += (completePoint.x - linePoint.x) * line2speed;
        linePoint.y += (completePoint.y - linePoint.y) * line2speed;
        c.a *= 0.265f;
    };
    void draw(float x, float y){
        if (lifeTime < currentMahoTime) return;
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y);
        c = mainPink + ofRandomf() * lineColorAmt;
        ofRotateZ(angle);
        ofSetColor(c);
        ofSetLineWidth(line2width);
        ofLine(linePoint.x, 0.0f, linePoint.y, 0.0f);
        ofPopMatrix();
        ofPopStyle();
    };
private:
    ofFloatColor c;
    ofPoint completePoint;//x:near, y:far
    int lifeTime;
    int lifeTimeAmt;
    float angle;
    bool complete;
};

////////////////
class bunshi {
public:
    bunshi(){
        energy = fabs(ofRandomf()) * energyAmt;
        c = mainPink;
        c *= energy + 0.2;
        //c = ofClamp(c, 0.5f, 1.0f);
        lifeTime = mainBunshiLifeTime * energy;
        radius = mainParticleSize * energy;
        pos.set(0.0f, 0.0f);
    };
    float update(ofPoint kasoku){
        ofPoint zero = ofPoint(0.0f, 0.0f);
        speed += kasoku;
        pos += speed * energy * energyAmt;
        radius += ofRandomf() * radiusAmt;
        radius = ofClamp(radius, radiusMin, radiusMax);
        if (currentMahoTime > lifeTime) {
            c *= alphaAmt;
        }
    };
    void draw(){
        ofSetColor(c);
        ofCircle(pos.x, pos.y, radius);
    };
    ofFloatColor c;
    int lifeTime;
private:
    ofPoint     pos;
    ofPoint     speed;
//    ofPoint     kasoku;
    float       energy;//dousei
    float       radius;
    
};
//////////////
class myLine {
public:
    ofPoint accel;//x:near, y:far
    float alphaSpeed;
    float angle;
    ofPoint linePoint;//x:near, y:far
    myLine(float _angle){
        angle = _angle;
        linePoint = ofPoint();
        speed = ofPoint();
        accel = ofPoint();
        alphaSpeed = 0.0f;
    };
    void update(ofPoint & _accel){
        accel = _accel;
        speed += accel;//speed no suii
        linePoint += speed;//line no ido
    };
    void draw(){
        c = mainPink + ofRandomf() * lineColorAmt;
        ofSetColor(c);
        ofLine(linePoint.x, 0.0f, linePoint.y, 0.0f);
    };
private:
    ofFloatColor c;
    ofPoint speed;//x:near, y:far
    
};
////////////////
class noiseBlob {
public:
    noiseBlob(){
        isOn = false;
    };
    void toggleStart(int noiseBlobPointNum){
        blob.clear();
        vecs.clear();
        float anAngle = TWO_PI / noiseBlobPointNum;
        for (int i = 0; i < noiseBlobPointNum; i++) {
            ofPoint p = ofPoint(cos(anAngle * i), sin(anAngle * i));
            vecs.push_back(p);
            blob.push_back(ofPoint(0,0));
        }
        isOn = true;
    };
    void update(){
        if (blob.size() == 0 || isOn == false) return;
        for (int i = 0; i < blob.size(); i++) {
            blob[i] = vecs[i] * ofRandom(sparkRangeMin, sparkRangeMax);
        }
    };
    void draw(float x, float y){
        if (blob.size() == 0 || !isOn) return;
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y);
        ofFill();
        ofSetColor(mainPink);
        ofSetPolyMode(OF_POLY_WINDING_ODD);
        ofBeginShape();
        for (int i = 0; i < blob.size(); i++) {
            if (i % 2 == 0) continue;
            if (i == blob.size() - 1) {
                ofVertex(blob[i]);
                ofVertex(blob[0]);
            } else {
                ofVertex(blob[i]);
            }
            
        }
        ofEndShape();
        ofPopMatrix();
        ofPopStyle();
    };
    bool isOn;
private:
    vector<ofPoint> blob;
    vector<ofPoint> vecs;
    
};
////////////////
class Goko1{
public:
    void update(){
        if (lines.size() > 0 && !isOn) {
            lines.clear();
        }
        if (bunshis.size() > 0 && !isOn) {
            bunshis.clear();
        }
        if (lines2.size() > 0 && !isOn) {
            lines2.clear();
        }
        for (int i = 0; i < bunshis.size(); i++) {
            bunshis[i].update(ofPoint(ofRandomf() * kasokuAmt, ofRandomf() * kasokuAmt));
        }
        //printf("///\n");
        for (int i = 0; i < lines.size(); i++) {
            float r1 = std::fabs(ofRandomf());
            float r2 = std::fabs(ofRandomf());
            ofPoint p;
            p.set(r1 * accelAmt, r2 * accelAmt);
            lines[i].update(p);
        }
        for (int i = 0; i < lines2.size(); i++) {
            lines2[i].update();
        }
        b.update();
    }
    void draw( float x, float y){
        b.draw(x, y);
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y);
        for (int i = 0; i < bunshis.size(); i++) {
            bunshis[i].draw();
        }
        ofSetLineWidth(4.0f);
        for (int i = 0; i < lines.size(); i++) {
            ofPushMatrix();
            ofRotateZ(lines[i].angle);
            lines[i].draw();
            ofPopMatrix();
        }
        ofPopMatrix();
        ofPopStyle();
        for (int i = 0; i < lines2.size(); i++) {
            lines2[i].draw(x, y);
        }
    }
    void toggleSpark(int lineNum, int bunshiNum, int line2Num, bool & toggleGoko1){
        if (!toggleGoko1) return;
        //time
        setMahoTimerMillis(startMahoTime);
        if (bLine1) {
            float anAngle = 360.0f / lineNum;
            float firstAngle = ofGetElapsedTimef() * 360.0f;
            for (int i = 0; i < lineNum; i++) {
                myLine line = myLine(firstAngle + anAngle * i + ofRandomf());
                lines.push_back(line);
            }
        }
        if (bBunshi){
            //bunshi
            for (int i = 0; i < bunshiNum; i++) {
                bunshi b = bunshi();
                bunshis.push_back(b);
            }
        }
        if (bLine2) {
            //line2
            float aAm = 360.0f / line2Num;
            for (int i = 0; i < line2Num; i++) {
                if (ofRandomf() > 0) continue;
                myLine2 l = myLine2(aAm * i);
                lines2.push_back(l);
            }
        }
        if (bBlob){
            b.toggleStart(blobPointNum);
        }
        printf("goko1発射!!\n");
        toggleGoko1 = false;
        isOn = true;
    }
    void setMahoTimerMillis(int & timer){
        timer = ofGetElapsedTimeMillis();
    };
    void setOff(){
        isOn = false;
        b.isOn = false;
        //trGoko1 = false;
    };
private:
    vector<myLine> lines;
    vector<myLine2> lines2;
    vector<bunshi> bunshis;
    noiseBlob b;
    float rotateSpeed;
    ofColor color;
    bool isOn;
};

class Keen {
public:
    Keen(){
        isOn = false;
    };
    
    void setup(){};
    void update(){};
    void draw(){
        if (isOn && currentMahoTime < 500) {
            ofPushMatrix();
            ofNoFill();
            ofSetColor(mainPink);
            ofSetLineWidth(keenWidth);
            ofLine(magicPos[0], magicPos[1]);
            ofPopMatrix();
        } else if (isOn && currentMahoTime > 500 &&currentMahoTime < 2000) {
            if (magicIndex == 0) {
                ofPushStyle();
                ofPushMatrix();
                ofFill();
                ofSetColor(mainPink);
                ofBeginShape();
                for (int i = 0; i < HONDANA_INDEX_NUM; i++) {
                    ofVertex(hondanaPoints[i]);
                    hondanaPoints[i] += ofRandom((-1)* hondanaNoiseAmt, hondanaNoiseAmt);
                }
                ofEndShape();
                ofPopMatrix();
                ofPopStyle();
            } else if (magicIndex == 1) {
                
            } else if (magicIndex == 2) {
            
            }
            
        } else if (isOn && currentMahoTime > 4000) {
            isOn = false;
        }
    };
    void setMahoTimerMillis(int & timer){
        timer = ofGetElapsedTimeMillis();
    };
    void toggleKeen(){
        setMahoTimerMillis(startMahoTime);
        isOn = true;
        trKeen = false;
    };
    
private:
    bool isOn;
    
};


