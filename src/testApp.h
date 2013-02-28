#pragma once

#include "uiWindow.h"
#include "Maho.h"
#include "ofxFastFboReader.h"
#include "grotes.h"


class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key){};
    void mouseMoved(int x, int y ){};
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button){};
    void mouseReleased(int x, int y, int button){};
    void windowResized(int w, int h){};
    void dragEvent(ofDragInfo dragInfo){};
    void gotMessage(ofMessage msg){};
    
    void mappingDraw();
    
    //xtion
    
    ofPixels pix;
    ofTexture tex;
    ofxCvColorImage cImg;
    ofxCvGrayscaleImage gImg;
    ofxCvContourFinder cfinder;
    ofxFastFboReader fboReader;
    
    //fenster
    uiWindow ui;
    int counter;
    
    //openCv
    
    //grotesQ
    
    
    //magic
    Maho maho;
    
    //another
    Kirakira kirakira;
    
    //mask
    ofShader maskShader;
    ofFbo mainMaskFbo;
    ofImage mainMaskImg;
    
    //videos
    ofRectangle himawariAlpha;
    ofRectangle kajiAlpha;
    ofColor videoMaskColor;
    int fadeCurrentTimeMilis;
    int ct;
    
    //testMonitor
    ofFbo testMonitor;
    
    //custom
    void fadingWhite();
    
    //onetime
    
    
};


