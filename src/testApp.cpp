#include "testApp.h"
//////////////////
//////GLOBAL//////
//xtions
myXtionOperator xtions;
ofTexture monoTexture[XTION_NUM];
ofTexture tex[XTION_NUM];
//soDepthThresholds thresholds[XTION_NUM];

bool doSave;
bool doLoad;

//no use xml
bool trCaptureBg[XTION_NUM];
bool trFreeBg[XTION_NUM];
//

int bgCapturePlay[XTION_NUM];//1
int thresholdNear[XTION_NUM];//2
int thresholdFar[XTION_NUM];//3
bool bUseBgDepth[XTION_NUM];//4
float rotx[XTION_NUM];
float roty[XTION_NUM];
float rotz[XTION_NUM];
ofVec3f axis[XTION_NUM];

//グローバルの初期化
uiWindow::uiWindow(){
    for (int i = 0; i < XTION_NUM; i++) {
        tex[i].allocate(640, 480, GL_RGBA);
        monoTexture[i].allocate(640, 480, GL_LUMINANCE);
        trCaptureBg[i] = false;
        trFreeBg[i] = false;
        bgCapturePlay[i] = 10;//1
        thresholdNear[i] = 50;//2
        thresholdFar[i] = 8000;//3
        bUseBgDepth[i] = false;//4
        rotx[i] = 0.0f;
        roty[i] = 0.0f;
        rotz[i] = 0.0f;
        axis[i] = ofVec3f(248, 272, -7760);
    }
}


//-xtions
//////////////////
//////////////////
void uiWindow::mySaveToXml(){
    for (int i = 0; i < XTION_NUM; i++) {
        xml.setValue("bgCapturePlay" + ofToString(i), bgCapturePlay[i]);//1
        xml.setValue("thresholdNear" + ofToString(i), thresholdNear[i]);//2
        xml.setValue("thresholdFar" + ofToString(i), thresholdFar[i]);//3
        xml.setValue("bUseBgDepth" + ofToString(i), bUseBgDepth[i]);//4
    }
    xml.saveFile("ToboWariProjection.xml");
    
}
void uiWindow::myLoadFromXml(){
    bool status = xml.loadFile("ToboWariProjection.xml");
    if (!status) {
        ofLog(OF_LOG_ERROR, "\nLoad xml File is failed!!!\n\n");
    }
    for (int i = 0; i < XTION_NUM; i++) {
        bgCapturePlay[i] = xml.getValue("bgCapturePlay" + ofToString(i), bgCapturePlay[i]);//1
        thresholdNear[i] = xml.getValue("thresholdNear" + ofToString(i), thresholdNear[i]);//2
        thresholdFar[i] = xml.getValue("thresholdFar" + ofToString(i), thresholdFar[i]);//3
        bUseBgDepth[i] = xml.getValue("bUseBgDepth" + ofToString(i), bUseBgDepth[i]);//4
    }
}

void uiWindow::setup(){
    doSave = false;
    doLoad = false;
    
    gui.setup("FirstPage");
    gui.headerPage->addButton("Save to Xml", doSave);
    gui.headerPage->addButton("Load to Xml", doLoad);
    for (int i = 0; i < XTION_NUM; i++) {

        
        //gui
        if(i > 0) gui.addTitle("Xtion No." + ofToString(i + 1)).setNewColumn(true);
        else gui.addTitle("Xtion No." + ofToString(i + 1));
        gui.addContent("depth_map" + ofToString(i+1), tex[i]);
        gui.addRangeSlider("thresholds:near_far"+ ofToString(i+1), thresholdNear[i], thresholdFar[i], 0, 10000);
        gui.addSlider("axis.x", axis[i].x, -1000, 1000);
        gui.addSlider("axis.y", axis[i].y, -1000, 1000);
        gui.addSlider("axis.z", axis[i].z, -100000, -5000);
        gui.addSlider("rotx", rotx[i], -180, 180);
        gui.addSlider("roty", roty[i], -180, 180);
        gui.addSlider("rotz", rotz[i], -180, 180);
        gui.addToggle("useBgDepth"+ ofToString(i+1), bUseBgDepth[i]).setNewColumn(true);
        gui.addButton("bgCapture()"+ ofToString(i+1), trCaptureBg[i]);
        gui.addButton("bgFree()"+ ofToString(i+1), trFreeBg[i]);
        gui.addSlider("capturePlay"+ ofToString(i+1), bgCapturePlay[i], 0, 200);
    }
    
    
}

void uiWindow::update(){
    for (int i = 0; i < XTION_NUM; i++) {
        const unsigned char * p = xtions.getDepthGenerator(i).getMonitorTexture();
        tex[i].loadData(p, 640, 480, GL_RGBA);
        const unsigned char * q = xtions.getDepthGenerator(i).getMonoTexture();
        monoTexture[i].loadData(q, 640, 480, GL_LUMINANCE);
        //button
        if (trCaptureBg[i]) {
            xtions.getDepthGenerator(i).runCapture();
            trCaptureBg[i] = false;
        }
        if (trFreeBg[i]) {
            xtions.getDepthGenerator(i).freeBgDepth();
            trFreeBg[i] = false;
        }
        
        xtions.getDepthGenerator(i).bBgDepth = bUseBgDepth[i];
        xtions.getDepthGenerator(i).capturePlay = bgCapturePlay[i];
        
        
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

void uiWindow::keyPressed(int key){
    if(key>='0' && key<='9') {
        gui.setPage(key - '0');
        gui.show();
    } else {
        switch(key) {
            case OF_KEY_ESC: exitApp(); break;
            case ' ': gui.toggleDraw(); break;
            case '[': gui.prevPage(); break;
            case ']': gui.nextPage(); break;
            case 'p': gui.nextPageWithBlank(); break;
        }
    }
}
void uiWindow::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    gui.draw();
    
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void testApp::setup(){
    printf("testApp setup() が呼ばれました。\n");
    //ofSetVerticalSync(true);
//fenster
	ofxFenster * win = ofxFensterManager::get()->createFenster(400 , 0, 1300, 900, OF_WINDOW);
	win -> addListener(new uiWindow());
    ui.setup();
    ofxFensterManager::get()->getPrimaryWindow()->setWindowPosition(-1279, 182);
    ofxFensterManager::get()->getPrimaryWindow()->toggleFullscreen();
//--fenster
//xtion--
    xtions.setup();
    
//--xtion
}

//--------------------------------------------------------------
void testApp::update(){
	//xtion--
    xtions.update();
    
    
    //--xtion
    counter++;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    if (sin(ofGetElapsedTimef()) < -0.9) ofSetColor(0.0f, 0.0f, 255.0f, 255.0f);
    else if (sin(ofGetElapsedTimef()) > 0.9) ofSetColor(0, 255.0f, 0.0f, 255.0f);
    else ofSetColor(200, 0, 0, 255.0f);
    ofCircle(100.0f, 100.0f, 100.0f * sin(ofGetElapsedTimef()), 10.0f);
    ofSetColor(255);
    
    xtions.testDraw();
    
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='f') ofxFensterManager::get()->getPrimaryWindow()->toggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}