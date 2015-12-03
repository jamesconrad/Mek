#pragma once

#include "2dOverlay.h"

class twodOverlayAnim
{
public:
	twodOverlayAnim(char* fp, unsigned int frames, float totalTime);
	//begin the animation
	void play();
	//change the scale and position of the anim
	void updatePos(float x, float y, float scale);
	//called per frame
	void update(float dTime);
	//draw
	void render();
	//change the total time for the animation to run
	void updateAnimTime(float seconds);
	//go through the animation in reverse once done
	bool cycle = false;
private:
	unsigned int stage;
	unsigned int numFrames;
	float animtime;
	float frametime;
	bool done;
	twodOverlay* _idleAnim;
	std::vector<twodOverlay*> _hudvec;
};