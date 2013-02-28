#include "grotes.h"

extern int grotesParticlesNum;

//--------------------------------------------------------------
void grotes::setup(){
	ofSetVerticalSync(true);
    ofSetFrameRate(30);

	int num = 10000;//grotesParticlesNum;
	p.assign(num, grotesParticle());
    
    bakuhatu = false;
    
}

//--------------------------------------------------------------
void grotes::update(){
	for(int i = 0; i < p.size(); i++){
		p[i].update();
	}
    if (bFadeOut) {
        for (int i = 0; i < p.size(); i++) {
            p[i].fadeOut();
        }
    }
    if (bakuhatu) {
        for (int i = 0; i < p.size(); i++) {
            
        }
    }
    
}

//--------------------------------------------------------------
void grotes::draw(){
    ofPushStyle();
    ofFill();
	for(int i = 0; i < p.size(); i++){
		p[i].draw();
	}
    ofPopStyle();
}
void grotes::toggleGo(){
    for (int i = 0; i < p.size(); i++) {
        p[i].toggleGo();
    }
}

void grotes::compalsion(){
    bakuhatu = true;
}

void grotes::clear(){
    p.clear();
}

void grotes::fadeOut(){
    bFadeOut = true;
}