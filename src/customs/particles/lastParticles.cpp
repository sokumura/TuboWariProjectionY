#include "lastParticles.h"

extern ofFloatColor mainPink;

//--------------------------------------------------------------
void lastParticles::setup(){
	ofSetVerticalSync(true);
    ofSetFrameRate(30);
	
	num = 500;
	for (int i = 0; i < num; i++){
        lastParticle p;
        p.pos = ofPoint(ofRandom(0, 1280), ofRandom(1000.0f, 5000.0f));
        p.speed = (0.0f, (-1) * ofRandom(1000.0f, 5000.0f));
        p.radius = ofRandom(4.0f, 30.0f);
        particles.push_back(p);
    }
    
    bakuhatu = false;
    
}

//--------------------------------------------------------------
void lastParticles::update(){
    
    for (int i = 0; i < particles.size(); i++){
        particles[i].pos += particles[i].speed;
        particles[i].speed /= 3.0f;
    }
}

//--------------------------------------------------------------
void lastParticles::draw(){
    ofDisableAlphaBlending();
    ofSetColor(mainPink);
    for (int i = 0; i < particles.size(); i++) {
        ofCircle(particles[i].pos.x, particles[i].pos.y, particles[i].radius);
    }
}