#pragma once
#include "ofMain.h"

extern ofPoint seraPos[1];

class seraParticle{

	public:
    seraParticle(){};

		void toggleGo();
		void update();
		void draw();		
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float scale;
};