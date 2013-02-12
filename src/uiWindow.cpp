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
ofFbo screenFbo;
ofPoint point1 = ofPoint((float)MONITOR_SIZE_W / 2, (float)MONITOR_SIZE_H);
ofPoint point2 = ofPoint((float)MONITOR_SIZE_W / 2, (float)MONITOR_SIZE_H);
//xtions
ofFbo xtionFbo;
myXtionOperator xtions;
ofTexture tex[XTION_NUM];

ofxCvContourFinder cfinder;

bool doSave;
bool doLoad;

//no use xml


//

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

ofPoint testPoint[4] = { ofPoint() };


ofPoint depthCheckPoint[XTION_NUM] = {ofPoint(0.0f, 0.0f)};///
ofPoint depthCheckPoint2[XTION_NUM] = {ofPoint((float)CAPTURE_WIDTH, (float)CAPTURE_HEIGHT)};
int depthPointValue[XTION_NUM] = {0};///

float pointSize = 4.0;//9
int step = 2;//10
int minArea = 50;//11
int maxArea = 500000;//12
int nConsidered = 10;//13
bool bFindHoles = true;//14
bool bUseApproximation = true;//15

//↑added to xmlset

//point of mapping
bool bMappingDraw = false;
ofPoint maguchi[4] = { ofVec3f(0.0f,0.0f), ofVec3f(MONITOR_SIZE_W,0.0f), ofVec3f(MONITOR_SIZE_W, MONITOR_SIZE_H), ofVec3f(0.0f, MONITOR_SIZE_H) };//間口
ofPoint okuguchi[4] = { ofVec3f(100.0f,100.0f), ofVec3f(MONITOR_SIZE_W - 100.0f,100.0f), ofVec3f(MONITOR_SIZE_W - 100.0f, MONITOR_SIZE_H - 100.0f), ofVec3f(100.0f, MONITOR_SIZE_H - 100.0f) };//奥口

float lineWidth = 2.0f;
bool bFilled[5] = { false, false, false, false, false };

ofPoint targetPointA = ofVec3f(MONITOR_SIZE_W / 2, MONITOR_SIZE_H / 2);
bool bDoorDraw = false;
ofPoint doorPos[5] = {ofPoint(954.0f, 551.0f),
                        ofPoint(1024.0f, 577.0f),
                        ofPoint(1024.0f, 768.0f),
                        ofPoint(954.0f, 698.0f),
                        ofPoint(985.0f, 606.0f)};//centertarget;
ofPoint himawariPos = ofVec3f(50, 300);


void uiWindow::consoleOut(){
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
}


//-xtions
//////////////////
//////////////////
void uiWindow::mySaveToXml(){

    
}
void uiWindow::myLoadFromXml(){


}

void uiWindow::setup(){
    
    
    doSave = false;
    doLoad = false;
    
    gui.setup("FirstPage");
    gui.headerPage->addSlider("Mesh_PointSize", pointSize, 0.001f, 10.0f);
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
    gui.addMulti2dSlider("test", 3, 4, testPoint, 0.0f, 1024.0f, 0.0f, 768.0f, 4.0f / 3.0f, true);
    gui.setPage(5);
    
    myLoadFromXml();
}

void uiWindow::update(){
    
    
    for (int i = 0; i < XTION_NUM; i++) {
        const unsigned char * p = xtions.getDepthGenerator(i).getMonitorTexture();
        tex[i].loadData(p, CAPTURE_WIDTH, CAPTURE_HEIGHT, GL_RGBA);
        
    }
    if (doSave) {
        mySaveToXml();
        doSave = false;
        printf("mySaveToXml() was called.\n");
    }
    if (doLoad) {
        myLoadFromXml();
        doLoad = false;
        printf("myLoadFromXml() was called.\n");
    }
}
void uiWindow::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    screenFboDraw(5, 0, 3);
    gui.draw();
    
}

