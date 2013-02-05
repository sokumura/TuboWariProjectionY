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
//xtions
myXtionOperator xtions;
ofTexture tex[XTION_NUM];

bool doSave;
bool doLoad;

//no use xml
bool trFreeBg[XTION_NUM];

//


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
bool bUseBgDepth[XTION_NUM] = {false};//4
bool trLoadBg[XTION_NUM] = {false};
bool bCaptureBg[XTION_NUM] = {false};
bool bSaveBgAsImage[XTION_NUM] = {false};
float depthBgAverage[XTION_NUM] = {0};
int realDepthMax[XTION_NUM] = {0};

float pointSize = 4.0;//9
int step = 2;//10
int minArea = 50;//11
int maxArea = 500000;//12
int nConsidered = 10;//13
bool bFindHoles = true;//14
bool bUseApproximation = true;//15

//↑added to xmlset

//point of mapping
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
//    printf("mySaveToXml が呼ばれました。\n");
//    for (int i = 0; i < XTION_NUM; i++) {
//        xml.setValue("bgCapturePlay" + ofToString(i), bgCapturePlay[i]);//1
//        xml.setValue("thresholdNear" + ofToString(i), thresholdNear[i]);//2
//        xml.setValue("thresholdFar" + ofToString(i), thresholdFar[i]);//3
//        xml.setValue("bUseBgDepth" + ofToString(i), bUseBgDepth[i]);//4
//        xml.setValue("rotx" + ofToString(i), rotx[i]);//5
//        xml.setValue("roty" + ofToString(i), roty[i]);//6
//        xml.setValue("rotz" + ofToString(i), rotz[i]);//7
//        xml.setValue("axis_x" + ofToString(i), axis[i].x);//8
//        xml.setValue("axis_y" + ofToString(i), axis[i].y);//8
//        xml.setValue("axis_z" + ofToString(i), axis[i].z);//8
//    }
//    xml.setValue("pointSize", pointSize);
//    xml.setValue("step", step);
//    xml.setValue("minArea", minArea);
//    xml.setValue("maxArea", maxArea);
//    xml.setValue("nConsidered", nConsidered);
//    xml.setValue("bFindHoles", bFindHoles);
//    xml.setValue("bUseApproximation", bUseApproximation);
//    
//    //mapping_setup
//    xml.setValue("maguchi0x", maguchi[0].x);
//    xml.setValue("maguchi0y", maguchi[0].y);
//    xml.setValue("maguchi1x", maguchi[1].x);
//    xml.setValue("maguchi1y", maguchi[1].y);
//    xml.setValue("maguchi2x", maguchi[2].x);
//    xml.setValue("maguchi2y", maguchi[2].y);
//    xml.setValue("maguchi3x", maguchi[3].x);
//    xml.setValue("maguchi3y", maguchi[3].y);
//    
//    xml.setValue("okuguchi0x", okuguchi[0].x);
//    xml.setValue("okuguchi0y", okuguchi[0].y);
//    xml.setValue("okuguchi1x", okuguchi[1].x);
//    xml.setValue("okuguchi1y", okuguchi[1].y);
//    xml.setValue("okuguchi2x", okuguchi[2].x);
//    xml.setValue("okuguchi2y", okuguchi[2].y);
//    xml.setValue("okuguchi3x", okuguchi[3].x);
//    xml.setValue("okuguchi3y", okuguchi[3].y);
//    
//    //--mapping_setup
//    xml.saveFile("ToboWariProjection.xml");
    
}
void uiWindow::myLoadFromXml(){
//    bool status = xml.loadFile("ToboWariProjection.xml");
//    if (!status) {
//        ofLog(OF_LOG_ERROR, "\nLoad xml File is failed!!!\n\n");
//    }
//    for (int i = 0; i < XTION_NUM; i++) {
//        bgCapturePlay[i] = xml.getValue("bgCapturePlay" + ofToString(i), bgCapturePlay[i]);//1
//        thresholdNear[i] = xml.getValue("thresholdNear" + ofToString(i), thresholdNear[i]);//2
//        thresholdFar[i] = xml.getValue("thresholdFar" + ofToString(i), thresholdFar[i]);//3
//        bUseBgDepth[i] = xml.getValue("bUseBgDepth" + ofToString(i), bUseBgDepth[i]);//4
//        rotx[i] = xml.getValue("rotx" + ofToString(i), rotx[i]);//5
//        roty[i] = xml.getValue("roty" + ofToString(i), roty[i]);//6
//        rotz[i] = xml.getValue("rotz" + ofToString(i), rotz[i]);//7
//        axis[i].x = xml.getValue("axis_x" + ofToString(i), axis[i].x);//8
//        axis[i].y = xml.getValue("axis_y" + ofToString(i), axis[i].y);//8
//        axis[i].z = xml.getValue("axis_z" + ofToString(i), axis[i].z);//8
//    }
//    pointSize = xml.getValue("pointSize", pointSize);
//    step = xml.getValue("step", step);
//    minArea = xml.getValue("minArea", minArea);
//    maxArea = xml.getValue("maxArea", maxArea);
//    nConsidered = xml.getValue("nConsidered", nConsidered);
//    bFindHoles = xml.getValue("bFindHoles", bFindHoles);
//    bUseApproximation = xml.getValue("bUseApproximation", bUseApproximation);

}

