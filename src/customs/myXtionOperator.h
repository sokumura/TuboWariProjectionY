//
//  myXtionOperator.h
//  xtionExample
//
//  Created by 奥村 周也 on 12/10/05.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

/*
 xtionの制御は基本的にこのクラスから一行で呼び出す
 */

#ifndef xtionExample_myXtionOperator_h
#define xtionExample_myXtionOperator_h


#include "ofxOpenNIHeaders.h"
#include "XnCppWrapper.h"
#include "myDepthGenerator.h"



extern float rotx[XTION_NUM];
extern float roty[XTION_NUM];
extern float rotz[XTION_NUM];
extern ofVec3f axis[XTION_NUM];


class myXtionOperator {
public:
    
    
    myXtionOperator();
    ~myXtionOperator();
    
    void setup();
    void update();
    void testDraw();
    void customEnumerateProductionTrees(xn::Context& con, XnProductionNodeType type);
    
    //ofVboMesh
    
    myDepthGenerator& getDepthGenerator(int index);
    int counter;
    

private:
    myXtionOperator(const myXtionOperator& other);
    myXtionOperator& operator = (const myXtionOperator&);
    
    ofxOpenNIContext context;
    myDepthGenerator depth_GRs[XTION_NUM];
    bool bNewDataXtion[XTION_NUM];
//    soDepthThresholds thresholds[XTION_NUM];
    int generatorNum;
    
    xn::EnumerationErrors errors;
    void logErrors(xn::EnumerationErrors& rErrors);

};

#endif
