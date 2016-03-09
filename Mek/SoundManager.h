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
inline static FMOD_VECTOR GlmVecToFVec(glm::vec3 _pos){ FMOD_VECTOR fpos = { _pos.x, _pos.y, _pos.z }; return fpos; }
static void ERRCHECK(FMOD_RESULT result);
static enum SOUND_TYPE getSoundType(int);//returns SOUND_TYPE based in int
static enum SOUND_TYPE getSoundType(std::string& _type);//Convert string to enum of Sound Types 2d,3d 2d_loop, 3d_loop/ mainly when loading sounds from a txt file
static enum ROLLOFF_TYPE getRollOff(int);//returns ROLLOFF_Type based on int
static enum ROLLOFF_TYPE getRollOff(std::string& _type);//Converts string ot enum of rolloff type linear,linearsquare, inverse / mainly when loading sounds from a txt file

class FSound;

//enum of Sound_Types
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
	void Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up,FMOD_VECTOR _vel);
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
	bool isPlaying;//gets checked everyupdate ..isSound playing?
	const char* name;	//full file path to the file
	std::string sname; //Short file name sound.wav
	std::string attribute; //attribute of sound or tag
	std::string owner; //Owner of the sound

	unsigned int length;

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
};
//OwnerList is a data structure that stores all sounds that belong to a particular owner
//Easy to use whinin game 
class OwnerList{
public:
	std::string owner;
	std::vector<FSound*> list;// vector of FSounds

	OwnerList(std::string _name) { owner = _name; };
	~OwnerList();
	int FindAttribute(char* _tag);//Finds a sound with a particular tag in the list, returns int
	void FindAndPlay(char* _tag); //Find a sound with a particular tag and play it, returns nothing
	void PlayAndPauseAll(); //Plays and Pauses all sounds// cannot play a sound within update, so have to have to do this magic and unpause in update
	void PlayAndPause(char* _tag); //PLays and pauses a specific sound
	void FindAndUnpause(char* _tag);// finds and unpauses a specific sound
	bool IsPlaying(char* _tag);
	FSound* FindSound(char* _tag);//Finds sound and returns FSound*
	void Stop(char* _tag);
	inline void StopAll(){ for (int c = 0; c < list.size(); c++) list[c]->Stop(); }
	void UpdateOwnerListPosition(glm::vec3 _pos){ for (int c = 0; c < list.size(); c++) list[c]->UpdatePosition(_pos); }
	void FindAndPause(char* _tag);
	void UpdateOwnerSoundPos(glm::vec3 _pos);

};
class SoundManager{
public:
	int sizeofSounds;
	FSystem* FSystemPtr; //Pointer to the FMOD::SYSTEM
	std::vector<FSound*> sounds; //Initial init loads all sounds into a vector;
	std::vector<std::vector<FSound*>> vSounds;// 2D vector of sounds Columm = different owners , row = all sounds of an owner
	std::vector<std::string> uOwners; //vector of unique owners
	std::vector<int> nuOwners; // vector of unique sounds for a specific owner
	std::vector<OwnerList*> oList; // vector of OwnerList or vector of owners and all their sounds

	SoundManager(FSystem*, std::string& _filePath, std::string& _fileName);
	~SoundManager();
	void List();// Lists all sounds in vSound
	void Sort();//Sorts all sounds and puts them into vSounds and oLists
	void printOList(); //prints oList and all the sounds init 
	int FindOwner(char* _owner);//Finds owner in oList and returns the index in the oList
	int FindAttribute(char* _tag);//finds tag in a list of olIsts and returns index of list within oList
	FSound* FindSound(char* _owner, char* _tag);//Finds sound and returns FSound*
	void FindAndPlay(char* _owner, char* _tag);//Finds and plays sound->goes through OwnerList functions
	OwnerList* GetOwnerList(char* _owner);// Finds all sounds of a particular owner and returns the list
	void PlayAndPause(char* _owner, char* _tag); //Plays and Pauses a sound// used withing update functions
	void PlayAndPauseAll();//Plays and Pauses all sounds// used withing update functions
	void FindAndUnpause(char* _owner, char* _tag);//Finds and pauses a specific sounds ->goes through Ownerlist functions
	void Update();//Goes through all sounds loaded in the system and updates them
	bool IsPlaying(char* _owner, char* _tag);//returns if a specific sound is playing//goes through OwnerList functions
	void Stop(char* _owner, char* _tag);
	void inline StopAll(){ for (int c = 0; c < oList.size(); c++)oList[c]->StopAll(); }
	void FindAndPause(char* _owner, char* _tag);
	void UpdateOwnerSoundPos(char* _owner, glm::vec3 _pos);
	void SoundPlaying();
	inline void UpdateSysO(glm::vec3 _pos, glm::vec3 _for, glm::vec3 _up, glm::vec3 _vel){ FSystemPtr->Set(GlmVecToFVec(_pos), GlmVecToFVec(_for), GlmVecToFVec(_up), GlmVecToFVec(_vel)); }
};
#endif 