#pragma once
#ifndef OWNERLIST_H
#define OWNERLIST_H
#include <string>
#include <vector>
#include "FSound.h"
//OwnerList is a data structure that stores all sounds that belong to a particular owner
//Easy to use whinin game 
class OwnerList{
public:
	std::string owner;
	std::vector<FSound*> list;// vector of FSounds
	OwnerList(){}
	OwnerList(std::string _name) { owner = _name; };
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
	void UpdateOwnerSoundPos(const glm::vec3*);
	void GetListSpectrum(char* _tag);
	void FastForwardAll();
	void ResetFastForwardAll();
	void SoundVolumeAll(float _volume);
};

#endif