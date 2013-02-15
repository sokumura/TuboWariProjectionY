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
//main



ofFbo screenFbo;
ofFbo monitorFbo;
ofPoint point1 = ofPoint((float)PROJECTION_SIZE_W / 2, (float)PROJECTION_SIZE_H);
ofPoint point2 = ofPoint((float)PROJECTION_SIZE_W / 2, (float)PROJECTION_SIZE_H);
//xtions
ofFbo xtionFbo;
myXtionOperator xtions;
ofTexture tex[XTION_NUM];
bool bXtionUpdate = true;

//openCv
bool bOpenCv = true;
bool bCfinderDraw = true;
ofxCvContourFinder cfinder;

bool bDraw[XTION_NUM] = {false};
int thresholdNear[XTION_NUM];//2
int thresholdFar[XTION_NUM];//3

float rotx[XTION_NUM];//5
float roty[XTION_NUM];//6
float rotz[XTION_NUM];//7
ofVec3f axis[XTION_NUM];//8
float scale[XTION_NUM];
float aspect[XTION_NUM];
float scaleZ[XTION_NUM];
int bgCapturePlay[XTION_NUM] = {50};//1
bool bCaptureBg[XTION_NUM] = {false};
bool bUseBgDepth[XTION_NUM] = {false};//4
int depthSlider[XTION_NUM] = { 10000 };
bool trLoadBg[XTION_NUM] = {false};
bool bSaveBgAsImage[XTION_NUM] = {false};
bool trGetPointDepth[XTION_NUM] = {false};
int realDepthMax[XTION_NUM] = {0};

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

//point of mapping
bool bMappingDraw = false;
bool bMappingDrawOnMonitor = true;
ofPoint maguchi[4] = {
    ofPoint(0.0f,46.0f),
    ofPoint(PROJECTION_SIZE_W,46.0f),
    ofPoint(PROJECTION_SIZE_W, 922.0f),
    ofPoint(0.0f, 922.0f) };//間口
ofPoint okuguchi[4] = {
    ofPoint(210.0f,60.0f),
    ofPoint(1064.0f,60.0f),
    ofPoint(1064, 586.0f),
    ofPoint(210.0f, 586.0f) };//奥口
ofPoint maguchiAll = ofPoint();
ofPoint okuguchiAll = ofPoint();
ofPoint exMaguchiAll = ofPoint();
ofPoint exOkuguchiAll = ofPoint();


float lineWidth = 4.0f;
bool bFilled[5] = { false, false, false, false, false };

ofPoint targetPointA = ofVec3f(PROJECTION_SIZE_W / 2, PROJECTION_SIZE_H / 2);
bool bDoorDraw = false;
ofPoint doorPos[5] = {ofPoint(954.0f, 551.0f),
    ofPoint(1024.0f, 577.0f),
    ofPoint(1024.0f, 768.0f),
    ofPoint(954.0f, 698.0f),
    ofPoint(985.0f, 606.0f)};//centertarget;

///////////operation////////////
///////////magics//////////////
int startMahoTime, currentMahoTime = 0;

ofPoint hondanaPoints[HONDANA_INDEX_NUM];
float hondanaNoiseAmt = 5.0f;

int magicIndex = 0;
ofFloatColor mainPink = ofFloatColor(1.0f, 0.44f, 0.72f, 1.0f);
ofPoint magicPos[2] = {ofPoint(-50.0f, -50.0f)};
//line
bool bLine1 = true;
int lineNumber = 300;
bool trGoko1 = false;
float accelAmt = 0.75f;
float lineColorAmt = 0.2f;
//bunshi
bool bBunshi = false;
int bunshiNum = 70;//10 - 1000;
float mainParticleSize = 5.0f;
float radiusAmt = 0.4f;//0.1 - 3.0f;
float radiusMin = 1.6f;
float radiusMax = 2.8f;
float energyAmt = 0.565f;//1-10;
float kasokuAmt = 0.855f;//1.0 - 100.0f
float alphaAmt = 0.635f;
int mainBunshiLifeTime = 1445;
//line2
bool bLine2 = true;
int line2Num = 42;
float minLength = 28.5f;//2.0f - 300.0f
float maxLength = 79.5f;//
float lengthNoiseAmt = 16.4f;
float line2speed = 0.08f;//0 - 1
float angleAmt = 0.30f;//0 - 60
float line2width = 2.5f;
//blobs
bool bBlob = true;
int blobPointNum = 33;//1 - 100 = 10
float sparkRangeMin = 5;//0 - 1000
float sparkRangeMax = 100;
//keen
bool trKeen = false;
float keenWidth = 4.8f;
ofPoint keenStart = ofPoint(500.0f, 500.0f);
ofPoint keenGoal = ofPoint(1000.0f, 1000.0f);
//////////-magics////////////
//inazuma
int inazumaNo = 0;
 bool trInazumaLoad = false;
 bool bInazumaDraw = false;
 ofImage inazuma;
