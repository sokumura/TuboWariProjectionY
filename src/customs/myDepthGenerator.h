//
//  myDepthGenerator.h
//  xtionExample
//
//  Created by Shuya Okumura on 12/10/03.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

/*
 DepthGenerator 1つが行う処理については全てここに書く。
 */

#pragma once

#include "ofxOpenNIHeaders.h"
#include "XnCppWrapper.h"
#include "ofMain.h"

#define XTION_NUM       2
#define CAPTURE_WIDTH   XN_QVGA_X_RES
#define CAPTURE_HEIGHT  XN_QVGA_Y_RES
#define TOTAL_PIXEL     CAPTURE_WIDTH * CAPTURE_HEIGHT
#define PROJECTION_SIZE_W   1024
#define PROJECTION_SIZE_H   768

#define  USE_RECORED_DATA    false
#define  DO_RECORED          true

extern bool bDraw[XTION_NUM];
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
extern float depthBgAverage[XTION_NUM];
extern int realDepthMax[XTION_NUM];
extern bool trLoadBg[XTION_NUM];

extern float pointSize;
extern int step;

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
    void  freeBgDepth();
    void  generateTexture();
    void  generateCurrentDepth();
    
    xn::DepthGenerator & getXnDepthGenerator(){
        return depth_generator;
    };
    
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
    xn::DepthGenerator      depth_generator;
    xn::DepthMetaData       dmd;
    XnMapOutputMode         out_put_modes;
    
    void checkSwitchMethods();
    void saveBgImage();
    bool loadBgImage();
    
    void planeBgCapthre();

    unsigned short bgDepth[TOTAL_PIXEL];//背景のキャプチャ
    unsigned char captureBgCountByPix[TOTAL_PIXEL];
    unsigned char monitor_texture[TOTAL_PIXEL *4];//全体を0~255にmapしたtex
    XnDepthPixel currentDepth[TOTAL_PIXEL];//thresholdの中に入ったものだけにした生データ
    
    XnDepthPixel depthMIN, depthMAX;//デプスのmin,maxをいれておく
    int thisNum;//このxtionの番号
    
    int counter;
	
};