void uiWindow::setup(){
    
    
    doSave = false;
    doLoad = false;
    
    gui.setup("FirstPage");
    gui.headerPage->addButton("Save to Xml", doSave);
    gui.headerPage->addButton("Load from Xml", doLoad);
    gui.addTitle("XTION SETTING");
    gui.addSlider("Mesh_PointSize", pointSize, 0.001f, 10.0f);
    gui.addSlider("step", step, 1, 20);
    gui.addSlider("minArea", minArea, 0, 1000);
    gui.addSlider("maxArea", maxArea, 30, 700000);
    gui.addSlider("nConsidered", nConsidered, 10, 1000);
    gui.addToggle("bFindHoles", bFindHoles);
    gui.addToggle("bUseApproximation", bUseApproximation);
    gui.addBlank();
    
    //mapping_set
    addMappingSetup();
    //--mapping_set
    bool bAddPage = false;
    for (int i = 0; i < XTION_NUM; i++) {
        //gui
        if ( i == 0 || i == 3 ) gui.addPage();
        if(i > 0) gui.addTitle("Xtion No." + ofToString(i + 1)).setNewColumn(true);
        else gui.addTitle("Xtion No." + ofToString(i + 1));
        gui.addContent("depth_map" + ofToString(i+1), tex[i]);
        gui.addRangeSlider("thresholds:near_far"+ ofToString(i+1), thresholdNear[i], thresholdFar[i], 0, 10000);
        gui.addSlider("scale", scale[i], 0.001f, 5.0f);
        gui.addSlider("aspect x/y", aspect[i], 0.5f, 2.0f);
        gui.addSlider("scaleZ", scaleZ[i], 0.2f, 2.0f);
        gui.addTitle("translate");
        gui.addSlider("axis.x", axis[i].x, -4000.0f, 4000.0f);
        gui.addSlider("axis.y", axis[i].y, -4000.0f, 4000.0f);
        gui.addSlider("axis.z", axis[i].z, -10000.0f, 0.0f);
        gui.addSlider("rotx", rotx[i], -180, 180).setNewColumn(true);
        gui.addSlider("roty", roty[i], -180, 180);
        gui.addSlider("rotz", rotz[i], -180, 180);
        gui.addTitle("bgCapture");
        gui.addToggle("useBgDepth"+ ofToString(i+1), bUseBgDepth[i]);
        gui.addButton("bgCapture()", bCaptureBg[i]);
        gui.addButton("save", bSaveBgAsImage[i]);
        gui.addButton("load", trLoadBg[i]).setFix(true);
        gui.addButton("bgFree()", trFreeBg[i]);
        gui.addSlider("capturePlay", bgCapturePlay[i], 0, 200);
        gui.addValueMonitor("realDepthMAX", realDepthMax[i]);
        gui.addValueMonitor("depthBgAverage", depthBgAverage[i]);
        
    }
    gui.setPage(1);
    
    myLoadFromXml();
}

void uiWindow::update(){
    
    
    for (int i = 0; i < XTION_NUM; i++) {
        const unsigned char * p = xtions.getDepthGenerator(i).getMonitorTexture();
        tex[i].loadData(p, CAPTURE_WIDTH, CAPTURE_HEIGHT, GL_RGBA);
        //button
        if (trFreeBg[i]) {
            xtions.getDepthGenerator(i).freeBgDepth();
            trFreeBg[i] = false;
        }
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
    gui.draw();
    
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