//videos
ofQTKitPlayer himawari;
ofQTKitPlayer kaji;
 ofPoint himawariPos;
 ofPoint kajiPos;
 bool trPlayHimawariVideo = false;
 bool bHimawariVideoPlaying = false;
 bool bHimawariAlphaStart = false;
 bool trHimawariClear = false;

 bool trPlayKajiVideo = false;
 bool bKajiVideoPlaying = false;
 bool bKajiAlphaStart = false;
 bool trKajiClear = false;

 int fadeTimeMilis = 1000;//1000 - 10000

//グローバルの初期化
uiWindow::uiWindow(){
    for (int i = 0; i < XTION_NUM; i++) {
        tex[i].allocate(CAPTURE_WIDTH, CAPTURE_HEIGHT, GL_RGBA);
        thresholdNear[i] = 1600;//2
        thresholdFar[i] = 3400;//3
        rotx[i] = 0.0f;
        roty[i] = 0.0f;
        rotz[i] = 0.0f;
        axis[i] = ofVec3f(248.0f, 272.0f, -5000.0f);
        scale[i] = 0.4f;
        aspect[i] = 1.0f;
        scaleZ[i] = 0.7f;
    }
    
    //magic
    for (int i = 0; i < HONDANA_INDEX_NUM; i++) {
        
        ofPoint p = ofPoint(200.0f + 200.0f * cos(TWO_PI / HONDANA_INDEX_NUM), 500.0f + 200.0f * cos(TWO_PI / HONDANA_INDEX_NUM));//とりあえず
        hondanaPoints[i] = p;
    }
}

void uiWindow::mySaveToXml(){}
void uiWindow::myLoadFromXml(){}

void uiWindow::setup(){
    gui.setup("FirstPage");
    gui.headerPage->addToggle("bOpenCv", bOpenCv);
    gui.headerPage->addToggle("bXtionUpdate", bXtionUpdate);
    gui.addSlider("Mesh_PointSize", pointSize, 0.001f, 10.0f);
    gui.headerPage->addSlider("minArea", minArea, 0, 1000);
    gui.headerPage->addSlider("maxArea", maxArea, 30, 700000);
    gui.addSlider("step", step, 1, 20);
    gui.addSlider("nConsidered", nConsidered, 10, 1000);
    gui.addToggle("bFindHoles", bFindHoles);
    gui.addToggle("bUseApproximation", bUseApproximation);
    //mapping_set
    addMappingSetup();
    //--mapping_set
    bool bAddPage = false;
    for (int i = 0; i < XTION_NUM; i++) {
        //gui
        if ( i == 0 || i == 3 ) gui.addPage();
        if(i > 0) gui.addTitle("Xtion No." + ofToString(i + 1)).setNewColumn(true);
        else gui.addTitle("Xtion No." + ofToString(i + 1));
        gui.addToggle("bDraw", bDraw[i]);
        gui.addToggle("bMeshDraw", bDrawMesh);
        gui.addToggle("useBgDepth"+ ofToString(i+1), bUseBgDepth[i]);
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
        gui.addContentSlider2d("depth", 1, tex[i], depthCheckPoint[i], depthCheckPoint2[i], 0.0f, (float)CAPTURE_WIDTH, 0.0f, (float)CAPTURE_HEIGHT, false);
        gui.addRangeSlider("thresholds:near_far"+ ofToString(i+1), thresholdNear[i], thresholdFar[i], 0, 10000);
        gui.addValueMonitor("depth_uint", depthPointValue[i]);
        gui.addToggle("getPointDepth", trGetPointDepth[i]);
        gui.addSlider("depthSlider", depthSlider[i], 0, 10000);
        gui.addButton("toggleBgSet", bCaptureBg[i]);
    
        gui.addButton("save", bSaveBgAsImage[i]);
        gui.addButton("load", trLoadBg[i]).setFix(true);
        gui.addSlider("capturePlay", bgCapturePlay[i], 0, 200);
        gui.addValueMonitor("realDepthMAX", realDepthMax[i]);
    }
    gui.addPage("test");
    gui.addMulti2dSlider("test", 4, 2, magicPos, 0.0f, (float)PROJECTION_SIZE_W, 0.0f, (float)PROJECTION_SIZE_H, 4.0f / 3.0f, true, 0.6f);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addBlank().setNewColumn(true);
    gui.addToggle("Map Draw onMonitor", bMappingDrawOnMonitor).setNewColumn(true);
    gui.addValueMonitor("currentMahoTime", currentMahoTime).setFix(true);
    gui.addButton("toggle goko1", trGoko1);
    gui.addButton("Keen!!", trKeen);
    gui.addToggle("bInazumaDraw", bInazumaDraw);
    
    gui.addToggle("cFinderDraw", bCfinderDraw);
    gui.addPage("anothersetting");
    gui.addMulti2dSlider("hondanaPos", 1, HONDANA_INDEX_NUM, hondanaPoints, 0.0f, 1280.0f, 0.0f, 960.0f, 4/3, false, false);
    gui.addTitle("inazuma");
    gui.addToggle("bInazumaDraw", bInazumaDraw);
    gui.addComboBox("inazumaNo", inazumaNo, 3);
    gui.addContent("nextInazuma", inazuma);
    gui.addButton("trInazumaLoad", trInazumaLoad);
    gui.addTitle("videos").setNewColumn(true);
    gui.addSlider2d("himawariPos", himawariPos, -200.0f, 1280.0f + 200.0f, -200.0f, 960.0f + 200.0f);
    gui.addSlider2d("kajiPos", kajiPos, -200.0f, 1280.0f + 200.0f, -200.0f, 960.0f + 200.0f);
    gui.addButton("trPlayKajiVideo", trPlayKajiVideo);
    gui.addToggle("bKajiVideoPlaying", bKajiVideoPlaying).setFix(true);
    gui.addToggle("bKajiAlphaStart", bKajiAlphaStart);
    gui.addButton("trKajiClear", trKajiClear);
    gui.setPage(5);
}

