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

class FSound;


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

	const char* name;
	std::string sname;
	std::string attribute;
	std::string owner;

	unsigned int length;

	FSound(FSystem*, std::string, SOUND_TYPE);
	FSound(FSystem*, std::string, SOUND_TYPE, ROLLOFF_TYPE, float minDist, float maxDist);
	~FSound();
	void LoadSound();
	void LoadSound(float minDist, float maxDist);
	void Update();
	void Clear();
	FMOD::Channel* Play();
	void setSName(const std::string& _sname);
	inline void setAttribute(const std::string& _attribute) { attribute = _attribute; };
	inline void setOwner(const std::string& _owner) { owner = _owner; };
};
class OwnerList{
public:
	OwnerList(std::string _name) { owner = _name; };
	~OwnerList();
	int findAttribute(char* _attribute);
	void findAndPlay(char* _attribute);
	std::string owner;

	std::vector<FSound*> list;
};
class SoundManager{
public:
	int sizeofSounds;
	FSystem* FSystemPtr;
	std::vector<FSound*> sounds;
	std::vector<std::vector<FSound*>> vSounds;

	std::vector<std::string> uOwners;
	std::vector<int> nuOwners;
	
	std::vector<OwnerList*> oList;

	int findName(std::string& _name);

	SoundManager(FSystem*, std::string& _filePath, std::string& _fileName);
	~SoundManager();
	void List();
	void Sort();
	void printOList();
	int findOwner(char* _owner);
	int findAttribute(char* _attribute);
	FSound* findSound(char* _owner, char* _attribute);
	void findAndPlay(char* _owner, char* _attribute);
	OwnerList* getOwnerList(char* _owner);
	//	for (int i = 0; i < oList.size(); i++)
	//		for (int j = 0; j < oList[i]->list.size(); j++)
	//			std::cout << oList[i]->owner << " " << oList[i]->list[j]->attribute << std::endl;
	//}
	//inline int getUOwner(char* _name){
	//	std::string name(_name);
	//	bool isFound = false;
	//	int index;
	//	for (int i = 0; i < vSounds.size(); i++){
	//		if (uOwners[i].compare(name) == 0){
	//			isFound = true;
	//			index = i;
	//		}
	//	}
	//	if (isFound){
	//		return index;
	//	}
	//	else{
	//		std::cout << "Couldnt find Owner";
	//		return -1;
	//	}
	//};
	//
	//inline std::vector<FSound*> getOwnerSounds(char* _owner){
	//	std::string name(_owner);
	//	bool isFound = false;
	//	int index;
	//	for (int i = 0; i < vSounds.size(); i++){
	//		if (uOwners[i].compare(name) == 0){
	//			isFound = true;
	//			index = i;
	//		}
	//	}
	//	if (isFound){
	//		return vSounds[index];
	//	}
	//	else{
	//		std::cout << "Couldnt find Owner Vector";
	//	}
	//}
	//
	//inline int getAttribute(char* _attribute){
	//	std::string attribute = _attribute;
	//	bool isFound = false;
	//	int index;
	//	for (int i = 0; i < oList.size(); i++)
	//		for (int j = 0; j < oList[i]->list.size(); j++)
	//	
	//			if (vSounds[i][j]->attribute == attribute){
	//				isFound = true;
	//				index = j;
	//			}
	//	if (isFound){
	//		return index;
	//	}
	//	else{
	//		std::cout << "Couldnt find Attribute";
	//		return -1;
	//	}
	//}
	//
	//inline FMOD::Channel* findAndPlay(char* _owner, char* _attribute){
	//
	//	int o = getUOwner(_owner);
	//	int a = getAttribute(_attribute);
	//
	//	std::cout << o << " " << a;
	//	return vSounds[o][a]->Play();
	//}
	//
	//inline FMOD::Channel* findAndPlayi(int _o,int _a){
	//	std::cout << _o << " " << _a;
	//	return vSounds[_o][_a]->Play();
	//}
	//
	//inline FSound* findSound(char* _owner, char* _attribute){
	//
	//	int o = getUOwner(_owner);
	//	int a = getAttribute(_attribute);
	//
	//	std::cout << o << " " << a;
	//	return vSounds[o][a];
	//}
};
#endif 