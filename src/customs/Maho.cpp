//
//  Maho.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/17.
//
//

#include "Maho.h"
#include "MACROS.h"

extern int startMahoTime, currentMahoTime;

extern int mahoIndex;
extern ofFloatColor mainPink;
extern bool trFadeOutLastMaho;
extern ofPoint mahoPos[2];

extern ofPoint hondanaPoints[HONDANA_INDEX_NUM];
extern float hondanaNoiseAmt;
//MAHO//
extern int mahoPhase;
extern int mahoPhase2Time[7];
extern int keenTime;
extern bool toggleCvFadeOut;
extern ofColor openCvDrawColor;
extern ofPoint seraBlob[6];
//openCvMissed
extern int choicedXtion;
extern bool bDrawScene1Blob;
extern bool bDraw[XTION_NUM];
//sera
extern bool bOpenCv;
extern bool bSera;
extern bool bDrawSeraBlob;
extern ofPoint seraHeadPos[1];
//harada door
extern bool bCvBlur;
extern float cvNoiseAmt;
extern bool bCvNoise;
extern ofPoint doorPos[6];
extern ofPoint doorPosDamy[6];
extern float doorGunyaAmt;
//harada fire
extern bool bHaradaFire;
extern grotes haradaFire;

//line//
extern bool bLine1;
extern int line1Num;
extern float accelAmt;
extern float lineColorAmt;

//bunshi
extern bool bBunshi;
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
extern bool bLine2;
extern int line2Num;
extern float mahoLine2MaxLength;
extern float mahoLine2MinLength;
extern float lengthNoiseAmt;
extern float line2speed;
extern float mahoLine2AngleAmt;
extern float line2width;

//noiseBlob
extern bool bBlob;
extern int blobPointNum;//1 - 100 = 10
extern float sparkRangeMin;
extern float sparkRangeMax;
////keen
extern float keenWidth;
//kirakira
extern bool bKirakiraDraw;
extern float kirakiraHankei;
extern int kirakiraAmt;
extern bool bKirakiraIncrease;
//hoshi
extern float hoshiSize; //5.0f
extern int   hoshiMainLifeTime;//100 - 10000
extern float kirakiraColorRangeAmt;
//sushi
extern ofPoint rokkaPosDamy[4];
extern ofPoint rokkaPos[4];

extern lastParticles lastparticles;

//==================================================================
hoshi::hoshi(){
    lifeTime = hoshiMainLifeTime * fabs(ofRandomf());
    angle = 360.0f * ofRandomf();
    counter = 0;
    color = mainPink;
    color.g += ofRandomf() * kirakiraColorRangeAmt;
    color.b += ofRandomf() * kirakiraColorRangeAmt;
    color.g = ofClamp(color.g, 0.0f, 1.0f);
    color.b = ofClamp(color.b, 0.0f, 1.0f);
}
void hoshi::draw(){
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateZ(angle + ofGetElapsedTimef() * 50.0f);
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetColor(color);
    ofLine(hoshiSize, 0.0f, -hoshiSize, 0);
    ofLine(0.0f, hoshiSize, 0.0f, -hoshiSize);
    ofPopMatrix();
    
    counter++;
}

//==================================================================
void Kirakira::update(float x, float y){
    
    if (!bKirakiraDraw) {
        eriaPos.set(x, y);
        exEriaPos.set(x,y);
        return;
    }
    eriaPos.set(x, y);
    
    if (bKirakiraIncrease) {
        for (int i = 0; i < kirakiraAmt; i++) {
            hoshi h;
            float angle = TWO_PI * ofRandomf();
            float dist = kirakiraHankei * fabs(ofRandomf());
            h.pos.set(eriaPos.x + dist * cos(angle), eriaPos.y + dist * sin(angle) * 1.3f);
            hoshis.push_back(h);
        }
    }
    
        vector<hoshi>::iterator it = hoshis.begin();
        while (it != hoshis.end())
        {
            hoshi s = *it;
            if (s.counter > s.lifeTime)
            {
                it = hoshis.erase(it);
            }
            else
                it++;
        }
    
};
void Kirakira::draw(){
    ofPushStyle();
    for (int i = 0; i < hoshis.size(); i++) {
        hoshis[i].draw();
    }
    ofPopStyle();
}
void Kirakira::clear(){

}



