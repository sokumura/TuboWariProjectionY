//
//  myDepthGenerator.h
//  xtionExample
//
//  Created by Shuya Okumura on 12/10/03.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

/*
 DepthGenerator 1つが行う処理については全てここに書く
 入出力に関しては、距離の数値を色の数値に変換しない。
 */

#pragma once

#include "XnCppWrapper.h"
#include "ofMain.h"

#define TOTAL_PIXEL 640*480
#define XTION_NUM 1

struct soDepthThresholds {
    int near = 3000;
    int far = 5000;
    int min = 0;
    int max = 8000;
};

struct rot{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

class myDepthGenerator {
public:
    
    myDepthGenerator();
    ~myDepthGenerator();
	bool setup(xn::NodeInfo const& node, int num);
    void startGenerating();
    bool update(soDepthThresholds thresholds);
    void draw(ofVec3f & pos, float degx, float degy, float degz);
    
    //options
    int               getNumber() const { return xtionNum;};
    
    bool              bUpdateMasks;
    XnMapOutputMode&  getMapMode();
    
    
    //背景のキャプチャ系
    void freeBgDepth();
    bool bBgDepth;
    int capturePlay;
    unsigned int getCaptureCount() const;
    void setCapturedCount();
    void runCapture();//
    
    
    void  generateTexture();
    void  generateMonoTexture();
    void  generateCurrentDepth();
    
    const unsigned char * getMonitorTexture() const;
    const unsigned char * getMonoTexture() const;
    const XnDepthPixel * getDistanceTexture() const;
    const vector<ofVec3f> getDepthVectors() const {
        return depthVecs;
    };
    void setStep(int s){
        step = s;
    };
    void transformVectors(ofVec3f pos);
    
//    ofVec3f localAxis;
//    rot localRot;
//    ofVec3f lightPos;
    float zThresholdNear, zThresholdFar;
    
    int counter;
    
    
    
private:
    myDepthGenerator(const myDepthGenerator& other);
	myDepthGenerator& operator = (const myDepthGenerator&);
    
    
    //new
    ofNode camTarget;
    ofLight light;
    float camDistance, exCamDistance;
    float pointSize;
    int step;
    
    vector <ofVec3f> depthVecs;
    ofVboMesh vboMesh;
    
    int w, h;
    
    void console(bool bOut);
    
    xn::DepthGenerator      depth_generator;
    xn::DepthMetaData       dmd;
    XnMapOutputMode         out_put_modes;
    
    void planeBgCapthre();
    void captureBgDepth();
    
    void generateVectors();
    
    XnDepthPixel bgDepth[TOTAL_PIXEL];//背景のキャプチャ
    unsigned char mono_texture[TOTAL_PIXEL];//thresholdに含まれる点を255,それ以外を0にしたtex
    unsigned char monitor_texture[TOTAL_PIXEL *4];//全体を0~255にmapしたtex
    XnDepthPixel currentDepth[TOTAL_PIXEL];//thresholdの中に入ったものだけにした生データ
    
    
    XnDepthPixel depthMIN, depthMAX;//デプスのmin,maxをいれておく
    soDepthThresholds privThresholds;//このxtionのthresholds
    int xtionNum;//このxtionの番号
    
    unsigned int bgCaptureCount, totalPixel;
	
};

