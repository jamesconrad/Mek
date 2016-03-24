#pragma once
#ifndef FSOUND_H
#define FSOUND_H
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


#include "FSystem.h"

#include "include\fmod\fmod.hpp"
#include "include\fmod\fmod_errors.h"

#include "lib\glm\glm.hpp"
//static FMOD_RESULT result;
//static char name[256];
enum SOUND_TYPE{
	SOUND_TYPE_NULL,
	SOUND_TYPE_2D,
	SOUND_TYPE_2D_LOOP,
	SOUND_TYPE_3D,
	SOUND_TYPE_3D_LOOP,
};
enum ROLLOFF_TYPE{
	ROLLOFF_LINEAR = 0,
	ROLLOFF_LINEARSQUARE,
	ROLLOFF_INVERSE,
};
class FSound{
public:

	FSystem* FSystemPtr;
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	FMOD_VECTOR soundPos, soundVel, soundPosD, activePos, activeVel;
	ReverbNode* node;
	ReverbNode* door;
	SOUND_TYPE soundType;
	ROLLOFF_TYPE rollOff;
	int typeOfActivePos;
	bool isPlaying;//gets checked everyupdate ..isSound playing?
	bool fastForward,playDoor,seeNode,sysOut;
	bool soundInsideNode,Gate;
	const char* fsname;	//full file path to the file
	std::string sname; //Short file name sound.wav
	std::string attribute; //attribute of sound or tag
	std::string owner; //Owner of the sound
	float min, max, distToSys, distToNode,doorVolume,nodeMin,nodeMax,activeMin,activeMax,activeVoulme,volume;
	unsigned int length;
	float spectrum[128];
	FSound();
	FSound(FSystem*, std::string, SOUND_TYPE);
	FSound(FSystem*, std::string, SOUND_TYPE, ROLLOFF_TYPE, float minDist, float maxDist);
	~FSound();
	void LoadSound();//default constructor for loading soud when minDst and maxDist not specified defualt: 0.5 100
	void LoadSound(float minDist, float maxDist); //Loading sound with specified min max distance
	void Update();//Checks if sound is playing updates bool, updates soundPos and soundVel within sound, calls FMOD::SystemUpdate
	void Clear();
	FMOD::Channel* Play();
	void SetSName(const std::string& _sname); //Sets name of the file, short name
	inline void SetAttribute(const std::string& _tag) { attribute = _tag; }; //sets attribute 
	inline void SetOwner(const std::string& _owner) { owner = _owner; };//sets owner
	bool IsPlaying(); //is sound playing?
	inline void Stop(){ ChannelPtr->stop(); } //Stops sound
	void UpdatePosition(glm::vec3 _pos){ if ((soundType != SOUND_TYPE_2D) || (soundType != SOUND_TYPE_2D_LOOP)) { FMOD_VECTOR pos = { _pos.x, _pos.y, _pos.z }; soundPos = pos; } }//Updates Pos
	inline void Pause(){ ChannelPtr->setPaused(true); }
	inline void UpdateSoundPos(glm::vec3 _pos){ FMOD_VECTOR fpos = { _pos.x, _pos.y, _pos.z }; soundPos = fpos; }
	void DistanceToFSystem();
	inline void printPos(){ std::cout << sname << soundPos.x << " " << soundPos.y << " " << soundPos.z << std::endl; }
	void printSound();
	void GetSpectrum();
	void UpdateDistToNode();
	void FindDoor();
	
	inline void PrintPos(){ std::cout << soundPos.x << " " << soundPos.y << " " << soundPos.z; };
	void USoundSets();
	inline void SetVolume(float _volume){ activeVoulme = _volume; }
	void UpdateTypeActivePos();
};


#endif