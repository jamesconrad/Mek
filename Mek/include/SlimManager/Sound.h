#ifndef SOUND_H
#define SOUND_H
#include"SoundSystem.h"
#include "RNode.h"
class Sound{
public:
	//init functions
	Sound();
	~Sound();
	bool Create(SoundSystem* _system,const char* _fileName, bool _is3d, bool _loop);

	//control functions
	FMOD::Channel* Play();
	inline void PauseSound(bool _pause){ m_ChannelPtr->setPaused(_pause); }
	inline void StopSound() { m_ChannelPtr->stop(); }
	inline void MuteSound(bool _mute) { m_ChannelPtr->setMute(_mute); }
	//info functions
	void PrintSoundInformation();
	void PrintDistanceToSystem();
	//set functions
	inline void SetSoundName(std::string _name){ m_soundName = _name; }
	inline void SetSoundOwner(std::string _name){ m_soundOwner = _name; }
	inline void SetSoundTag(std::string _name){ m_soundTag = _name; }
	inline void SetMinDistance(float _min){ m_activeMaxDistance = _min; }
	inline void SetMaxDistance(float _max){ m_activeMaxDistance = _max; }
	inline void SetMinMaxDistance(float _min, float _max) { SetMinDistance(_min); SetMaxDistance(_max); }
	inline void SetVolume(float _v){ m_activeVolume = _v; }
	inline void SetSoundRNode(RNode* _node){ m_rnode = _node; }
	inline void SetSystemRNode(RNode* _node){ m_systemRNode = _node;}
	inline void SetOminmax(float _omin, float _omax){ m_minDistance = _omin; m_maxDistance = m_maxDistance = _omax; }
	void SetSoundPosition(glm::vec3 _pos);
	void FastForward(bool _fastForward);
	//GetFunctions
	inline Sound* GetSound(){ return this; }
	inline const char* GetFullName(){ return m_fullPathName; }
	inline std::string GetSoundName(){ return m_soundName; }
	inline std::string GetSoundOwner(){ return  m_soundOwner; }
	inline std::string GetSoundTag(){ return  m_soundTag; }
	inline FMOD_VECTOR& GetSoundPostion(){ return m_soundPos; }
	inline float GetDistanceToSystem() { return m_distanceToSystem; }
	inline FMOD_MODE GetSoundMode(){ return m_mode; }
	inline float GetMinDistance(){ return m_activeMinDistance; }
	inline float GetMaxDistance(){ return m_activeMaxDistance; }
	inline FMOD::Channel* GetSoundChannelPtr(){ return m_ChannelPtr; }
	bool GetIsPlaying(){ return m_isPlaying; }
	RNode* FindCloserNode();
	//update functions
	void UpdateRNode(std::vector<RNode*> _rnodes);
	void UpdateSoundSettings();
	void UpdateDistanceToSystem();
	void UpdateRNode();
	void Update();
	
private:
	SoundSystem* m_SoundSystemPtr;
	std::vector<RNode*> m_rnodes;
	RNode* m_rnode;
	RNode* m_systemRNode;
	FMOD::Sound* m_SoundPtr;
	FMOD::Channel* m_ChannelPtr;

	FMOD_MODE m_mode;

	FMOD_RESULT m_result;

	FMOD_VECTOR m_soundPos;
	FMOD_VECTOR m_soundVel;
	FMOD_VECTOR m_activePos;
	FMOD_VECTOR m_activeVel;

	std::string m_soundName;
	std::string m_soundTag;
	std::string m_soundOwner;

	const char* m_fullPathName;

	float m_minDistance;
	float m_maxDistance;
	float m_activeMinDistance;
	float m_activeMaxDistance;
	float m_activeVolume;
	float m_spectrum[128];
	float m_distanceToSystem;

	bool m_isPlaying;
	bool m_insideNode;
	bool m_Node;
	bool m_isFastforward;
};
#endif