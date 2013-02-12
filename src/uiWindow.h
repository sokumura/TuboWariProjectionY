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

#define OFXMSAGUI_DONT_USE_XML

#define MONITOR_SIZE_W 1024
#define MONITOR_SIZE_H 768

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
    void screenFboDraw(int page, int blockNo, int blockNumByWidth);
    
};
