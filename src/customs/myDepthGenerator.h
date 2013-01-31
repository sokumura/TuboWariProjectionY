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

#define XTION_NUM       1
#define CAPTURE_WIDTH   XN_QVGA_X_RES //XN_VGA_X_RES
#define CAPTURE_HEIGHT  XN_QVGA_Y_RES //XN_VGA_Y_RES
#define TOTAL_PIXEL     CAPTURE_WIDTH * CAPTURE_HEIGHT

extern int thresholdNear[XTION_NUM];
extern int thresholdFar[XTION_NUM];
extern float scale[XTION_NUM];
extern float aspect[XTION_NUM];
extern float scaleZ[XTION_NUM];
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
    void draw(ofVec3f & pos, float degx, float degy, float degz);
    
    //options
    int               getNumber() const { return thisNum;};
    
    //背景のキャプチャ系
    void freeBgDepth();
    bool bBgDepth;
    int capturePlay;
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
    
    int counter;
    
    unsigned char * getPointerOfUnsignedChar(){
        const XnDepthPixel * depth = dmd.Data();
        XN_ASSERT(depth);
        if (dmd.FrameID() == 0) return;
        
        unsigned char * pix = (unsigned char * )depth;
        return pix;
    };
    
private:
    myDepthGenerator(const myDepthGenerator& other);
	myDepthGenerator& operator = (const myDepthGenerator&);
    
    //new
    
    vector <ofVec3f> depthVecs;
    ofVboMesh vboMesh;
    
    ofShortImage img;
    
    void console(bool bOut);
    
    xn::DepthGenerator      depth_generator;
    xn::DepthMetaData       dmd;
    XnMapOutputMode         out_put_modes;
    
    
    void planeBgCapthre();
    void generateVectors();
    
    unsigned short bgDepth[TOTAL_PIXEL];//背景のキャプチャ
    unsigned char captureBgCount[TOTAL_PIXEL];
    unsigned char captureCount;
    
    unsigned char monitor_texture[TOTAL_PIXEL *4];//全体を0~255にmapしたtex
    XnDepthPixel currentDepth[TOTAL_PIXEL];//thresholdの中に入ったものだけにした生データ
    
    XnDepthPixel depthMIN, depthMAX;//デプスのmin,maxをいれておく
    int thisNum;//このxtionの番号
    
    unsigned int bgCaptureCount, totalPixel;
	
};

