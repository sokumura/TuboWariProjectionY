#pragma once

#include "uiWindow.h"
//#include "ofxOpenCv.h"
//#include "ofxCv.h"



class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key){};
    void mouseMoved(int x, int y ){};
    void mouseDragged(int x, int y, int button){};
    void mousePressed(int x, int y, int button){};
    void mouseReleased(int x, int y, int button){};
    void windowResized(int w, int h){};
    void dragEvent(ofDragInfo dragInfo){};
    void gotMessage(ofMessage msg){};
    
    void mappingDraw();
    void contourFinderSetup();
    
    //xtion
    ofFbo xtionFbo;
    
    ofTexture tex, testTex;
    ofxCv::ContourFinder cfinder;
    
    //fenster
    uiWindow ui;
    int counter;
    
    cv::Mat mat;

};
