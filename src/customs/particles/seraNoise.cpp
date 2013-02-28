#include "seraNoise.h"

//--------------------------------------------------------------
void seraNoise::setup(){
	ofSetVerticalSync(true);
    ofSetFrameRate(15);
	
	int num = 1000;
	p.assign(num, seraParticle());
}

//--------------------------------------------------------------
void seraNoise::update(){
	for(int i = 0; i < p.size(); i++){
		p[i].update();
	}
}

//--------------------------------------------------------------
void seraNoise::draw(){
    ofPushStyle();
    ofDisableAlphaBlending();
    ofFill();
	for(int i = 0; i < p.size(); i++){
		p[i].draw();
	}
    ofPopStyle();
}
void seraNoise::toggleGo(){
    for (int i = 0; i < p.size(); i++) {
        p[i].toggleGo();
    }
}

void seraNoise::clear(){
    p.clear();
}