void uiWindow::screenFboDraw(int page, int blockNo, int blockNumByWidth){
    if (gui.getCurrentPageNo() != page) return;
    ofPushMatrix();
    ofTranslate(gui.config->offset.x + gui.config->width * blockNo, gui.getPages()[1]->getControls()[0]->x + gui.config->sliderTextHeight + 100.0f);
    float ww = blockNumByWidth * gui.config->width + gui.config->padding.x * (blockNumByWidth - 1);
    screenFbo.draw(0.0f, 0.0f, ww, ww * 3.0f / 4.0f);
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
        case 's':
            
            break;
        default:
            printf("key error. key : %u\n", key);
            break;
    }
}

//--------------------------------------------------------------


void uiWindow::addMappingSetup(){
    gui.addPage("mapping_set1");
    gui.addToggle("bMappingDraw", bMappingDraw);
    gui.addSlider2d("magu_t_l", maguchi[0], -50.0f, 210.0f, -50.0f, 210.0f);
    gui.addToggle("fill", bFilled[3]);
    gui.addSlider2d("magu_b_l", maguchi[3], -50.0f, 210.0f, (float)MONITOR_SIZE_H - 210.0f, (float)MONITOR_SIZE_H + 50.0f);
    //-----
    gui.addSlider("lineWidth", lineWidth, 0.0f, 10.0f).setNewColumn(true);
    gui.addSlider2d("oku_t_l", okuguchi[0], 0.0f, MONITOR_SIZE_W / 2, 0.0f, MONITOR_SIZE_H /2 );
    gui.addToggle("fill", bFilled[4]);
    gui.addSlider2d("oku_b_l", okuguchi[3], 0.0f, MONITOR_SIZE_W / 2, MONITOR_SIZE_H / 4, MONITOR_SIZE_H);
    //-----
    gui.addToggle("fill", bFilled[0]).setNewColumn(true);
    gui.addSlider2d("oku_t_r", okuguchi[1], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, 0.0f, MONITOR_SIZE_H / 2 );
    gui.addSlider2d("oku_b_r", okuguchi[2], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, MONITOR_SIZE_H / 4, MONITOR_SIZE_H);
    gui.addToggle("fill", bFilled[2]);
    //------
    gui.addSlider2d("magu_t_l", maguchi[1], MONITOR_SIZE_W - 210.0f, MONITOR_SIZE_W + 50.0f, -50.0f, 210.0f).setNewColumn(true);
    gui.addToggle("fill", bFilled[1]);
    gui.addSlider2d("magu_b_l", maguchi[2], MONITOR_SIZE_W - 210.0f, MONITOR_SIZE_W + 50.0f, MONITOR_SIZE_H - 210.0f, MONITOR_SIZE_H + 50.0f);
    ///////////////////////////
    ///////////////////////////
    gui.addPage("mapping_set2");
    gui.addTitle("door's mapping");
    gui.addSlider2d("t_l", doorPos[0], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, MONITOR_SIZE_H / 2, MONITOR_SIZE_H);
    gui.addBlank();
    gui.addBlank();
    gui.addSlider2d("b_l", doorPos[3], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, MONITOR_SIZE_H / 2, MONITOR_SIZE_H);
    //------------
    gui.addBlank().setNewColumn(true);
    gui.addBlank();
    gui.addBlank();
    gui.addBlank();
    gui.addBlank();
    gui.addBlank();
    gui.addToggle("doorDraw", bDoorDraw);
    gui.addSlider2d("doorTarget", doorPos[4], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, MONITOR_SIZE_H / 2, MONITOR_SIZE_H);
    //-------------
    gui.addSlider2d("t_r", doorPos[1], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, MONITOR_SIZE_H / 2, MONITOR_SIZE_H ).setNewColumn(true);
    gui.addBlank();
    gui.addBlank();
    gui.addSlider2d("b_r", doorPos[2], MONITOR_SIZE_W / 2, MONITOR_SIZE_W, MONITOR_SIZE_H / 2, MONITOR_SIZE_H);
    
    
    
}