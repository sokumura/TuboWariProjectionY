#include "grotesParticle.h"

extern ofPoint mahoPos[2];
extern bool bHaradaFire;
extern bool bHaradaFireFadeOut;
extern float haradaFireFadeOutAmt;

//------------------------------------------------------------------
void grotesParticle::toggleGo(){
    if (bHaradaFire) {
        pos.x = 1500 * cos(TWO_PI * ofRandomf());
        pos.y = 1500 * sin(TWO_PI * ofRandomf()) - 1000.0f;// - 1500.0f;
        color = ofColor(255, 20, 0);
    } else {
        pos.x = 1500 * cos(TWO_PI * ofRandomf()) - 1500.0f;
        pos.y = 1500 * sin(TWO_PI * ofRandomf());// - 1500.0f;
	}
    if (bHaradaFire) {
        vel.x = -10.0f;//ofRandom(-3.9 * 3, 3.9 * 3);
        vel.y = 10.0f;//ofRandom(-3.9 * 3, 3.9 * 3);
    }
	vel.x = 2.0f;//ofRandom(-3.9 * 3, 3.9 * 3);
	vel.y = 0.0f;//ofRandom(-3.9 * 3, 3.9 * 3);
	
	frc   = ofPoint(0.0f,0.0f, 0.0f);
	
	scale = ofRandom(3.5, 6.0);
	drag  = ofRandom(0.6, 0.998);
    color = ofColor(255, 60, 10, 255);
}

//------------------------------------------------------------------
void grotesParticle::update(){
    ofPoint attractPt;
    if (bHaradaFire){
        attractPt.set(mahoPos[1].x, mahoPos[1].y);
    } else attractPt.set(mahoPos[0].x, mahoPos[0].y);
    
    frc = attractPt-pos;;
    frc.normalize();
    vel *= drag;
    vel += frc * 5.0;
	if (bHaradaFire) {
        pos += vel * ofRandom(5.0f, 20.0f);
    } else {
        pos += vel;
    }
}

//------------------------------------------------------------------
void grotesParticle::draw(){
//    ofDisableAlphaBlending();
    if (bHaradaFire) {
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofFill();
        if (bHaradaFireFadeOut) {
            fadeOut();
            ofSetColor (color);
        } else {
            ofSetColor (color);
        }
        ofCircle(pos.x, pos.y, scale * 0.6);
    } else {
        ofSetColor(0, 0, 0);
        ofCircle(pos.x, pos.y, scale);
    }
}

void grotesParticle::fadeOut(){
    if (color.r > 20) color.r -= 20;
    else if(color.r > 10) color.r -= 3;
    else if(color.r > 0) color.r --;
    if (color.g > 4) color.g -= 4;
    else if (color.g > 0) color.g --;
    if (color.b > 2) color.b -= 2;
    else if (color.b > 0) color.b--;
    
}