//==================================================================
bunshi::bunshi(){
    energy = fabs(ofRandomf()) * energyAmt;
    c = mainPink;
    c *= energy + 0.2;
    //c = ofClamp(c, 0.5f, 1.0f);
    lifeTime = mainBunshiLifeTime * energy;
    radius = mainParticleSize * energy;
    pos.set(0.0f, 0.0f);
}
float bunshi::update(ofPoint kasoku){
    ofPoint zero = ofPoint(0.0f, 0.0f);
    speed += kasoku;
    pos += speed * energy * energyAmt;
    radius += ofRandomf() * radiusAmt;
    radius = ofClamp(radius, radiusMin, radiusMax);
    if (currentMahoTime > lifeTime) {
        c *= alphaAmt;
    }
}
void bunshi::draw(){
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
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
    }
    ofSetColor(c);
    ofCircle(pos.x, pos.y, radius);
}
//////////////
myLine::myLine(float _angle){
    angle = _angle;
    linePoint = ofPoint();
    speed = ofPoint();
    accel = ofPoint();
    alphaSpeed = 0.0f;
}
void myLine::update(ofPoint & _accel){
    accel = _accel;
    speed += accel;//speed no suii
    linePoint += speed;//line no ido
}
void myLine::draw(){
    if (trFadeOutLastMaho) {
        if(c.r > 0)c.r--;
        if(c.g > 0)c.g--;
        if(c.b > 0)c.b--;
    }else c = mainPink + ofRandomf() * lineColorAmt;
    ofSetColor(c);
    ofLine(linePoint.x, 0.0f, linePoint.y, 0.0f);
}
//==================================================================
void noiseBlob::toggleStart(int noiseBlobPointNum){
    blob.clear();
    vecs.clear();
    float anAngle = TWO_PI / noiseBlobPointNum;
    for (int i = 0; i < noiseBlobPointNum; i++) {
        ofPoint p = ofPoint(cos(anAngle * i), sin(anAngle * i));
        vecs.push_back(p);
        blob.push_back(ofPoint(0,0));
    }
    c = mainPink;
    
}
void noiseBlob::update(){
    if (blob.size() == 0) return;
    for (int i = 0; i < blob.size(); i++) {
        blob[i] = vecs[i] * ofRandom(sparkRangeMin, sparkRangeMax);
    }
}
void noiseBlob::draw(){
    if (blob.size() == 0) return;
    ofPushStyle();
    ofPushMatrix();
    ofFill();
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
    }
    ofSetColor(c);
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
}

