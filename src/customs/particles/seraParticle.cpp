#include "seraParticle.h"

extern ofPoint mahoPos[2];
extern float seraParticleKasokuAmt;
extern float seraParticleRadius;

//------------------------------------------------------------------
void seraParticle::toggleGo(){
	pos.x = 1500 * cos(TWO_PI * ofRandomf()) - 1500.0f;
	pos.y = 1500 * sin(TWO_PI * ofRandomf());// - 1500.0f;
	
	vel.x = 4.0f;//ofRandom(-3.9 * 3, 3.9 * 3);
	vel.y = 0.0f;//ofRandom(-3.9 * 3, 3.9 * 3);
	
	frc   = ofPoint(1.0f,0.0f);
	
	scale = ofRandom(0.5, 1.0);
	drag  = ofRandom(0.95, 0.998);
}

//------------------------------------------------------------------
void seraParticle::update(){
    ofPoint attractPt(mahoPos[1].x, mahoPos[1].y);
    frc = attractPt-pos;
    frc.normalize();
    vel *= drag;
    vel += frc * seraParticleKasokuAmt;
	pos += vel;
}

//------------------------------------------------------------------
void seraParticle::draw(){
    ofDisableAlphaBlending();
    ofSetColor(0, 0, 0);
	ofCircle(pos.x, pos.y, seraParticleRadius);
}

