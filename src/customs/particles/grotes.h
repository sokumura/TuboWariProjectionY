#pragma once

#include "ofMain.h"
#include "grotesParticle.h"

class grotes {

	public:
		void setup();
		void update();
		void draw();
		void toggleGo();
    void clear();
    
		vector <grotesParticle> p;
        void fadeOut();
    void compalsion();
private:
    int time;
    bool bFadeOut;
    bool bakuhatu;
};
