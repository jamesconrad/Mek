#pragma once
#ifndef FSOUNDMANAGER_H
#define FOSUNDMANAGER_H

#include "FSound.h"
#include "OwnerList.h"
#include "ConsoleMagic.h"

static enum SOUND_TYPE getSoundType(int);//returns SOUND_TYPE based in int
static enum SOUND_TYPE getSoundType(std::string& _type);//Convert string to enum of Sound Types 2d,3d 2d_loop, 3d_loop/ mainly when loading sounds from a txt file
static enum ROLLOFF_TYPE getRollOff(int);//returns ROLLOFF_Type based on int
static enum ROLLOFF_TYPE getRollOff(std::string& _type);//Converts string ot enum of rolloff type linear,linearsquare, inverse / mainly when loading sounds from a txt file


class FSoundManager{
public:
	int sizeofSounds;
	FSystem* FSystemPtr; //Pointer to the FMOD::SYSTEM
	std::vector<FSound*> sounds; //Initial init loads all sounds into a vector;
	std::vector<std::vector<FSound*>> vSounds;// 2D vector of sounds Columm = different owners , row = all sounds of an owner
	std::vector<std::string> uOwners; //vector of unique owners
	std::vector<int> nuOwners; // vector of unique sounds for a specific owner
	std::vector<OwnerList*> oList; // vector of OwnerList or vector of owners and all their sounds

	FSoundManager(FSystem*, std::string& _filePath, std::string& _fileName);
	~FSoundManager();
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

	//DSP affect
	void Amplify(char* _file, float _amp);
	void Echo(char* _file, int _delay, int _echos);
	void Convolution(char* _file, int _filterWidth, int _amp);
	void Robot(FSound* s,char* _file, int _pitch, int _distortion, int _amp);
	void Fishman(FSound* s, char* _file, float _volume, float _amount);
	void BadConnection(char* _file, float _levels, float _radioStatic);

	bool LoadRawData(const char * file_name, FMOD::System* system, std::vector<signed short>* rawData, int* channels);
	bool CreateSoundFromData(FMOD::System* system, FMOD::Sound **sound, int channels, unsigned int frequency, bool loop, std::vector<signed short>* rawData);
	void FastForwardAll();
	void ResetFastForwardAll();
};

#endif