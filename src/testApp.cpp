#include "testApp.h"

////GLOBAL////
extern ofFloatColor mainPink;
extern ofFbo screenFbo;
extern ofFbo monitorFbo;
extern ofPoint mahoPos[2];
extern bool bBackWhite;
extern bool currentScene;
extern bool bTail;
extern bool trTail;

//xtion
extern ofFbo xtionFbo;
extern myXtionOperator xtions;
extern bool trSaveDepthByFrame[XTION_NUM];//new!
extern bool trStopSaveDepthByFrame[XTION_NUM];
extern int choicedXtion;


//grotesq
extern bool bGrotesq;
extern grotes grotesq;

//harada fire
extern bool bHaradaFire;
extern grotes haradaFire;

//OpenCv
extern bool bOpenCv;
extern bool bCfinderDraw;
extern int gImgThreshold;
extern bool bCvBlur;
extern bool bCvNoise;
extern float cvNoiseAmt;
extern ofColor openCvDrawColor;
extern bool toggleCvFadeOut;
//missing opencv
extern bool bDrawScene1Blob;
extern ofPoint scene1Blob[6];
extern ofPoint seraBlob[6];
extern bool bDrawSeraBlob;
extern bool bSeraHeadMappingDraw;
extern ofPoint seraHeadPos[1];

//maho
extern bool bKirakiraDraw;
extern ofPoint kirakiraPos[1];


//mapping
extern ofPoint gekijo[8];
extern bool bMappingDraw;
extern bool bMappingDrawOnMonitor;
extern float lineWidth;
extern ofPoint rokkaPos[4];
extern bool bRokkaDraw;
extern bool bSeraBlobMappingDraw;

extern ofPoint targetPointA;
extern ofPoint doorPos[6];
extern ofPoint doorPosDamy[6];
extern bool bDoorDraw;

extern bool bDrawMesh;
extern int minArea;//11
extern int maxArea;//12
extern int nConsidered;
extern bool bFindHoles;
extern bool bUseApproximation;
//maho/
extern bool trKeen;
extern bool trMaho;
extern bool mahoIndex;

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
extern bool trHimawariLoad;
extern bool trKajiLoad;

//mapping
extern bool bHondanaDraw;
extern ofPoint hondanaPoints[6];
extern ofPoint rokkaPosDamy[4];

//setting
extern bool bMaskScreen;
////GLOBAL----

void testApp::setup(){
    printf("testApp setup() が呼ばれました。\n");
    ofSetFrameRate(30);
    ofSetVerticalSync(false);
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
    mainMaskFbo.allocate(s);
    
    //openCv
    tex.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, GL_LUMINANCE);
    pix.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, 3);
    cImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    gImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    //grotesq
    
    //mask
    mainMaskImg.allocate(PROJECTION_SIZE_W, PROJECTION_SIZE_H, OF_IMAGE_COLOR_ALPHA);
    maskShader.load("common.vert","mult.frag");
    mainMaskImg.loadImage("screenMask.png");
    //videos
    himawari.setPixelFormat(OF_PIXELS_RGB);
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
	himawari.loadMovie("himawari.mov", decodeMode);
    kaji.setPixelFormat(OF_PIXELS_RGB);
	kaji.loadMovie("fire2_glow.mp4", decodeMode);
    himawariAlpha.set(himawariPos.x, himawariPos.y, 500.0f, 500.0f);
    kajiAlpha.set(0.0f, 0.0f, 1280.0f, 960.0f);
    himawari.setSpeed(2.5f);
    himawari.setLoopState(OF_LOOP_NONE);
    kaji.setLoopState(OF_LOOP_NONE);
    

//--xtion
}

float fadeWhite = 0;
int WhiteFading = 0;
int milli = 0;
int FadingStartTime = 0;
void testApp::fadingWhite(){
    if (WhiteFading == 0) return;
    else if (WhiteFading == 1) {
        int a = round(255.0f * (ofGetElapsedTimeMillis() - FadingStartTime) / milli);
        if (a >= 255.0f) {
            fadeWhite = 255;
            WhiteFading = 0;
        } else fadeWhite = a;
        
    } else if (WhiteFading == 2) {
        int a = round(fabs(255 - 255 * (ofGetElapsedTimeMillis() - FadingStartTime) / milli));
        if (a <= 0) {
            fadeWhite = 0;
            WhiteFading = 0;
        } else fadeWhite = a;
    }
}

