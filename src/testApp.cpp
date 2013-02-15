#include "testApp.h"

////GLOBAL////
extern bool bOpenCv;
extern bool bCfinderDraw;

extern ofFbo screenFbo;
extern ofFbo monitorFbo;
extern ofPoint magicPos[2];
//xtion
extern ofFbo xtionFbo;
extern myXtionOperator xtions;
extern bool bXtionUpdate;

//mapping
extern bool bMappingDraw;
extern bool bMappingDrawOnMonitor;
extern float lineWidth;
extern ofPoint maguchi[4];
extern ofPoint okuguchi[4];
extern bool bFilled[5];
extern ofPoint targetPointA;
extern ofPoint doorPos[5];
extern bool bDoorDraw;

extern bool bDrawMesh;
extern int minArea;//11
extern int maxArea;//12
extern int nConsidered;
extern bool bFindHoles;
extern bool bUseApproximation;
//magic
extern int lineNumber;
extern int bunshiNum;
extern int line2Num;
extern bool trGoko1;
//inazuma
extern int inazumaNo;
extern bool trInazumaLoad;
extern bool bInazumaDraw;
extern ofImage inazuma;
//videos
extern ofQTKitPlayer himawari;
extern ofQTKitPlayer kaji;
extern ofPoint himawariPos;
extern ofPoint kajiPos;
extern bool trPlayHimawariVideo;
extern bool bHimawariVideoPlaying;
extern bool bHimawariAlphaStart;
extern bool trHimawariClear;
extern bool trPlayKajiVideo;
extern bool bKajiVideoPlaying;
extern bool bKajiAlphaStart;
extern bool trKajiClear;
extern int fadeTimeMilis;
////GLOBAL----

void testApp::setup(){
    printf("testApp setup() が呼ばれました。\n");
    
    ofSetVerticalSync(true);
//fenster
	ofxFenster * win = ofxFensterManager::get()->createFenster(0 , 0, 1280, 800, OF_WINDOW);
	win -> addListener(new uiWindow());
    ui.setup();
    ofxFensterManager::get()->getPrimaryWindow()->setWindowPosition(-1000, 182);
    ofxFensterManager::get()->getPrimaryWindow()->toggleFullscreen();

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
    s.internalformat = GL_RGBA;
    mainMaskFbo.allocate(s);
    
    //inazuma
    inazuma.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H,OF_IMAGE_COLOR_ALPHA);//グレースケールにしたい
    inazuma.loadImage("hibi1.png");
    
    //openCv
    tex.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, GL_LUMINANCE);
    pix.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, 3);
    cImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    gImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    
    //mask
    mainMaskImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, OF_IMAGE_COLOR_ALPHA);
    maskShader.load("common.vert","mult.frag");
    mainMaskImg.loadImage("screenMask.png");
    //videos
    himawari.setPixelFormat(OF_PIXELS_RGB);
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
	//himawari.loadMovie("movies/fire2_glow.mp4", decodeMode);
    kaji.setPixelFormat(OF_PIXELS_RGB);
	kaji.loadMovie("fire2_glow.mp4", decodeMode);
    //himawariAlpha.set(himawariPos.x, himawariPos.y, <#float w#>, <#float h#>);
    kajiAlpha.set(0.0f, 0.0f, 1280.0f, 960.0f);
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
    if (bXtionUpdate) {
        xtions.update();
    
        xtionFbo.begin();
            xtions.testDraw();
        xtionFbo.end();
    }
    //inazuma
    if (trInazumaLoad) {
        inazuma.clear();
        bool check = false;
        check = inazuma.loadImage("hibi" + ofToString(int(inazumaNo + 1)) + ".png");
        //TO DO CHECK
        printf("稲妻ロード[%u]の成功 : %u\n", inazumaNo + 1,(int)check);
        trInazumaLoad = false;
    }
    //magic
    currentMahoTime = ofGetElapsedTimeMillis() - startMahoTime;
    goko1.update();
    goko1.toggleSpark(lineNumber, bunshiNum, line2Num, trGoko1);
    if (trKeen) {
        goko1.setOff();
        keen1.toggleKeen();
    }
    //-magic
    //openCv
    if (bOpenCv) {
        tex = xtionFbo.getTextureReference();
        tex.readToPixels(pix);//このpixにはしっかり入ってる。
        cImg.setFromPixels(pix);
        gImg = cImg;
        cfinder.findContours(gImg, minArea, maxArea, nConsidered, bFindHoles, bUseApproximation);
    }
    //movies
    if (trKajiClear) {
        kaji.closeMovie();
        trKajiClear = false;
        bKajiVideoPlaying = false;
        bKajiAlphaStart = false;
    }
    if (trPlayKajiVideo) {
        fadeCurrentTimeMilis = ofGetElapsedTimeMillis();
        if (kaji.isLoaded()) {
            kaji.play();
            bKajiVideoPlaying = true;
            trPlayKajiVideo = false;
            ct = 0;
            videoMaskColor.set(0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            printf("kaji is not loaded!!\n");
            trPlayKajiVideo = false;
        }
    }
    if (bKajiVideoPlaying) kaji.update();
    if (bKajiAlphaStart) {
        if (ct == 0) {
            fadeCurrentTimeMilis = ofGetElapsedTimeMillis();
        }
        ct++;
    }
    
    //
    if (counter == 100) {

    }
    
    //--xtion
    //shader
    maskShader.load("common.vert","mult.frag");
    mainMaskImg.allocate(1280, 960, OF_IMAGE_COLOR_ALPHA);
    mainMaskImg.loadImage("screenMask.png");
    counter++;
}