void uiWindow::update(){
    for (int i = 0; i < XTION_NUM; i++) {
        const unsigned char * p = xtions.getDepthGenerator(i).getMonitorTexture();
        tex[i].loadData(p, CAPTURE_WIDTH, CAPTURE_HEIGHT, GL_RGBA);
    }
    if (maguchiAll.x != exMaguchiAll.x || maguchiAll.y != exMaguchiAll.y) {
        for (int i = 0; i < 4; i++) {
            maguchi[i].x += (maguchiAll.x - exMaguchiAll.x);
            maguchi[i].y += (maguchiAll.y -exMaguchiAll.y);
        }
        exMaguchiAll = maguchiAll;
    }
    if (okuguchiAll.x != exOkuguchiAll.x || okuguchiAll.y != exOkuguchiAll.y) {
        for (int i = 0; i < 4; i++) {
            okuguchi[i].x += (okuguchiAll.x - exOkuguchiAll.x);
            okuguchi[i].y += (okuguchiAll.y - exOkuguchiAll.y);
        }
        exOkuguchiAll = okuguchiAll;
    }
}
void uiWindow::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    monitorFboDraw(5, 1, 1, 4, 0.713f);
    gui.draw();
}

void uiWindow::monitorFboDraw(int pageFrom1, int controlNoFrom1,int blockNoFrom1, int blockNumByWidth, float scale){
    if (gui.getCurrentPageNo() != pageFrom1) return;
    ofPushMatrix();
    ofTranslate(gui.config->offset.x + gui.config->width * (blockNoFrom1 - 1) ,
                gui.getPages()[pageFrom1]->getControls()[controlNoFrom1 - 1]->y + gui.config->slider2DTextHeight);
    float ww = blockNumByWidth * gui.config->width + gui.config->padding.x * (blockNumByWidth - 1);
    monitorFbo.draw( ww * (1.0f - scale)/ 2.0f, ww * 3.0f / 4.0f * (1.0f - scale)/ 2.0f, ww * scale, ww * 3.0f / 4.0f * scale);
    ofPopMatrix();
}

