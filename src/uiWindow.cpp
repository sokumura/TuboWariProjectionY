//
//  uiWindow.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/01/26.
//
//

#include "uiWindow.h"

//////////////////
//////GLOBAL//////
bool bTail = false;
bool trTail = false;
vector <ofPoint> messagePoint;
ofFloatColor mainPink = ofFloatColor(1.0f, 0.44f, 0.82f, 1.0f);
int currentScene = 0;
//main
ofFbo screenFbo;
ofFbo monitorFbo;
ofPoint point1 = ofPoint((float)PROJECTION_SIZE_W / 2, (float)PROJECTION_SIZE_H);
ofPoint point2 = ofPoint((float)PROJECTION_SIZE_W / 2, (float)PROJECTION_SIZE_H);
//grotesq
int grotesParticlesNum = 5000;
bool bGrotesq = false;
grotes grotesq;
bool bHaradaFire = false;
grotes haradaFire;
bool bHaradaFireFadeOut = false;
float haradaFireFadeOutAmt = 1.0f;

//xtions
ofFbo xtionFbo;
myXtionOperator xtions;
ofTexture tex[XTION_NUM];
bool bSetUp = false;
bool bShutDown = false;
bool trSaveDepthByFrame[XTION_NUM] = { false};//new!
bool trStopSaveDepthByFrame[XTION_NUM] = {false};
//openCv
bool bOpenCv = true;
ofColor openCvDrawColor = mainPink;
bool toggleCvFadeOut = false;
bool bCfinderDraw = true;
ofxCvContourFinder cfinder;
int gImgThreshold = 0;
bool bCvBlur = true;
bool bCvNoise = false;
float cvNoiseAmt = 40.0f;
bool bDrawScene1Blob = false;
bool bSeraBlobMappingDraw = false;
bool bSeraHeadMappingDraw = false;
ofPoint seraHeadPos[1] = {ofPoint(759.0f, 548.0f)};

bool bUpdateXtion[XTION_NUM] = {true};
bool bDraw[XTION_NUM] = {true, false, false};
int thresholdNear[XTION_NUM];//2
int thresholdFar[XTION_NUM];//3

float rotx[XTION_NUM] = {-10.8, 0.0f, -45.0f};//5
float roty[XTION_NUM] = {-59.4f, 0.0f, -9.0f};//6
float rotz[XTION_NUM] = {-21.6f, 0.0f, -12.6f};//7
ofVec3f axis[XTION_NUM] = {ofVec3f(2400.0f, 1920.0f, -5000.0f), ofVec3f(), ofVec3f(3680.0f, 320.0f, -4950.0f)};//8
float scale[XTION_NUM] = {0.85f, 1.0f, 0.90f};
float aspect[XTION_NUM] = {1.115f, 1.0f, 0.9875f };
float scaleZ[XTION_NUM] = {0.884f, 1.0f, 0.623f};
int bgCapturePlay[XTION_NUM] = {20};//1
bool bCaptureBg[XTION_NUM] = {false};
bool bUseBgDepth[XTION_NUM] = {false};//4
int depthSlider[XTION_NUM] = { 10000 };
bool trLoadBg[XTION_NUM] = {false};
bool bSaveBgAsImage[XTION_NUM] = {false};
bool trGetPointDepth[XTION_NUM] = {true};

bool bKirakiraIncrease = true;

ofPoint depthCheckPoint[XTION_NUM] = {ofPoint(0.0f, 0.0f)};///
ofPoint depthCheckPoint2[XTION_NUM] = {ofPoint((float)CAPTURE_WIDTH, (float)CAPTURE_HEIGHT)};
int depthPointValue[XTION_NUM] = {0};///

bool bDrawMesh = false;
float pointSize = 4.0;//9
int step = 2;//10
int minArea = 50;//11
int maxArea = 500000;//12
int nConsidered = 10;//13
bool bFindHoles = true;//14
bool bUseApproximation = true;//15
int choicedXtion = 0;

//point of mapping
bool bMappingDraw = false;
bool bMappingDrawOnMonitor = true;
ofPoint gekijo[8] = {
    ofPoint(13.0f,81.5f),
    ofPoint(1267.0f,80.0f),
    ofPoint(1344.0f, 864.0f),
    ofPoint(-68.0f, 879.0f),
    ofPoint(204.0f,88.0f),
    ofPoint(1077.0f,86.0f),
    ofPoint(1056, 627.0f),
    ofPoint(221.0f, 634.0f)};//間口

ofPoint rokkaPos[4] = {ofPoint(1022.0f, 480.0f),
    ofPoint(1132.0f, 475.0f),
    ofPoint(1114.0f, 688.0f),
    ofPoint(1005.0f, 688.0f)
};
ofPoint rokkaPosDamy[4];
bool bRokkaDraw = false;