//==================================================================
void Maho::update(){
    currentMahoTime = ofGetElapsedTimeMillis() - startMahoTime;
    if (bSera) {
        sera.update();
    }
    if (mahoPhase != 0) {
        lines.clear();
        lines2.clear();
        bunshis.clear();
    }
    //phase0
    if (mahoPhase == 0) {
        for (int i = 0; i < bunshis.size(); i++) {
            bunshis[i].update(ofPoint(ofRandomf() * kasokuAmt, ofRandomf() * kasokuAmt));
        }
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
        b2.update();
    }
    if (mahoIndex == 9)lastparticles.update();
}
void Maho::draw( float x, float y){
    if (!drawing) return;
    //phase0
//    if (lastMaho) {
//        float lastMahoLineNum = line1Num *2;
//        int lastMahoBlobPointNum = blobPointNum * 3;
//        float lastMahoSparkRangeMax = 180.0f;
//        float lastMahoLine2MinLength = 180.0f;//2.0f - 300.0f
//        float lastMahoLine2MaxLength = 500.0f;
//        float mahoLine2MaxLength = 110.5f;//
//        ofPushStyle();
//        ofPushMatrix();
//        ofTranslate(x, y);
//        
//        
//        for (int i = 0; i < lines2.size(); i++) {
//            lines2[i].draw();
//        }
//        ofFill();
//        b.draw();
//        b2.draw();
//        for (int i = 0; i < bunshis.size(); i++) {
//            bunshis[i].draw();
//        }
//        ofNoFill();
//        ofSetLineWidth(4.0f);
//        for (int i = 0; i < lines.size(); i++) {
//            ofPushMatrix();
//            ofRotateZ(lines[i].angle);
//            lines[i].draw();
//            ofPopMatrix();
//        }
//        ofPopMatrix();
//        ofPopStyle();
//        ofPushStyle();
//        lastparticles.draw();
//        ofPopStyle();
//        
//        return;
//    }
    if (mahoPhase == 0){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y);
        for (int i = 0; i < lines2.size(); i++) {
            lines2[i].draw();
        }
        ofFill();
        b.draw();
        b2.draw();
        for (int i = 0; i < bunshis.size(); i++) {
            bunshis[i].draw();
        }
        ofNoFill();
        ofSetLineWidth(4.0f);
        for (int i = 0; i < lines.size(); i++) {
            ofPushMatrix();
            ofRotateZ(lines[i].angle);
            lines[i].draw();
            ofPopMatrix();
        }
        ofPopMatrix();
        ofPopStyle();
        ofPushStyle();
        lastparticles.draw();
        ofPopStyle();
        
    } else if (mahoPhase == 1){
        if ( currentMahoTime < keenTime) {//Keen
            ofPushStyle();
            ofPushMatrix();
            ofNoFill();
            ofSetColor(mainPink);
            ofSetLineWidth(keenWidth);
            ofPoint v = mahoPos[1] - mahoPos[0];
            ofLine(mahoPos[0], mahoPos[1]);
            ofPopMatrix();
            ofPopStyle();
        } else if ( currentMahoTime > keenTime && currentMahoTime < mahoPhase2Time[0]) {//本棚がグニャグニャ hondana
            //fade
            if (mahoIndex == 1) {
                //gunya1
                ofPushStyle();
                ofPushMatrix();
                ofFill();
                ofSetColor(color);
                ofBeginShape();
                for (int i = 0; i < HONDANA_INDEX_NUM; i++) {
                    ofVertex(hondanaPoints[i]);
                    hondanaPoints[i] += ofRandomf() * hondanaNoiseAmt;
                }
                ofEndShape();
                ofBeginShape();
                for (int i = HONDANA_INDEX_NUM - 1; i >= 0; i--) {
                    ofVertex(hondanaPoints[i]);
                    hondanaPoints[i] += ofSignedNoise(ofGetElapsedTimeMillis()) * hondanaNoiseAmt;
                }
                ofEndShape();
                ofPopMatrix();
                ofPopStyle();
            } 
        }
        
        
        //sera
        if (currentMahoTime > keenTime && mahoIndex == 2) {//sera
            if (!seraDid) {
                if (choicedXtion == 0) {
                    bDrawSeraBlob = true;
                } else {
                    for (int i = 1; i <= XTION_NUM; i++) {
                        if (choicedXtion - 1 == i){
                            bDraw[i] = true;
                        }else bDraw[i] = false;
                    }
                    bOpenCv = true;
                }
                seraDid = true;
            }
            if (bSera) {
                if (bDrawSeraBlob){
                    ofPushStyle();
                    ofFill();
                    if (toggleCvFadeOut) {
                        if (openCvDrawColor.r > 2)openCvDrawColor.r -= 3;
                        if (openCvDrawColor.g > 2)openCvDrawColor.g -= 3;
                        if (openCvDrawColor.b > 2)openCvDrawColor.b -= 3;
                        if (openCvDrawColor.r < 3) {
                            bDrawSeraBlob = false;
                        }
                    }
                    ofSetColor(openCvDrawColor);
                    
//                    ofBeginShape();
//                    for (int i = 0; i < 6; i++) {
//                        ofVertex(seraBlob[i].x + ofRandomf() * 10.0f, seraBlob[i].y + ofRandomf() * 10.0f);
//                    }
//                    ofEndShape();
                    
//                    ofBeginShape();
//                    for (int i = 0; i < 6; i++) {
//                        ofVertex(seraBlob[i].x + ofRandomf() * 10.0f, seraBlob[i].y + ofRandomf() * 10.0f);
//                    }
//                    ofEndShape();
//                    ofPoint centerOfSera(0.0f, 0.0f);
//                    for (int i = 0; i < 6; i++) {
//                        centerOfSera.x += seraBlob[i].x;
//                        centerOfSera.y += seraBlob[i].y;
//                    }
//                    centerOfSera /= 6.0f;
//                    for (int i = 0; i < 100; i++) {
//                        float length = ofRandom(0.0f, 100.0f);
//                        float angle = ofRandom(0.0f, 360.0f);
//                        float xx = length * cos(angle);
//                        float yy = length * sin(angle);
//                        if (sin(angle) < 0) yy *= 0.5f;
//                        
//                        ofRect(2.0f + centerOfSera.x + xx, 2.0f + centerOfSera.y + yy, 8.0f, 8.0f);
//                    }
                    
                    for (int i = 0; i < 100; i++) {
                        float length = ofRandom(0.0f, 100.0f);
                        float angle = ofRandom(0.0f, 360.0f);
                        float xx = length * cos(angle) * 0.4;
                        float yy = length * sin(angle);
                        if (sin(angle) < 0) yy *= 0.5f;
                        
                        ofRect(4.0f + seraHeadPos[0].x + xx, 4.0f + seraHeadPos[0].y + yy, 8.0f, 8.0f);
                    }
                    
                    for (int i = 0; i < 100; i++) {
                        float length = ofRandom(0.0f, 100.0f);
                        float angle = ofRandom(0.0f, 360.0f);
                        float xx = length * cos(angle) * 0.4;
                        float yy = length * sin(angle);
                        if (sin(angle) < 0) yy *= 0.5f;
                        
                        ofCircle(seraHeadPos[0].x + xx, seraHeadPos[0].y + yy, 8.0f);
                    }

//                    float noiseLineLength = 30.0f;
//                    ofNoFill();
//                    ofSetColor(openCvDrawColor);
//                    ofSetLineWidth(2.0f);
//                    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
//                    for (int i = 0; i < 50; i++) {
//                        ofLine(centerOfSera.x - noiseLineLength /2.0f, centerOfSera.y + ofNoise(ofGetElapsedTimef() * 100 * i)*noiseLineLength,  centerOfSera.x + noiseLineLength /2.0f, centerOfSera.y + ofNoise(ofGetElapsedTimef() * 100 * i)*noiseLineLength);
//                    }
//                    ofDisableBlendMode();
                    
                }
                sera.draw();
            } else {
                sera.toggleGo();
                bSera = true;
            }
        }
        //harada door
        if (currentMahoTime > keenTime && mahoIndex == 3) {
            //gunya1
            ofPushStyle();
            ofFill();
            ofSetColor(color);
            ofBeginShape();
            for (int i = 0; i < 6; i++) {
                ofVertex(doorPos[i]);
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 0; i < 6; i++) {
                ofVertex(doorPosDamy[i]);
                doorPosDamy[i].x += ofRandomf() * doorGunyaAmt;
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 4 - 1; i >= 0; i--) {
                ofVertex(doorPosDamy[i]);
                doorPosDamy[i].x += ofRandomf() * doorGunyaAmt;
            }
            ofEndShape();
            ofPopStyle();
        }
        //harada fire
        if (currentMahoTime > keenTime && mahoIndex == 6) {
            bHaradaFire = true;
            haradaFire.setup();
            haradaFire.toggleGo();
            mahoIndex = 0;
        }
        //sushi
        if (currentMahoTime > keenTime && mahoIndex == 7) {
            //rokka-
            ofPushStyle();
            ofPushMatrix();
            ofFill();
            ofSetColor(color);
            ofBeginShape();
            for (int i = 0; i < 4; i++) {
                ofVertex(rokkaPos[i]);
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 0; i < 4; i++) {
                ofVertex(rokkaPosDamy[i].x + ofRandomf() * hondanaNoiseAmt, rokkaPosDamy[i].y + ofRandomf() * hondanaNoiseAmt);
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 3; i >= 0; i--) {
                ofVertex(rokkaPosDamy[i].x + ofSignedNoise(ofGetElapsedTimeMillis()) * hondanaNoiseAmt, rokkaPosDamy[i].y + ofSignedNoise(ofGetElapsedTimeMillis()) * hondanaNoiseAmt);
            }
            ofEndShape();
            ofPopMatrix();
            ofPopStyle();
        }
        //gomory
        if (currentMahoTime > keenTime && mahoIndex == 8) {
            ofPushStyle();
            ofPushMatrix();
            ofFill();
            ofSetColor(color);
            ofBeginShape();
            for (int i = 0; i < 6; i++) {
                ofVertex(doorPos[i]);
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 0; i < 6; i++) {
                ofVertex(doorPosDamy[i]);
                doorPosDamy[i].x += ofRandomf() * doorGunyaAmt;
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 4 - 1; i >= 0; i--) {
                ofVertex(doorPosDamy[i]);
                doorPosDamy[i].x += ofRandomf() * doorGunyaAmt;
            }
            ofEndShape();
            ofPopMatrix();
            ofPopStyle();
        }

        
    }
    
}

void Maho::toggleSpark(){
    mahoPhase = 0;
    fadeCount = 0;
    drawing = true;
    color = mainPink;
    if (bLine1) {
        float anAngle = 360.0f / line1Num;
        float firstAngle = ofGetElapsedTimef() * 360.0f;
        for (int i = 0; i < line1Num; i++) {
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
            myLine2 l = myLine2(aAm * i * ofRandom(0.0f, 30.0f));
            lines2.push_back(l);
        }
    }
    if (bBlob){
        b.toggleStart(blobPointNum);
        b2.toggleStart(blobPointNum);
    }
    if (mahoIndex == 2) {
        bSera = false;
        sera.setup();
    }
    printf("goko1発射!!\n");
    seraDid = false;
}
void Maho::toggleKeen(){
    if (mahoPhase != 0) return;
    startMahoTime = ofGetElapsedTimeMillis();
    mahoPhase = 1;
}
