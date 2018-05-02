
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <fstream>
#include <ostream>
#include <sstream>
#include "SoundList.h"
using namespace std;
class SoundManager{
public:
	SoundManager(){}
	void Init(SoundSystem* _system, std::vector<RNode*> _rnodes, std::string _filePath, std::string _fileName);
	~SoundManager();
	//control functions
	void StopAll();
	void StopOwner(char* _owner, bool _pause);
	void PauseOwner(char* _owner,bool _pause);
	void MuteOwner(char* _owner,bool _mute);
	void PauseSound(char* _owner, char* _tag, bool _mute);
	void MuteSound(char* _owner, char* _tag, bool _mute);
	void FindAndPlay(char* _owner,char* _tag);
	void PlayAndPause(char* _owner, char* _tag, bool _mute);
	void PlayAll();
	void PauseAll(bool _pause);
	//get functions
	Sound* FindSound(char* _owner,char* _tag);
	SoundList* GetOwnerList(char* _owner);
	bool IsPlaying(char* _owner, char* _tag);
	//set functions
	void SetOwnerPosition(char* _owner, glm::vec3 _pos);
	void Sort();
	void UpdateSoundRNode();
	void UpdateSystemRNode(RNode* _node);
	void FastForwardAll(bool _fastForward);
	//update functions
	void Update();
	//display functions
	void PrintList();
	void PrintDistance();
private:
	SoundSystem* m_SoundSystemPtr;
	std::vector<RNode*> m_rnodes;
	std::vector<std::string> m_uOwners;
	std::vector<Sound*> m_soundList;
	std::vector<SoundList*> m_oList;
};
#endif
