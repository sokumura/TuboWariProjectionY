#include "testApp.h"

////GLOBAL////
extern ofFbo screenFbo;
extern ofFbo monitorFbo;
extern ofPoint testPoint[4];
//xtion
extern ofFbo xtionFbo;
extern myXtionOperator xtions;

//mapping
extern bool bMappingDraw;
extern float lineWidth;
extern ofPoint maguchi[4];
extern ofPoint okuguchi[4];
extern bool bFilled[5];
extern ofPoint targetPointA;
extern ofPoint doorPos[5];
extern bool bDoorDraw;
extern ofPoint himawariPos;

extern int minArea;//11
extern int maxArea;//12
extern int nConsidered;
extern bool bFindHoles;
extern bool bUseApproximation;

////GLOBAL----

void testApp::setup(){
    printf("testApp setup() が呼ばれました。\n");
    
    ofSetVerticalSync(true);
//fenster
	ofxFenster * win = ofxFensterManager::get()->createFenster(0 , 0, 1280, 800, OF_WINDOW);
	win -> addListener(new uiWindow());
    ui.setup();
    ofxFensterManager::get()->getPrimaryWindow()->setWindowPosition(-1, 182);

//--fenster
    
//xtion--
    xtions.setup();
    ofFbo::Settings s = ofFbo::Settings();
    s.width = PROJECTION_SIZE_W;
    s.height = PROJECTION_SIZE_H;
    s.internalformat = GL_RGB;
    s.useDepth = false;
    s.depthStencilAsTexture = false;
    xtionFbo.allocate(s);
    screenFbo.allocate(s);
    monitorFbo.allocate(s);
    
    tex.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, GL_LUMINANCE);
    pix.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, 3);
    cImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    gImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    
//--xtion
}

//--------------------------------------------------------------
void testApp::update(){
	//xtion--
    xtionFbo.begin();
        ofClear(0,0,0,255);
    xtionFbo.end();
    screenFbo.begin();
        ofClear(0, 0, 0, 255);
    screenFbo.end();
    monitorFbo.begin();
    ofClear(0, 0, 0, 255);
    monitorFbo.end();
    
    xtions.update();
    
    xtionFbo.begin();
        xtions.testDraw();
    xtionFbo.end();
    
    
    
    tex = xtionFbo.getTextureReference();
    tex.readToPixels(pix);//このpixにはしっかり入ってる。
    cImg.setFromPixels(pix);
    gImg = cImg;
    cfinder.findContours(gImg, minArea, maxArea, nConsidered, bFindHoles, bUseApproximation);
    
    if (counter == 100) {

    }
    
    //--xtion
    counter++;
}

//--------------------------------------------------------------
void testApp::draw(){
    monitorFbo.begin();
    screenFbo.begin();
    //Drawing point////////////////////////////////////
    ofSetColor(255);
    //gImg.draw(0.0f, 0.0f);
    cfinder.draw();
    for (int i = 0; i < 4; i++) {
        ofSetColor(ofColor::white);
        ofCircle(testPoint[i], 20);
    }
    
    
    
    ///////////////////////////////////////////////////
    screenFbo.end();
    screenFbo.draw(0, 0);
    mappingDraw();
    monitorFbo.end();
    
    screenFbo.draw(0,0);
}

void testApp::mappingDraw(){
    if (!bMappingDraw) return;
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 255, 66);
    ofSetLineWidth(lineWidth);
    ofBeginShape();
    
    for (int i = 0; i < 4; i++) {
        ofVertex(maguchi[i]);
        if (i == 3) ofVertex( maguchi[0] );
    }
    ofEndShape();
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    if (bFilled[4]) {
        ofFill();
    }
    ofSetLineWidth(lineWidth);
    ofSetColor(255, 66, 255);
    ofBeginShape();
    for (int i = 0; i < 4; i++) {
        ofVertex(okuguchi[i]);
        if (i == 3) ofVertex(okuguchi[0]);
    }
    ofEndShape();
    
    ofBeginShape();
    ofSetLineWidth(lineWidth);
    ofSetColor(255, 255, 255);
    for (int i = 0; i < 4; i++) {
        ofLine(maguchi[i], okuguchi[i]);
    }
    ofEndShape();
    
    for (int i = 0; i < 3; i++) {
        if (bFilled[i]) {//上
            ofPushStyle();
            ofFill();
            ofSetColor(255);
            ofBeginShape();
            ofVertex(maguchi[i]);
            ofVertex(maguchi[i+1]);
            ofVertex(okuguchi[i+1]);
            ofVertex(okuguchi[i]);
            ofEndShape();
            ofPopStyle();
        }
    }
    if (bFilled[3]) {
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofBeginShape();
        ofVertex(maguchi[3]);
        ofVertex(maguchi[0]);
        ofVertex(okuguchi[0]);
        ofVertex(okuguchi[3]);
        ofEndShape();
        ofPopStyle();
    }
    
    if (bDoorDraw) {
        ofPushStyle();
        ofFill();
        ofSetColor(0, 255, 0);
        ofBeginShape();
        ofVertex(doorPos[0]);
        ofVertex(doorPos[1]);
        ofVertex(doorPos[2]);
        ofVertex(doorPos[3]);
        ofEndShape();
        ofPopStyle();
        
        ofFill();
        ofSetColor(0, 0, 255);
        ofCircle(doorPos[4].x, doorPos[4].y, 10.0f);
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'r') {
        xtions.startOniRecording(0);
        printf("recStart\n");
    } else if (key == 's') {
        xtions.startOniRecording(0);
        printf("recStop\n");
    } else if (key == 'f'){
            ofxFensterManager::get()->getPrimaryWindow()->toggleFullscreen();
    }
       
}
