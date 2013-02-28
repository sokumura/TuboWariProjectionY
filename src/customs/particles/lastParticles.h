#pragma once

#include "ofMain.h"
#include "grotesParticle.h"

struct lastParticle {
    ofPoint pos;
    float speed;
    float radius;
};

class lastParticles {

	public:
		void setup();
		void update();
		void draw();
		void toggleGo();
    void clear();
    
		vector <lastParticle> particles;
        void fadeOut();
    void compalsion();
private:
    int time;
    bool bFadeOut;
    bool bakuhatu;
    int num;
};