void uiWindow::keyPressed(int key){
    switch (key) {
        case 267:
            gui.prevPage();
            break;
        case 268:
            gui.nextPage();
            break;
        case 'c':
            consoleOut();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------


void uiWindow::addMappingSetup(){
    gui.addPage("mapping_set1");
    gui.addToggle("Map Draw onScreen", bMappingDraw);
    gui.addSlider2d("magu_t_l", maguchi[0], -50.0f, 210.0f, -50.0f, 210.0f);
    gui.addToggle("fill", bFilled[3]);
    gui.addSlider2d("magu_b_l", maguchi[3], -50.0f, 210.0f, (float)PROJECTION_SIZE_H - 210.0f, (float)PROJECTION_SIZE_H + 50.0f);
    //-----
    gui.addSlider("lineWidth", lineWidth, 0.0f, 10.0f).setNewColumn(true);
    gui.addSlider2d("oku_t_l", okuguchi[0], 0.0f, PROJECTION_SIZE_W / 2, 0.0f, PROJECTION_SIZE_H /2 );
    gui.addToggle("fill", bFilled[4]);
    gui.addSlider2d("oku_b_l", okuguchi[3], 0.0f, PROJECTION_SIZE_W / 2, PROJECTION_SIZE_H / 4, PROJECTION_SIZE_H);
    //-----
    gui.addToggle("fill", bFilled[0]).setNewColumn(true);
    gui.addSlider2d("oku_t_r", okuguchi[1], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, 0.0f, PROJECTION_SIZE_H / 2 );
    gui.addSlider2d("oku_b_r", okuguchi[2], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, PROJECTION_SIZE_H / 4, PROJECTION_SIZE_H);
    gui.addToggle("fill", bFilled[2]);
    //------
    gui.addSlider2d("magu_t_l", maguchi[1], PROJECTION_SIZE_W - 210.0f, PROJECTION_SIZE_W + 50.0f, -50.0f, 210.0f).setNewColumn(true);
    gui.addToggle("fill", bFilled[1]);
    gui.addSlider2d("magu_b_l", maguchi[2], PROJECTION_SIZE_W - 210.0f, PROJECTION_SIZE_W + 50.0f, PROJECTION_SIZE_H - 210.0f, PROJECTION_SIZE_H + 50.0f);
    gui.addSlider2d("maguchiAll", maguchiAll, -500.0f, 500.0f, -500.0f, 500.0f).setNewColumn(true);
    gui.addSlider2d("okuguchiAll", okuguchiAll, -500.0f, 500.0f, -500.0f, 500.0f);
    ///////////////////////////
    ///////////////////////////
    gui.addPage("mapping_set2");
    gui.addTitle("door's mapping");
    gui.addSlider2d("t_l", doorPos[0], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, PROJECTION_SIZE_H / 2, PROJECTION_SIZE_H);
    gui.addBlank();
    gui.addBlank();
    gui.addSlider2d("b_l", doorPos[3], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, PROJECTION_SIZE_H / 2, PROJECTION_SIZE_H);
    //------------
    gui.addBlank().setNewColumn(true);
    gui.addBlank();
    gui.addBlank();
    gui.addBlank();
    gui.addBlank();
    gui.addBlank();
    gui.addToggle("doorDraw", bDoorDraw);
    gui.addSlider2d("doorTarget", doorPos[4], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, PROJECTION_SIZE_H / 2, PROJECTION_SIZE_H);
    //-------------
    gui.addSlider2d("t_r", doorPos[1], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, PROJECTION_SIZE_H / 2, PROJECTION_SIZE_H ).setNewColumn(true);
    gui.addBlank();
    gui.addBlank();
    gui.addSlider2d("b_r", doorPos[2], PROJECTION_SIZE_W / 2, PROJECTION_SIZE_W, PROJECTION_SIZE_H / 2, PROJECTION_SIZE_H);
}

void uiWindow::consoleOut(){
    printf("-------------global console-----------\n");
    for (int i = 0; i < 4; i++) {
        printf("maguchi[%u] x:%.2f, y:%.2f, z:%.2f\n", i, maguchi[i].x, maguchi[i].y, maguchi[i].z);
    }
    for (int i = 0; i < 4; i++) {
        printf("okuguchi[%u] x:%.2f, y:%.2f, z:%.2f\n", i, okuguchi[i].x, okuguchi[i].y, maguchi[i].z);
    }
    
    printf("-------------//global console-----------\n");
    std::cout <<
    "///////////////consoleOut///////////////\n" <<
    "DoorPos : " << doorPos << "\n" <<
    "HimawariPos : " << himawariPos << "\n" <<
    "PointSize : " << pointSize << "step : " << step << "\n" << std::endl;
    for (int i = 0; i < XTION_NUM; i++) {
        std::cout <<
        "///XTION number : " << i << "\n" <<
        "threshold FAR : " << thresholdFar[i] << " NEAR : " << thresholdNear[i] << "\n" <<
        "rotx[ " << i << " ] : " << rotx[i] << "\n" <<
        "roty[ " << i << " ] : " << roty[i] << "\n" <<
        "rotz[ " << i << " ] : " << rotz[i] << "\n\n" <<
        "axis[ " << i << " ] x : " << axis[i].x << " y : " << axis[i].y << " z : " << axis[i].z << std::endl;
    }
    std::cout <<
    "//////////console out end//////////\n\n" << std::endl;
    
}
