#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <sstream>
#include <stdlib.h>



#include "include\fmod\fmod.hpp"
#include "include\fmod\fmod_errors.h"

#include "lib\glm\glm.hpp"


static FMOD_RESULT result;
static char name[256];

static void ERRCHECK(FMOD_RESULT result);
static enum SOUND_TYPE getSoundType(int);
static enum SOUND_TYPE getSoundType(std::string& _type);
static enum ROLLOFF_TYPE getRollOff(int);
static enum ROLLOFF_TYPE getRollOff(std::string& _type);
static enum SOUND_ATT getSoundAtt(std::string& _type);
static enum OWNER getOwner(std::string& _type);

class FSound;
struct SNode{
	FSound* sound;
	std::string target;
	SNode* next;
};
enum OWNER{
	Background,
	Player,
	ETarget,
	NOWNERS,
};
enum SOUND_ATT{
	BACKGROUND,
	HIT,
	SMOVING,
	DEATH,
	PROJECTILE,
	NOFATT,
};
enum SOUND_TYPE{
	SOUND_TYPE_NULL,
	SOUND_TYPE_2D,
	SOUND_TYPE_2D_LOOP,
	SOUND_TYPE_3D,
	SOUND_TYPE_3D_LOOP,
};
enum ROLLOFF_TYPE{
	ROLLOFF_LINEAR,
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
	OWNER owner;
	SOUND_ATT att;
	const char* name;
	std::string sname;
	std::string target;

	unsigned int length;

	FSound(FSystem*, std::string, SOUND_TYPE);
	FSound(FSystem*, std::string, SOUND_TYPE,ROLLOFF_TYPE, float minDist, float maxDist);
	~FSound();
	void LoadSound();
	void LoadSound(float minDist, float maxDist);
	void Update();
	void Clear();
	FMOD::Channel* Play();
	void setSName(const std::string& _sname);
	void setTarget(const std::string& _sname);
	void setOwner(OWNER _owner);
	inline void setAtt(SOUND_ATT _att) { att = _att; }
};
class SoundManager{
public:
	FSystem* FSystemPtr;
	std::vector<FSound*> sounds;
	std::vector<std::vector<FSound*>> vSounds;
	int findName(std::string& _name);

	SoundManager(FSystem*, std::string& _filePath,std::string& _fileName);
	~SoundManager();
	void List();
	void Sort();
};


#endif