#pragma once
#include "ofMain.h"

extern ofPoint mahoPoint[2];

class grotesParticle{

	public:
    grotesParticle(){};

		void toggleGo();
		void update();
		void draw();
    
        void fadeOut();
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float scale;
    ofColor color;
    ofColor exColor;
    
};