#ifndef FMODMANAGER_H
#define FMODMANAGER_H
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "include\fmod\fmod.hpp"
#include "include\fmod\fmod_errors.h"
static 	FMOD_RESULT result;
static char name[256];

using namespace std;
void ERRCHECK(FMOD_RESULT result);
char* getfPath();

enum SOUND_TYPE{
	SOUND_TYPE_NULL = 0,
	SOUND_TYPE_2D,
	SOUND_TYPE_2D_LOOP,
	SOUND_TYPE_3D,
	SOUND_TYPE_3D_LOOP,
};

class FSystem{
public:
	FMOD::System *SystemPtr;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	FMOD_VECTOR listenerpos, vel, forward, up;
	int numdrivers, key;
	unsigned int version;
	bool isUp;
	FSystem();
	~FSystem();
	void Update();
	void clear();
	void init();
	void set(FMOD_VECTOR, FMOD_VECTOR,FMOD_VECTOR);
};
class FSound{
public:
	FSystem *SystemPtr;
	FMOD::Sound *SoundPtr;
	FMOD::Channel *ChannelPtr;
	FMOD_VECTOR listenerpos;
	FMOD_VECTOR soundPos;
	FMOD_VECTOR soundVel;
	FMOD_VECTOR vel, forward, up;
	SOUND_TYPE sound_type;
	unsigned int length;
	char *name;
	int key;
	bool             listenerflag, rollOff, pan;
	bool isSystem;
	FSound();
	FSound::FSound(char*, SOUND_TYPE);
	FSound(FSystem*, char*, SOUND_TYPE);
	FSound(FSystem*, char*, SOUND_TYPE,float minDist,float maxDist);
	void loadSound();
	void loadSound(float minDist, float maxDist);
	FMOD::Channel* play();
	~FSound();
	void clear();
	FSound* getSound();
	//void loadSound(FSystem);
	void update();
};
#endif
/*
_____SYSTEM
FMOD vel, pos, forward, up,
SSystem();
~SSystem();
FMOD result
FMOD:System *System
int numbdrivers
FMOD_SPEAKERMODE speakermode
FMOD_CAPS caps

//Sound Class
FMOD::Channel *channel play():
FMOD::CHannel *system;

*/