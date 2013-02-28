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
#include "Maho.h"
#include "grotes.h"
#include "lastParticles.h"

//class monitorMessage {
//public:
//    void addMessage(int pageNo, string message, ofColor color);
//    void draw(int pageNo);
//private:
//    char mMessage[3][64];
//    vector<int> mMessagePage;
//    float messageHeight;
//};


class uiWindow : public ofxFensterListener {
public:
    uiWindow();
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    void ope1Setup();
    void ope2Setup();
    void ope3Setup();
    void ope4Setup();
    void ope5Setup();
    void ope6Setup();
    void ope7Setup();
    void ope8Setup();
    void ope9Setup();
    void ope11Setup();
    
    int counter;
    void addMappingSetup();
    ofTexture screenTex;
    
    bool trDrawGlobalMap = false;
    ofPoint screenTexPoint;
    ofPoint screenTexWH;
    
    void consoleOut();

    
private:
    void monitorFboDraw(int controlNoFrom1,int blockNoFrom1, int blockNumByWidth, float scale);
    void taskToggle(int scene, int & phase);
    
    bool exBcfinderDraw;
    bool exBdrawMesh;
    
    int phaseNoByScene[SCENE_SIZE];
    int currentScene;
};
