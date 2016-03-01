#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

#include "include\fmod\fmod.hpp"
#include "include\fmod\fmod_errors.h"

#include "lib\glm\glm.hpp"

static FMOD_RESULT result;
static char name[256];
static void ERRCHECK(FMOD_RESULT result);
enum SOUND_TYPE{
	SOUND_TYPE_NULL=0,
	SOUND_TYPE_2D,
	SOUND_TYPE_2D_LOOP,
	SOUND_TYPE_3D,
	SOUND_TYPE_3D_LOOP,
};
enum ROLLOFF_TYPE{
	ROLLOFF_LINEAR=0,
	ROLLOFF_LINEARSQUARE,
	ROLLOFF_INVERSE,
};
class FSystem{
public:
	FMOD::System* SystemPtr;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	FMOD_VECTOR sysPos, sysVel, sysFor, sysUp;
	int numberDrivers, key;
	unsigned int version;
	
	FSystem();
	~FSystem();
	void Update();
	void Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up);
	void Clear();
};
class FSound{
public:
	FSystem* FSystemPtr;
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	FMOD_VECTOR soundPos, soundVel;
	SOUND_TYPE soundType;
	ROLLOFF_TYPE rollOff;
	char* name;
	unsigned int length;

	FSound(FSystem*, char*, SOUND_TYPE);
	FSound(FSystem*, char*, SOUND_TYPE,ROLLOFF_TYPE, float minDist, float maxDist);
	~FSound();
	void LoadSound();
	void LoadSound(float minDist, float maxDist);
	void Update();
	void Clear();
	FMOD::Channel* Play();
};
class SoundManager{
public:
	SoundManager(std::string& _filePath);
	~SoundManager();

	std::vector<FSound*> sounds;
};
#endif