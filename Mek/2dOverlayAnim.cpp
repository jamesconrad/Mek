#include "2dOverlayAnim.h"


twodOverlayAnim::twodOverlayAnim(char* fp, unsigned int frames, float totalTime)
{
	stage = 0;
	numFrames = frames;
	updateAnimTime(totalTime);
	animtime = 0;

	_idleAnim = new twodOverlay(fp, 0, 0, 1);

	std::string rootfp = fp;
	rootfp = rootfp.substr(0, rootfp.length() - 4);
	for (int i = 0; i < numFrames; i++)
	{
		std::string sfp = rootfp;
		char buffer[8];
		_itoa_s(i, buffer, 10);
		sfp.append(buffer);
		sfp.append(".png");
		_hudvec.push_back(new twodOverlay((char*)sfp.c_str(), 0, 0, 1));
	}

	done = true;
}

void twodOverlayAnim::updateAnimTime(float seconds)
{
	frametime = seconds / numFrames;
}

void twodOverlayAnim::updatePos(float x, float y, float scale)
{
	glm::vec3 t = glm::vec3(x, y, scale);
	for (int i = 0; i < numFrames; i++)
		_hudvec[i]->pos = t;
	_idleAnim->pos = t;
}

void twodOverlayAnim::update(float dTime)
{
	if (animtime > frametime && !done)
	{
		animtime = 0;
		stage++;
		if (stage == numFrames && !cycle)
		{
			done = true;
			stage = 0;
		}
		else if (stage == numFrames * 2 && cycle)
		{
			done = true;
			stage = 0;
		}
	}

	animtime += dTime;
}

void twodOverlayAnim::render()
{
	if (done)
		_idleAnim->render();
	else
	{
		if (!cycle)
			_hudvec[stage]->render();
		else
		{
			int frame;
			stage >= numFrames ? frame = numFrames * 2 - (stage + 1) : frame = stage;
			_hudvec[frame]->render();
		}
	}
}

void twodOverlayAnim::play()
{
	if (done)
	{
		done = false;
		stage = 0;
	}
}