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
#include "ofxCv.h"
#include "ofMain.h"

using namespace ofxCv;
using namespace cv;

#define XTION_NUM       1
#define CAPTURE_WIDTH   XN_QVGA_X_RES //XN_VGA_X_RES
#define CAPTURE_HEIGHT  XN_QVGA_Y_RES //XN_VGA_Y_RES
#define TOTAL_PIXEL     CAPTURE_WIDTH * CAPTURE_HEIGHT

extern int bgCapturePlay[XTION_NUM];
extern int thresholdNear[XTION_NUM];
extern int thresholdFar[XTION_NUM];
extern bool bUseBgDepth[XTION_NUM];
extern float scale[XTION_NUM];
extern float aspect[XTION_NUM];
extern float scaleZ[XTION_NUM];
extern float rotx[XTION_NUM];//5
extern float roty[XTION_NUM];//6
extern float rotz[XTION_NUM];//7
extern ofVec3f axis[XTION_NUM];//8
extern bool bCaptureBg[XTION_NUM];
extern bool bSaveBgAsImage[XTION_NUM];

extern float pointSize;
extern int step;

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
    bool update();
    void draw();
    
    //options
    int               getNumber() const { return thisNum;};
    
    
    //背景のキャプチャ系
    void freeBgDepth();
    unsigned int getCaptureCount() const;
    void setCapturedCount();
    void runCapture();//
    
    void  generateTexture();
    void  generateCurrentDepth();
    
    const unsigned char * getMonitorTexture() const;
    const XnDepthPixel * getDistanceTexture() const;
    const vector<ofVec3f> getDepthVectors() const {
        return depthVecs;
    };
    void generateRealWorld( XnPoint3D * p3d);
    XnPoint3D realWorld[TOTAL_PIXEL];
    
    unsigned char * getPointerOfUnsignedChar(){
        unsigned char * pix = (unsigned char * )currentDepth;
        return pix;
    };
    
    void console(bool bOut);
    
private:
    myDepthGenerator(const myDepthGenerator& other);
	myDepthGenerator& operator = (const myDepthGenerator&);
    
    //new
    
    vector <ofVec3f> depthVecs;
    ofVboMesh vboMesh;
    
    ofShortImage bgImg;
    
    bool loadBgImage();
    
    xn::DepthGenerator      depth_generator;
    xn::DepthMetaData       dmd;
    XnMapOutputMode         out_put_modes;
    
    
    void planeBgCapthre();
    void generateVectors();
    void saveBgImage();
    void contourFinderSetup();
    
    unsigned short bgDepth[TOTAL_PIXEL];//背景のキャプチャ
    unsigned char captureBgCount[TOTAL_PIXEL];
    unsigned char captureCount;
    unsigned char monitor_texture[TOTAL_PIXEL *4];//全体を0~255にmapしたtex
    XnDepthPixel currentDepth[TOTAL_PIXEL];//thresholdの中に入ったものだけにした生データ
    
    XnDepthPixel depthMIN, depthMAX;//デプスのmin,maxをいれておく
    int thisNum;//このxtionの番号
    int counter;
    //unsigned int bgCaptureCount;
	
};

