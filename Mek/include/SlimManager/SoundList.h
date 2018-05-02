#ifndef SOUNDLIST_H
#define SOUNDLIST_H
#include "Sound.h"
class SoundList{
public:
	//init functions
	SoundList(){}
	void Init(std::string _owner,std::vector<Sound*> _list);
	//control functions
	void StopList();
	void PauseList(bool _pause);
	void MuteList(bool _mute);
	void PauseSound(char* _tag,bool _pause);
	void MuteSound(char* _tag, bool _mute);
	void FindAndPlay(char* _tag);
	void PlayAndPause(char* _tag, bool _pause);
	void PlayAll();
	void PauseAll(bool _pause);
	//get functions
	Sound* FindSound(char* _tag);
	inline std::string GetOwnerName(){ return m_ownerName; }
	//set functions
	void UpdateListPosition(glm::vec3 _pos);
	void FastForwardList(bool _fastForward);
	//update functions
	
	//display functions
	void PrintList();
	void PrintDistance();

	
private:
	std::string m_ownerName;
	std::vector<Sound*> m_list;
};
#endif