float lineWidth = 4.0f;

ofPoint targetPointA = ofVec3f(PROJECTION_SIZE_W / 2, PROJECTION_SIZE_H / 2);
bool bDoorDraw = false;
ofPoint doorPos[6] = {ofPoint(1228.0f,490.0f),
    ofPoint(1364.0f, 572.0f),
    ofPoint(1315.0f, 673.0f),
    ofPoint(1333.0f, 892.0f),
    ofPoint(1218.0f, 757.0f),
    ofPoint(1230.0f, 688.0f)};

ofPoint doorPosDamy[6];

/////////maho////////////
bool trFadeOutLastMaho = false;
int startMahoTime, currentMahoTime = 0;

ofPoint hondanaPoints[HONDANA_INDEX_NUM] = {ofPoint(41.0f, 464.0f),
                                            ofPoint(222.0f, 482.0f),
                                            ofPoint(224.0f, 718.0f),
                                            ofPoint(-6.0f, 905.0f),
                                            ofPoint(-196.0f, 703.0f),
                                            ofPoint(-250.0f, 555.0f)
};
float hondanaNoiseAmt = 15.0f;

int mahoIndex = 0;
int keenTime = 300;
int mahoPhase2Time[7] = {3000};
ofPoint mahoPos[2] = {ofPoint()};

int mahoPhase = 0;
bool trMaho = false;
//sera
ofPoint seraPos[1] = {ofPoint(512.0f, 480.0f)};
bool bSera = false;
float seraParticleKasokuAmt = 20.0f;
float seraParticleRadius = 6.0f;
ofPoint seraBlob[6] = {ofPoint(720.0f, 405.0f),
    ofPoint(858.0f, 407.0f),
    ofPoint(897.0f, 500),
    ofPoint(856.0f, 603.0f),
    ofPoint(718.0f, 605.0f),
    ofPoint(692.0f, 493.0f),};
bool bDrawSeraBlob = false;
//harada door
float doorGunyaAmt = 5.0f;

//line
bool bLine1 = true;
int line1Num = 200;
float accelAmt = 98.75f;
float lineColorAmt = 0.35f;
//bunshi
bool bBunshi = false;
int bunshiNum = 55;//10 - 1000;
float mainParticleSize = 5.0f;
float radiusAmt = 0.4f;//0.1 - 3.0f;
float radiusMin = 1.6f;
float radiusMax = 2.8f;
float energyAmt = 0.5f;//1-10;
float kasokuAmt = 22.0f;//1.0 - 100.0f
float alphaAmt = 0.785f;
int mainBunshiLifeTime = 1445;
//line2
bool bLine2 = true;
int line2Num = 22;
float mahoLine2MinLength = 34.5f;//2.0f - 300.0f
float mahoLine2MaxLength = 110.5f;//
float lengthNoiseAmt = 16.4f;
float line2speed = 0.816f;//0 - 1
float mahoLine2AngleAmt = 0.30f;//0 - 60
float line2width = 2.5f;
//blobs
bool bBlob = true;
int blobPointNum = 33;//1 - 100 = 10
float sparkRangeMin = 10;//0 - 1000
float sparkRangeMax = 60;
//keen
bool trKeen = false;
float keenWidth = 4.8f;
ofPoint keenStart = ofPoint(500.0f, 500.0f);
ofPoint keenGoal = ofPoint(1000.0f, 1000.0f);
//kirakira
bool bKirakiraDraw = false;
ofPoint kirakiraPos[1] = {ofPoint(1000.0f, 500.0f)};
float hoshiSize = 50.0f; //5.0f
int   hoshiMainLifeTime = 32;//100 - 10000
float kirakiraHankei = 175.0f;
int kirakiraAmt = 100;
float kirakiraColorRangeAmt = 0.015;//0.0f - 0.1

//chirachira
ofPoint chirachiraPos[1] = {ofPoint(0,0)};
bool bHondanaDraw = false;

//videos
ofQTKitPlayer himawari;
ofQTKitPlayer kaji;
ofPoint himawariPos = ofPoint(100.0f, 270.0f);
ofPoint kajiPos = ofPoint(0.0f, 0.0f);
bool trPlayHimawariVideo = false;
bool bHimawariVideoPlaying = false;
bool bHimawariAlphaStart = false;
bool trHimawariClear = false;


bool trPlayKajiVideo = false;
bool bKajiVideoPlaying = false;
bool bKajiAlphaStart = false;
bool trKajiClear = false;
bool trHimawariLoad = false;
bool trKajiLoad = false;
int fadeTimeMilis = 1000;//1000 - 10000

