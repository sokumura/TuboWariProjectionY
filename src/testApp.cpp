#include "testApp.h"

////GLOBAL////
//xtion
extern myXtionOperator xtions;

//mapping
extern float lineWidth;
extern ofPoint maguchi[4];
extern ofPoint okuguchi[4];
extern bool bFilled[5];
extern ofPoint targetPointA;
extern ofPoint doorPos[5];
extern bool bDoorDraw;
extern ofPoint himawariPos;
////GLOBAL----

void testApp::setup(){
    printf("testApp setup() が呼ばれました。\n");
    
    //ofSetVerticalSync(true);
//fenster
	ofxFenster * win = ofxFensterManager::get()->createFenster(0 , 0, 1280, 800, OF_WINDOW);
	win -> addListener(new uiWindow());
    ui.setup();
    ofxFensterManager::get()->getPrimaryWindow()->setWindowPosition(-1279, 182);
    ofxFensterManager::get()->getPrimaryWindow()->toggleFullscreen();
//--fenster
//xtion--
    xtions.setup();
    ofFbo::Settings s = ofFbo::Settings();
    s.width = 1024;
    s.height = 768;
    s.internalformat = GL_RGBA;
    s.useDepth = false;
    s.depthStencilAsTexture = false;
    xtionFbo.allocate(s);
    
    tex.allocate(1024, 768, GL_LUMINANCE);
    mat = cv::Mat(1024, 768, CV_8UC1);
    contourFinderSetup();
    
//--xtion
}

//--------------------------------------------------------------
void testApp::update(){
	//xtion--
    xtionFbo.begin();
        ofClear(0,0,0,255);
    xtionFbo.end();
    
    xtions.update();
    
    xtionFbo.begin();
        xtions.testDraw();
    xtionFbo.end();
    
//    tex = xtionFbo.getTextureReference();
//    ofPixels pix = ofPixels();
//    tex.readToPixels(pix);//このpixにはしっかり入ってる。
//    mat.data = pix.getPixels();//ここが変なのかな？
//    cfinder.setThreshold(ofMap(mouseY, 0, 1000, 0, 255));//真っ白と真っ黒の境界線を抽出したい。
//    cfinder.findContours(mat);
    //--xtion
    counter++;
}
//--------------------------------------------------------------
void testApp::contourFinderSetup(){
    cfinder.setFindHoles(true);
    cfinder.setMinArea(10.0f);
    cfinder.setMaxArea(1000.0f);
    
}
//--------------------------------------------------------------
void testApp::draw(){
    
    //xtions.testDraw();
    mappingDraw();
    
    //img.draw(0.0f, 0.0f);
    ofSetColor(255);
    xtionFbo.draw(0.0f, 0.0f);
    //tex.draw(0, 0);
    //cfinder.draw();

}

void testApp::mappingDraw(){

    
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
    
       
}