//--------------------------------------------------------------
void testApp::update(){
    if (trTail) {
        if (bTail) {
            ofSetBackgroundAuto(true);
            bTail = false;
        } else {
            ofSetBackgroundAuto(false);
            bTail = true;
        }
        trTail = false;
    }
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
    
    //magic
    maho.update();
    if (trMaho) {
        maho.toggleSpark();
        trMaho = false;
    }
    if (trKeen) {
        maho.toggleKeen();
        trKeen = false;
    }
    
    //maho
    //kirakira
    kirakira.update(kirakiraPos[0].x, kirakiraPos[0].y);

    //openCv
    if (bOpenCv) {
        fboReader.readToPixels(xtionFbo, pix);
        cImg.setFromPixels(pix);
        gImg = cImg;
        if (bCvBlur && !bDrawMesh) {
            gImg.blurHeavily();
            gImg.threshold(gImgThreshold);
        }
        if (!bDrawMesh) cfinder.findContours(gImg, minArea, maxArea, nConsidered, bFindHoles, bUseApproximation);
    }
    
    
    //movies himawari
    if (trHimawariClear) {
        himawari.closeMovie();
        trHimawariClear = false;
        bHimawariVideoPlaying = false;
        bHimawariAlphaStart = false;
    }
    if (trPlayHimawariVideo) {
        fadeCurrentTimeMilis = ofGetElapsedTimeMillis();
        if (himawari.isLoaded()) {
            himawari.setFrame(0);
            himawari.play();
            bHimawariVideoPlaying = true;
            trPlayHimawariVideo = false;
            ct = 0;
            videoMaskColor.set(0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            printf("Himawari is not loaded!!\n");
            trPlayHimawariVideo = false;
        }
    }
    if (bHimawariVideoPlaying) himawari.update();
    if (bHimawariAlphaStart) {
        if (ct == 0) {
            fadeCurrentTimeMilis = ofGetElapsedTimeMillis();
        }
        ct++;
    }
    if (trHimawariLoad) {
        himawari.setPixelFormat(OF_PIXELS_RGB);
        ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
        himawari.loadMovie("himawari.mov", decodeMode);
        trHimawariLoad = false;
        himawari.setFrame(0);
        himawari.setSpeed(2.0f);
        himawari.setLoopState(OF_LOOP_NONE);
    }
    
    //video kaji
    if (trKajiClear) {
        kaji.closeMovie();
        trKajiClear = false;
        bKajiVideoPlaying = false;
        bKajiAlphaStart = false;
    }
    if (trPlayKajiVideo) {
        fadeCurrentTimeMilis = ofGetElapsedTimeMillis();
        if (kaji.isLoaded()) {
            kaji.setFrame(0);
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
    
    if (bKajiAlphaStart) {
        if (ct == 0) {
            fadeCurrentTimeMilis = ofGetElapsedTimeMillis();
        }
        ct++;
    }
    if (bKajiVideoPlaying) kaji.update();
    if (trKajiLoad) {
        kaji.setPixelFormat(OF_PIXELS_RGB);
        ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
        kaji.loadMovie("fire2_glow.mp4", decodeMode);
        trKajiLoad = false;
        kaji.setLoopState(OF_LOOP_PALINDROME);
    }
    //
    if (counter == 100) {

    }
    mainMaskFbo.begin();
    ofBackground(0, 0, 0, 0);
    ofSetColor(255, 255, 255, 255);
    mainMaskImg.draw(0.0f, 0.0f);
    mainMaskFbo.end();
    //--xtion
    
    //grotes
    grotesq.update();
    if(bHaradaFire) haradaFire.update();
    
    //custum
    fadingWhite();
    
    //shader
    counter++;
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofBackground(0);
    if (bTail) {
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(0, 0, 0, 100);
        ofRect(0.0f, 0.0f, 1280.0f, 960.0f);
        ofDisableAlphaBlending();
        ofPopStyle();
    }
    ofDisableAlphaBlending();
    ofPushStyle();//style1
    //ofSetColor(0, 0, 0, 255);
    //ofRect(0.0f, 0.0f, PROJECTION_SIZE_W, PROJECTION_SIZE_H);
    monitorFbo.begin();
    screenFbo.begin();
    //Drawing point////////////////////////////////////
    //ofSetColor(255);// white set
    if (bBackWhite) {
        ofPushStyle();
        ofFill();
        //ofEnableAlphaBlending();
        ofSetColor(fadeWhite);
        ofRect(0.0f, 0.0f, 1280, 960);
        ofPopStyle();
    }
    ofPushStyle();
    
    //himawari
    if (bHimawariVideoPlaying) {
        ofEnableAlphaBlending();
        if (bHimawariAlphaStart) {
            int a = ofGetElapsedTimeMillis() - fadeCurrentTimeMilis;
            a = round(ofClamp((float)a, 0.0f, 1000.0f));
            int newA = round(ofMap((float)a, 0.0f, 1000.0f, 0.0f, 255.0f));
            ofSetColor(255 - newA);
        } else ofSetColor(255);
        himawari.draw(himawariPos.x, himawariPos.y, 500.0f, 500.0f);
    }
    ofPopStyle();
    
    //kaji
    
    if (bKajiVideoPlaying) {
        ofPushStyle();
        ofEnableAlphaBlending();
        if (!bKajiAlphaStart) {
            int sa = ofGetElapsedTimeMillis() - fadeCurrentTimeMilis;
            sa = round(ofClamp((float)sa, 0.0f, 1000.0f));
            int newsA = round(ofMap((float)sa, 0.0f, 1000.0f, 0.0f, 255.0f));
            ofSetColor(newsA);
        } else if (bKajiAlphaStart) {
            int a = ofGetElapsedTimeMillis() - fadeCurrentTimeMilis;
            a = round(ofClamp((float)a, 0.0f, 1000.0f));
            int newA = round(ofMap((float)a, 0.0f, 1000.0f, 0.0f, 255.0f));
            ofSetColor(255 - newA);
        } else ofSetColor(255);
        
        kaji.draw(0.0f, 0.0f, 1280.0f, 960.0f);
        ofPopStyle();
    }
    
    if (bDrawScene1Blob){
        ofPushStyle();//style2);
        ofFill();
        bool bDrawRokka = true;
        if (toggleCvFadeOut) {
            if(openCvDrawColor.r >= 2)openCvDrawColor.r -= 2;
            if(openCvDrawColor.g >= 2) openCvDrawColor.g -= 2;
            if(openCvDrawColor.b >= 2) openCvDrawColor.b -= 2;
            if (openCvDrawColor.r < 2)  bDrawRokka = false;
        }
        if (bDrawRokka) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            ofSetColor(openCvDrawColor);
            ofBeginShape();
            for (int i = 0; i < 4; i++) {
                ofVertex(rokkaPos[i]);
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 0; i < 4; i++) {
                ofVertex(rokkaPosDamy[i].x + ofRandomf() * 20.0f, rokkaPosDamy[i].y + ofRandomf() * 20.0f);
            }
            ofEndShape();
            ofBeginShape();
            for (int i = 0; i < 4; i++) {
                ofVertex(rokkaPosDamy[i].x + ofRandomf() * 20.0f, rokkaPosDamy[i].y + ofRandomf() * 20.0f);
            }
            ofEndShape();
            glDisable(GL_BLEND);
        }
        ofPopStyle();//style2
    }
    
    
    
    ofPopStyle();//style1
    
    //maho main
    ofPushStyle();
    ofDisableAlphaBlending();
    glEnable(GL_BLEND);//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    glBlendFunc(GL_ONE, GL_ONE);
    ofSetCircleResolution(12);
    maho.draw(mahoPos[0].x, mahoPos[0].y);
    //kirakira
    kirakira.draw();
    if (bHaradaFire) {
        haradaFire.draw();
    }
    glDisable(GL_BLEND);
    ofPopStyle();
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    if (bDrawMesh) {
        ofPushStyle();
        ofSetColor(255);
        xtionFbo.draw(0.0f, 0.0f);
        ofPopStyle();
    }
    if (bGrotesq) {
        grotesq.draw();
    }
    
    if (bMappingDraw) mappingDraw();
    //--drawPoint/////////////////////////////////////////////////
    screenFbo.end();
    //Only Monitor///////////////////////////////////////////////
    screenFbo.draw(0, 0);
    if (bMappingDrawOnMonitor) mappingDraw();
    if (bOpenCv && bCfinderDraw) cfinder.draw();
    
    ////--only monitor////////////////////////////////////
    monitorFbo.end();
    if(bMaskScreen) {
        maskShader.begin();
        maskShader.setUniformTexture("tex2",mainMaskFbo.getTextureReference(), 1);
    }
    ofSetColor(255);
    screenFbo.draw(0,0);
    if (bMaskScreen) maskShader.end();
}

void testApp::mappingDraw(){
    ofPushStyle();
    ofPushMatrix();
    ofNoFill();
    ofSetColor(255, 255, 66);
    ofSetLineWidth(lineWidth);
    ofBeginShape();
    
    for (int i = 0; i < 4; i++) {
        ofVertex(gekijo[i]);
        if (i == 3) ofVertex( gekijo[0] );
    }
    ofEndShape();
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(255, 66, 255);
    ofBeginShape();
    for (int i = 4; i < 8; i++) {
        ofVertex(gekijo[i]);
        if (i == 7) ofVertex(gekijo[4]);
    }
    ofEndShape();
    
    ofBeginShape();
    ofSetLineWidth(lineWidth);
    ofSetColor(255, 255, 255);
    for (int i = 0; i < 4; i++) {
        ofLine(gekijo[i], gekijo[i + 4]);
    }
    ofEndShape();
    
    if (bDoorDraw) {
        ofPushStyle();
        ofFill();
        ofSetColor(0, 255, 0);
        ofBeginShape();
        ofVertex(doorPos[0]);
        ofVertex(doorPos[1]);
        ofVertex(doorPos[2]);
        ofVertex(doorPos[3]);
        ofVertex(doorPos[4]);
        ofVertex(doorPos[5]);
        ofEndShape();
        ofPopStyle();
        
    }
    ofPopMatrix();
    
    if (bRokkaDraw) {
        ofPushStyle();
        ofFill();
        ofSetColor(0, 255, 0);
        ofBeginShape();
        ofVertex(rokkaPos[0]);
        ofVertex(rokkaPos[1]);
        ofVertex(rokkaPos[2]);
        ofVertex(rokkaPos[3]);
        ofEndShape();
        ofPopStyle();
    }
    
    if (bHondanaDraw) {
        ofPushStyle();
        ofFill();
        ofSetColor(0, 255, 0);
        ofBeginShape();
        ofVertex(hondanaPoints[0]);
        ofVertex(hondanaPoints[1]);
        ofVertex(hondanaPoints[2]);
        ofVertex(hondanaPoints[3]);
        ofVertex(hondanaPoints[4]);
        ofVertex(hondanaPoints[5]);
        ofEndShape();
        ofPopStyle();
    }
    
//    if (bSeraBlobMappingDraw) {
//        ofPushStyle();
//        ofFill();
//        ofSetColor(0, 255, 0);
//        ofBeginShape();
//        ofVertex(seraBlob[0]);
//        ofVertex(seraBlob[1]);
//        ofVertex(seraBlob[2]);
//        ofVertex(seraBlob[3]);
//        ofVertex(seraBlob[4]);
//        ofVertex(seraBlob[5]);
//        ofEndShape();
//        ofPopStyle();
//    }
    if (bSeraHeadMappingDraw) {
        ofFill();
        ofSetColor(255, 255, 0);
        ofCircle(seraHeadPos[0].x, seraHeadPos[0].y, 10.0f);
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'w') {
        bBackWhite = !bBackWhite;
    } else if (key == 't') {
        bTail = !bTail;
    } else if (key == ' '){
        
    }
       
}
void testApp::mouseDragged(int x, int y, int button){
}