//custom testApp's
extern char fadeWhite;
extern char fadeMilli;
extern int WhiteFading;
extern int milli;
extern int FadingStartTime;
bool bBackWhite = false;

//setting
bool bMaskScreen = true;


lastParticles lastparticles;


//グローバルの初期化
uiWindow::uiWindow(){
    for (int i = 0; i < XTION_NUM; i++) {
        tex[i].allocate(CAPTURE_WIDTH, CAPTURE_HEIGHT, GL_RGB);
    }
    for (int i = 0; i < SCENE_SIZE; i++) {
        phaseNoByScene[i] = 0;
    }
//    for (int i = 0; i < 6; i++) {
//        seraBlob[i].y += 50.0f;
//    }

}



void uiWindow::setup(){
    //header
    gui.setup("OPE 1");
    //gui.setTextColorParadox(true);
    gui.headerPage->addToggle("Map Draw onMonitor", bMappingDraw).setNewColumn(true).setFix(true);
    gui.headerPage->addToggle("Map Draw onMonitor", bMappingDrawOnMonitor).setNewColumn(true);
    gui.headerPage->addToggle("bOpenCv", bOpenCv);
    gui.headerPage->addToggle("bMaskScreen", bMaskScreen);
    //page 1 ope1========================
    gui.addMulti2dSlider("OPE 1", 4, 1, kirakiraPos, 0.0f, (float)PROJECTION_SIZE_W, 0.0f, (float)PROJECTION_SIZE_H, 4.0f / 3.0f, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addComboBox("chiceXtion", choicedXtion, XTION_NUM + 1).setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[1]);
    gui.addSlider("cvNoiseAmt", cvNoiseAmt, 0.0f, 300.0f);
    gui.addToggle("bCvNoise", bCvNoise);
    gui.addToggle("bKirakiraDraw", bKirakiraDraw);
    gui.addSlider("hoshiSize", hoshiSize, 0, 50);
    gui.addSlider("ColorRangeAmt", kirakiraColorRangeAmt, 0.0f, 0.5f);
    gui.addSlider("hoshiMainLifeTime", hoshiMainLifeTime, 5, 100);
    gui.addSlider("kirakiraHankei", kirakiraHankei, 0.0f, 1000.0f);
    gui.addSlider("kirakiraAmt", kirakiraAmt, 0, 100);
    gui.addToggle("cFinderDraw", bCfinderDraw);
    gui.addToggle("bCvBlur", bCvBlur);
    gui.addSlider("gImgThreshold", gImgThreshold, 0, 255);
    
    //page 2 ope2 =========================
    gui.addPage("OPE 2");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[2]).setNewColumn(true);
    gui.addButton("toggle maho", trMaho);
    gui.addButton("Keen!!", trKeen);
    gui.addSlider("keenTime", keenTime, 0, 700);
    gui.addToggle("bLine1", bLine1);
    gui.addToggle("bBunshi", bBunshi);
    gui.addToggle("bLine2", bLine2);
    gui.addToggle("bBlob", bBlob);
    gui.addTitle("himawari").setNewColumn(true);
    gui.addSlider2d("himawariPos", himawariPos, -200.0f, 1280.0f + 200.0f, -200.0f, 960.0f + 200.0f);
    gui.addButton("trPlayHimawariVideo", trPlayHimawariVideo);
    gui.addToggle("bHimawariVideoPlaying", bHimawariVideoPlaying).setFix(true);
    gui.addToggle("bHimawariAlphaStart", bHimawariAlphaStart);
    gui.addButton("trHimawariClear", trHimawariClear);
    gui.addButton("trHimawariLoad", trHimawariLoad).setFix(true);
    
    //page3 ope3 maho opencv sera======================
    gui.addPage("OPE 3");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addComboBox("chiceXtion", choicedXtion, XTION_NUM + 1).setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[3]);
    gui.addButton("toggle maho", trMaho);
    gui.addButton("Keen!!", trKeen);
    gui.addSlider("keenTime", keenTime, 0, 700);
    gui.addSlider("ParticleKasokuAmt", seraParticleKasokuAmt,5.0f, 30.0f);
    gui.addSlider("seraParticleRadius", seraParticleRadius, 0.5, 20.0f);
    gui.addToggle("bLine1", bLine1);
    gui.addToggle("bBunshi", bBunshi);
    gui.addToggle("bLine2", bLine2);
    gui.addToggle("bBlob", bBlob);
    gui.addToggle("cFinderDraw", bCfinderDraw);
    gui.addSlider("Mesh_PointSize", pointSize, 0.001f, 10.0f);
    gui.addToggle("bCvBlur", bCvBlur);
    gui.addSlider("gImgThreshold", gImgThreshold, 0, 255);
    gui.addToggle("bCvNoise", bCvNoise);
    gui.addSlider("cvNoiseAmt", cvNoiseAmt, 0.0f, 300.0f);
    
    //page4 ope4 maho opencv door harada======================
    gui.addPage("OPE 4");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[4]).setNewColumn(true);
    gui.addButton("toggle maho", trMaho);
    gui.addButton("Keen!!", trKeen);
    gui.addSlider("keenTime", keenTime, 0, 700);
    gui.addToggle("bLine1", bLine1);
    gui.addToggle("bBunshi", bBunshi);
    gui.addToggle("bLine2", bLine2);
    gui.addToggle("bBlob", bBlob);
    gui.addToggle("cFinderDraw", bCfinderDraw).setNewColumn(true);
    gui.addSlider("Mesh_PointSize", pointSize, 0.001f, 10.0f);
    gui.addToggle("bCvBlur", bCvBlur);
    gui.addSlider("gImgThreshold", gImgThreshold, 0, 255);
    gui.addToggle("bCvNoise", bCvNoise);
    gui.addSlider("cvNoiseAmt", cvNoiseAmt, 0.0f, 300.0f);
    
    //page5 ope5 grotesq 1======================
    gui.addPage("OPE 5");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[5]).setNewColumn(true);
    
    //page6 ope6 maho harada fire======================
    gui.addPage("OPE 6");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[6]).setNewColumn(true);
    gui.addButton("toggle maho", trMaho);
    gui.addButton("Keen!!", trKeen);
    gui.addSlider("keenTime", keenTime, 0, 700);
    gui.addSlider("grotesParticlesNum", grotesParticlesNum, 0, 10000);
    gui.addToggle("bHaradaFireFadeOut", bHaradaFireFadeOut);
    gui.addSlider("haradaFireFadeOutAmt", haradaFireFadeOutAmt, 0.0f, 100.0f);
    gui.addToggle("bLine1", bLine1);
    gui.addToggle("bBunshi", bBunshi);
    gui.addToggle("bLine2", bLine2);
    gui.addToggle("bBlob", bBlob);
    gui.addToggle("cFinderDraw", bCfinderDraw).setNewColumn(true);
    gui.addSlider("Mesh_PointSize", pointSize, 0.001f, 10.0f);
    gui.addToggle("bCvBlur", bCvBlur);
    gui.addSlider("gImgThreshold", gImgThreshold, 0, 255);
    gui.addToggle("bCvNoise", bCvNoise);
    gui.addSlider("cvNoiseAmt", cvNoiseAmt, 0.0f, 300.0f);
    
    //page7 ope7 maho sushi======================
    gui.addPage("OPE 7");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[7]).setNewColumn(true);
    gui.addButton("toggle maho", trMaho);
    gui.addButton("Keen!!", trKeen);
    gui.addSlider("keenTime", keenTime, 0, 700);
    
    //page8 ope8 gomory=====================
    gui.addPage("OPE 8");
    gui.addMulti2dSlider("mahoPos", 4, 2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addValueMonitor("Phase", phaseNoByScene[7]).setNewColumn(true);
    gui.addButton("toggle maho", trMaho);
    gui.addButton("Keen!!", trKeen);
    gui.addSlider("keenTime", keenTime, 0, 700);
    
    //page9 ope9======================
    gui.addPage("OPE 9");
    gui.addMulti2dSlider("lastMaho", 4,2, mahoPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addButton("toggle maho", trMaho).setNewColumn(true);
    gui.addToggle("trFadeLastMaho", trFadeOutLastMaho);
    gui.addTitle("line");
    gui.addToggle("bLine1", bLine1);
    gui.addSlider("lineNumber", line1Num, 0, 500);
    gui.addSlider("accelAmount", accelAmt, 0.0f, 200.0f);
    gui.addSlider("alphaColorAmt", lineColorAmt, 0.0f, 1.0f);
    gui.addTitle("bunshis");
    gui.addToggle("bBunshi", bBunshi);
    gui.addSlider("bunshiNum", bunshiNum, 10, 1000);
    gui.addSlider("maniParticleSize", mainParticleSize, 0.1, 10.0f);
    gui.addSlider("radiusAmt", radiusAmt, 0.0f, 1.0f);
    gui.addRangeSlider("radiusRenge", radiusMin, radiusMax, 0.0f, 40.0f);
    gui.addSlider("energyAmt", energyAmt, 0.0f, 5.0f);
    gui.addSlider("kasokuAmt", kasokuAmt, 0.0f, 100.0f);
    gui.addSlider("alphaAmt", alphaAmt, 0.0f, 1.0f);
    gui.addSlider("mainLifeTime", mainBunshiLifeTime, 500, 4000);
    gui.addTitle("line2");
    gui.addToggle("bLine2", bLine2);
    gui.addSlider("line2Num", line2Num, 0, 100);
    gui.addSlider("line2Width", line2width, 0.0f, 20.0f);
    gui.addRangeSlider("lenth_range", mahoLine2MinLength, mahoLine2MaxLength, 0.0, 300.0f);
    gui.addSlider("lengthNoiseAmt", lengthNoiseAmt, 2.0f, 50.0f);
    gui.addSlider("line2Speed", line2speed, 0.0f, 1.0f);
    gui.addSlider("angleAmt", mahoLine2AngleAmt, 0.0f, 20.0f);
    gui.addTitle("blob");
    gui.addToggle("bBlob", bBlob);
    gui.addSlider("pointNum", blobPointNum, 1, 100);
    gui.addRangeSlider("sparkRange", sparkRangeMin, sparkRangeMax, 0.0f,200.0f);
    
    //page10 ope10===============================
    gui.addPage("OPE 10 LastMAHO");
    gui.addTitle("line");
    gui.addToggle("bLine1", bLine1);
    gui.addSlider("lineNumber", line1Num, 0, 500).setFix(true);
    gui.addSlider("accelAmount", accelAmt, 0.0f, 200.0f).setFix(true);
    gui.addSlider("alphaColorAmt", lineColorAmt, 0.0f, 1.0f);
    gui.addColorPicker("color1", mainPink).setFix(true);
    //bunshis
    gui.addBlank();
    gui.addTitle("bunshis");
    gui.addToggle("bBunshi", bBunshi);
    gui.addSlider("bunshiNum", bunshiNum, 10, 1000).setFix(true);
    gui.addSlider("maniParticleSize", mainParticleSize, 0.1, 10.0f).setFix(true);
    gui.addSlider("radiusAmt", radiusAmt, 0.0f, 1.0f).setFix(true);
    gui.addRangeSlider("radiusRenge", radiusMin, radiusMax, 0.0f, 40.0f).setFix(true);
    gui.addSlider("energyAmt", energyAmt, 0.0f, 5.0f).setFix(true);
    gui.addSlider("kasokuAmt", kasokuAmt, 0.0f, 100.0f).setFix(true);
    gui.addSlider("alphaAmt", alphaAmt, 0.0f, 1.0f).setFix(true);
    gui.addSlider("mainLifeTime", mainBunshiLifeTime, 500, 4000).setFix(true);
    //line2
    gui.addBlank();
    gui.addTitle("line2");
    gui.addToggle("bLine2", bLine2);
    gui.addSlider("line2Num", line2Num, 0, 100);
    gui.addSlider("line2Width", line2width, 0.0f, 20.0f);
    gui.addRangeSlider("lenth_range", mahoLine2MinLength, mahoLine2MaxLength, 0.0, 300.0f);
    gui.addSlider("lengthNoiseAmt", lengthNoiseAmt, 2.0f, 50.0f);
    gui.addSlider("line2Speed", line2speed, 0.0f, 1.0f);
    gui.addSlider("angleAmt", mahoLine2AngleAmt, 0.0f, 20.0f);
    //blob
    gui.addBlank();
    gui.addTitle("blob");
    gui.addToggle("bBlob", bBlob);
    gui.addSlider("pointNum", blobPointNum, 1, 100);
    gui.addRangeSlider("sparkRange", sparkRangeMin, sparkRangeMax, 0.0f, 1000.0f);
    //Keen
    gui.addTitle("Keen");
    gui.addSlider("keenWidth", keenWidth, 0.0f, 50.0f);
    gui.addSlider("keenTime", keenTime, 0, 700);
    //////////////////-maho-//////////////////////
    
    //page 11 ope11==============================
    gui.addPage("OPE 11 Kaji");
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addTitle("kaji").setNewColumn(true);
    gui.addSlider2d("kajiPos", kajiPos, -200.0f, 1280.0f + 200.0f, -200.0f, 960.0f + 200.0f);
    gui.addButton("trPlayKajiVideo", trPlayKajiVideo);
    gui.addToggle("bKajiVideoPlaying", bKajiVideoPlaying).setFix(true);
    gui.addToggle("bKajiAlphaStart", bKajiAlphaStart);
    gui.addButton("trKajiClear", trKajiClear);
    gui.addButton("trKajiLoad", trKajiLoad);
    
    //setting
    gui.addPage("gekijo");
    gui.addMulti2dSlider("gekijo", 4, 8, gekijo, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);

    
    gui.addPage("door");
    gui.addMulti2dSlider("doorPos", 4, 6, doorPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addToggle("bDoorDraw", bDoorDraw).setNewColumn(true);
    
    gui.addPage("Rokka");
    gui.addMulti2dSlider("rokkaPos", 4, 4, rokkaPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addToggle("rokka Draw", bRokkaDraw).setNewColumn(true);
    
    gui.addPage("Hondana");
    gui.addMulti2dSlider("hondanaPoints", 4, HONDANA_INDEX_NUM, hondanaPoints, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addToggle("Hondana Draw", bHondanaDraw).setNewColumn(true);
    gui.addSlider("hondanaNoiseAmt", hondanaNoiseAmt, 0.0f, 50.0f);
    
//    gui.addPage("seraBlob");
//    gui.addMulti2dSlider("seraBlob", 4, 6, seraBlob, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
//    gui.addBlank().setNewColumn(true);
//    gui.addBlank().setNewColumn(true);
//    gui.addBlank().setNewColumn(true);
//    gui.addToggle("seraBlob Draw", bSeraBlobMappingDraw).setNewColumn(true);
    gui.addPage("seraHead");
    gui.addMulti2dSlider("seraHead", 4, 1, seraHeadPos, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    //gui.addToggle("seraBlob Draw", bSeraBlobMappingDraw).setNewColumn(true);
    gui.addToggle("seraHead Draw", bSeraHeadMappingDraw).setNewColumn(true);
    
    
    //xtion
    for (int i = 0; i < XTION_NUM; i++) {
        gui.addPage("Xtion" + ofToString(i));
        gui.addMulti2dSlider("xtion" + ofToString(i), 4, 0, NULL, 0.0f, (float)PROJECTION_SIZE_W, 0.0f, (float)PROJECTION_SIZE_H, 4.0f / 3.0f, true, 0.6f);
        gui.addBlank().setNewColumn(true);
        gui.addBlank().setNewColumn(true);
        gui.addBlank().setNewColumn(true);
        gui.addTitle("Xtion No." + ofToString(i + 1)).setNewColumn(true);
        gui.addToggle("bUpdate", bUpdateXtion[i]);
        gui.addToggle("bDraw", bDraw[i]);
        gui.addSlider("scale", scale[i], 0.001f, 5.0f);
        gui.addSlider("aspect x/y", aspect[i], 0.5f, 2.0f);
        gui.addSlider("scaleZ", scaleZ[i], 0.2f, 2.0f);
        gui.addTitle("translate");
        gui.addSlider("axis.x", axis[i].x, -4000.0f, 4000.0f);
        gui.addSlider("axis.y", axis[i].y, -4000.0f, 4000.0f);
        gui.addSlider("axis.z", axis[i].z, -10000.0f, 0.0f);
        gui.addSlider("rotx", rotx[i], -180, 180);
        gui.addSlider("roty", roty[i], -180, 180);
        gui.addSlider("rotz", rotz[i], -180, 180);
        gui.addTitle("bgCapture").setNewColumn(true);
        gui.addToggle("useBgDepth"+ ofToString(i+1), bUseBgDepth[i]);
        gui.addContentSlider2d("depth", 1, tex[i], depthCheckPoint[i], depthCheckPoint2[i], 0.0f, (float)CAPTURE_WIDTH, 0.0f, (float)CAPTURE_HEIGHT, false);
        gui.addRangeSlider("thresholds:near_far"+ ofToString(i+1), thresholdNear[i], thresholdFar[i], 0, 10000);
        gui.addValueMonitor("depth_value", depthPointValue[i]);
        gui.addSlider("depthSlider", depthSlider[i], 0, 10000);
        gui.addButton("toggleBgSet", bCaptureBg[i]);
        gui.addButton("save", bSaveBgAsImage[i]);
        gui.addButton("load", trLoadBg[i]).setFix(true);
        gui.addSlider("capturePlay", bgCapturePlay[i], 0, 200);
        gui.addTitle("Xtion golobal");
        gui.addToggle("xtionSetup", bSetUp).isFixed();
        gui.addButton("xtionShutdown", bShutDown).isFixed();
        gui.addToggle("bMeshDraw", bDrawMesh).isFixed();
    }
    
    
    

    gui.setPage(1);
    
    exBdrawMesh = bDrawMesh;
}

void uiWindow::update(){
    
    for (int i = 0; i < XTION_NUM; i++) {
        const unsigned char * p = xtions.getDepthGenerator(i).getMonitorTexture();
        tex[i].loadData(p, CAPTURE_WIDTH, CAPTURE_HEIGHT, GL_RGBA);
    }
}

void guiM(string s){
    ofSetColor(ofColor::red);
    ofDrawBitmapString(s, ofPoint(130.0f, 154.0f));
}

void uiWindow::draw(){
    ofPushStyle();
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    ofSetColor(255);
    monitorFboDraw(1, 1, 4, 0.713f);
    gui.draw();
    ofPopStyle();
    ///MontiroMessage
    switch (gui.getCurrentPageNo()) {
        case 1: //page1
            guiM("Warning : did you set the xtion No? 0 is Blob, 1 is XTION 1");
            break;
        case 2:
            guiM("Warning : did you set the xtion No? only xtion No.1\nWarning : did you set hondanaPos?\npos 0 : mahoPos on SMOLL SOFA  pos1 : hondana\n1. mahoDraw  2. Keen draw  3.hondana noise\n3. himawariPlay  4. himawari fadeOut  5. himawari clear");
            break;
        case 3:
            
            break;
        default:
            break;
    }
    ///-MonitorMessage
}

void uiWindow::taskToggle(int scene, int & phase){
    if (scene == 1) {
        if (phase == 0) {
            if (choicedXtion == 0) {
                bDrawScene1Blob = true;
            } else {
                for (int i = 1; i <= XTION_NUM; i++) {
                    if (choicedXtion - 1 == i){
                        bDraw[i] = true;
                    }else bDraw[i] = false;
                }
                bOpenCv = true;
                bCvNoise = true;
            }
            bKirakiraIncrease = true;
            openCvDrawColor = mainPink;
        } else if (phase == 1) {
            bKirakiraDraw = true;
            bCvNoise = false;
            toggleCvFadeOut = true;
        } else if (phase == 2) {
            bDrawScene1Blob = false;
            bOpenCv = false;
        } else if (phase == 3) {
            bKirakiraIncrease = false;
        } else if (phase == 4) {
            bKirakiraDraw = false;
        }
    } else if (scene == 2) {//himawari
        if (phase == 0) {
            trMaho = true;
        } else if (phase == 1){
            trKeen = true;
            mahoIndex = 1;
        } else if (phase == 2){
            trPlayHimawariVideo = true;
        } else if (phase == 3) {
            bHimawariAlphaStart = true;
        } else if (phase == 4) {
            trHimawariClear = true;
            mahoIndex = 0;
        }
    } else if (scene == 3) {//sera
        if (phase == 0){
            toggleCvFadeOut = false;
            trMaho = true;
        } else if (phase == 1){
            mahoIndex = 2;
            openCvDrawColor = mainPink;
            trKeen = true;
        } else if (phase == 2){
            toggleCvFadeOut = true;
        } else if (phase == 3) {
            mahoIndex = 0;
            openCvDrawColor = ofColor(0,0,0);
            bSera = false;
            bDrawSeraBlob = false;
            bOpenCv = false;
        } 
    } else if (scene == 4) {//harada
        if (phase == 0){
            trMaho = true;
        } else if (phase == 1){
            trKeen = true;
            mahoIndex = 3;
            cvNoiseAmt = 20.0f;
        } else if (phase == 2){
            mahoIndex = 0;
            bOpenCv = false;//todo フェードアウトしたい
            bCvBlur = false;
            cvNoiseAmt = 0.0f;
            bCvNoise = false;
        }
    } else if (scene == 5) {//カット
        if (phase == 0){
            mahoIndex = 0;
            milli = 1000;
            FadingStartTime = ofGetElapsedTimeMillis();
            WhiteFading = 1;
            bBackWhite = true;
        } else if (phase == 1){
            bGrotesq = true;
            grotesq.setup();
            grotesq.toggleGo();
        } else if (phase == 2){
            mahoPos[0] = ofPoint(2500.0f, 1000.0f);
        } else if (phase == 3){
            milli = 1000;
            FadingStartTime = ofGetElapsedTimeMillis();
            WhiteFading = 2;
        } else if (phase == 4){
            bBackWhite = false;
            grotesq.clear();
        }
    } else if (scene == 6) {//haradaFire
        if (phase == 0){
            mahoIndex = 6;
            trMaho = true;
        } else if (phase == 1){
            trKeen = true;
        } else if (phase == 2){
            bHaradaFireFadeOut = true;
        } else if (phase == 3){
            haradaFire.clear();
        }
    } else if (scene == 7) {//ロッカーから寿司
        if (phase == 0){
            trMaho = true;
        } else if (phase == 1){
            mahoIndex = 7;
            trKeen = true;
        } else if (phase == 2){
            
            mahoIndex == 0;
        } 
    } else if(scene == 11){
        if (phase == 0) {
            trPlayKajiVideo = true;
        } else if (phase == 1){
            bKajiAlphaStart = true;
        } else if (phase >= 2) {
            if (bKajiAlphaStart) {
                trKajiClear = true;
            }
        }
    } else if (scene == 8) {
        if (phase == 0){
            trMaho = true;
        } else if (phase == 1){
            mahoIndex = 8;
            trKeen = true;
        } else if (phase == 2){
            mahoIndex == 0;
        }
    } else if (scene == 9) {
        if (phase == 0){
            mahoIndex = 9;
            trMaho = true;
        } else if (phase == 1){
            //trKeen = true;
        } else if (phase == 2){
            mahoIndex == 0;
        }
    }
    
}


void uiWindow::ope1Setup(){
    bDrawMesh = false;
    bOpenCv = false;
    bKirakiraDraw = false;
    mahoIndex = 0;
    for (int i = 0; i < 4; i++ ){
        rokkaPosDamy[i] = rokkaPos[i];
    }
    //-----
    gui.setPage(1);
    currentScene = 1;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope2Setup(){
    if (!bKirakiraIncrease && bKirakiraDraw) bKirakiraIncrease = true;
    bDrawMesh = false;
    bOpenCv = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    mahoIndex = 0;
    mahoPos[0].set(722.0f, 572.0f);
    mahoPos[1].set(174.0f, 569.0f);
    trHimawariLoad = true;
    //------
    gui.setPage(2);
    currentScene = 2;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope3Setup(){
    bDrawMesh = false;
    bOpenCv = false;
    bSera = false;
    bKirakiraDraw = false;
    bDrawSeraBlob = false;
    bDrawScene1Blob = false;
    openCvDrawColor = mainPink;
    mahoIndex = 0;
    gui.setPage(3);
    currentScene = 3;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope4Setup(){
    bDrawMesh = false;
    bSera = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    mahoIndex = 0;
    for (int i = 0; i < 6; i++) {
        doorPosDamy[i] = doorPos[i];
    }
    mahoPos[1].set(1245.0f, 662.0f);
    gui.setPage(4);
    currentScene = 4;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope5Setup(){
    bDrawMesh = false;
    bSera = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    bGrotesq = false;
    bHaradaFire = false;
    bDrawScene1Blob = false;
    mahoIndex = 0;
    gui.setPage(5);
    currentScene = 5;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope6Setup(){
    bDrawMesh = false;
    bSera = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    bGrotesq = false;
    bHaradaFire = false;
    bHaradaFireFadeOut = false;
    haradaFire.clear();
    mahoIndex = 0;
    gui.setPage(6);
    currentScene = 6;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope7Setup(){
    for (int i = 0; i < 4; i++) {
        rokkaPosDamy[i] = rokkaPos[i];
    }
    bDrawMesh = false;
    bSera = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    bGrotesq = false;
    bHaradaFire = false;
    trFadeOutLastMaho = false;
    mahoIndex = 0;
    gui.setPage(7);
    currentScene = 7;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope8Setup(){//gomory
    for (int i = 0; i < 4; i++) {
        doorPosDamy[i] = doorPos[i];
    }
    bDrawMesh = false;
    bSera = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    bGrotesq = false;
    bHaradaFire = false;
    mahoIndex = 0;
    gui.setPage(8);
    currentScene = 8;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope9Setup(){
    bDrawMesh = false;
    bSera = false;
    bKirakiraDraw = false;
    bOpenCv = false;
    bGrotesq = false;
    bHaradaFire = false;
    mahoIndex = 0;
    gui.setPage(9);
    currentScene = 9;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::ope11Setup(){
    gui.setPage(11);
    currentScene = 11;
    phaseNoByScene[currentScene] = 0;
}

void uiWindow::keyPressed(int key){
    switch (key) {
        case 267:
            gui.prevPage();
            break;
        case 268:
            gui.nextPage();
            break;
        case 't':
            trTail = true;
            break;
        case ' ':
            taskToggle(currentScene, phaseNoByScene[currentScene]);
            phaseNoByScene[currentScene]++;
            break;
        case '1':
            ope1Setup();
            break;
        case '2':
            ope2Setup();
            break;
        case '3':
            ope3Setup();
            break;
        case '4':
            ope4Setup();
            break;
        case '5':
            ope5Setup();
            break;
        case '6':
            ope6Setup();
            break;
        case '7':
            ope7Setup();
            break;
        case '8':
            ope8Setup();
            break;
        case '9':
            ope9Setup();
            break;
        case 'k':
            ope11Setup();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------


void uiWindow::monitorFboDraw(int controlNoFrom1,int blockNoFrom1, int blockNumByWidth, float scale){
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(11.0f , 134.84f);
    float ww = blockNumByWidth * gui.config->width + gui.config->padding.x * (blockNumByWidth - 1);
    ofDisableAlphaBlending();
    ofSetColor(255);
    monitorFbo.draw( ww * (1.0f - scale)/ 2.0f, ww * 3.0f / 4.0f * (1.0f - scale)/ 2.0f, ww * scale, ww * 3.0f / 4.0f * scale);
    ofPopMatrix();
    ofPopStyle();
}
void uiWindow::addMappingSetup(){
    
}

void uiWindow::consoleOut(){

    
}
