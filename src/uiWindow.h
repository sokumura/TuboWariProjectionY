//
//  uiWindow.h
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/01/26.
//
//

#pragma once

#include "ofMain.h"
#include "ofxFensterManager.h"
#include "ofxSimpleGuiToo.h"
#include "myXtionOperator.h"
#include "ofxOpenCv.h"
#include "magicTest.h"


class uiWindow : public ofxFensterListener {
public:
    uiWindow();
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    int counter;
    
    void mySaveToXml();
    void myLoadFromXml();
    
    void consoleOut();
    
    ofxXmlSettings xml;
    
    void addMappingSetup();
    ofTexture screenTex;
    
    bool trDrawGlobalMap = false;
    ofPoint screenTexPoint;
    ofPoint screenTexWH;
    
    
private:
    void monitorFboDraw(int pageFrom1, int controlNoFrom1,int blockNoFrom1, int blockNumByWidth, float scale);
    
};
