#pragma once

#include "ofMain.h"
#include "seraParticle.h"

class seraNoise {

	public:
		void setup();
		void update();
		void draw();
		void toggleGo();
    void clear();
    
		vector <seraParticle> p;
private:
    int time;
		
};