//--------------------------------------------------------------
void testApp::draw(){
    mainMaskFbo.begin();
    ofBackground(0, 0, 0, 0);
    ofSetColor(255, 255, 255, 255);
    //ofCircle(mouseX, mouseY, 500.0f);
    //ofRect(0,mouseY,ofGetWidth(),200);
    mainMaskImg.draw(0.0f, 0.0f);
    mainMaskFbo.end();
    ofPushStyle();
    ofPushMatrix();
    ofBackground(ofColor::gray);
    ofSetColor(0, 0, 0, 255);
    ofRect(0.0f, 0.0f, PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    monitorFbo.begin();
    screenFbo.begin();
    //Drawing point////////////////////////////////////
    ofSetColor(255, 255, 255, 255);
    if (bKajiVideoPlaying) {
        
        kaji.draw(0.0f, 0.0f, 1280.0f, 960.0f);
        if (!bKajiAlphaStart) {
            int sa = ofGetElapsedTimeMillis() - fadeCurrentTimeMilis;
            int newsA = round(ofMap((float)sa, 0.0f, 1000.0f, 0.0f, 255.0f));
            ofSetColor(0,0,0,255 - newsA);
            ofRect(kajiAlpha);
        }
        
        if (bKajiAlphaStart) {
            int a = ofGetElapsedTimeMillis() - fadeCurrentTimeMilis;
            int newA = round(ofMap((float)a, 0.0f, 1000.0f, 0.0f, 255.0f));
            ofSetColor(0,0,0,newA);
            ofRect(kajiAlpha);
        }
    }
    ofSetColor(255, 255, 255, 255);
    if (bDrawMesh) gImg.draw(0.0f, 0.0f);
    if (bOpenCv && bCfinderDraw) cfinder.draw();
    if (bMappingDraw) mappingDraw();
    ofPushStyle();
    ofSetCircleResolution(12);
    ofEnableAlphaBlending();
    glBlendFunc(GL_ONE, GL_ONE);
    goko1.draw(magicPos[0].x, magicPos[0].y);
    keen1.draw();
    ofPushStyle();
    if (bInazumaDraw) inazuma.draw(0.0f, 0.0f);
    ///////////////////////////////////////////////////
    screenFbo.end();
    screenFbo.draw(0, 0);
    if (!bMappingDraw && bMappingDrawOnMonitor) mappingDraw();
    monitorFbo.end();
    maskShader.begin();
    maskShader.setUniformTexture("tex2",mainMaskFbo.getTextureReference(), 1);
    ofSetColor(255, 255, 255, 255);
    ofSetColor(255, 255, 255, 255);
    screenFbo.draw(0,0);
    maskShader.end();
    ofPopMatrix();
    ofPopStyle();
}

void testApp::mappingDraw(){
    ofPushStyle();
    ofPushMatrix();
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
    ofPopMatrix();
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
            
    }
       
}
