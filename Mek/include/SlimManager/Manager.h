#ifndef MANAGER_H
#define MANAGER_H
#include <ctime>
#include "SoundManager.h"
#include "PresetManager.h"
#include "RNodeManager.h"

class Manager{
public:
	//init functions
	Manager(){}
	void Init();

	//get functions
	inline SoundSystem* GetSoundSystem(){ return &m_SoundSystemPtr; }
	inline SoundManager* GetSoundManager(){ return &m_SoundManagerPtr; }
	inline RNodeManager* GetRNodeManager(){ return &m_RNManagerPtr; }
	inline PresetManager* GetPresetManager(){ return &m_PresetManagerPtr; }
	//Display Functions
	void DisplaySystemNode();
	void DisplayAll();
	//update functions
	inline void SetSystemPosition(glm::vec3 _pos){ m_SoundSystemPtr.SetPosition(_pos); }
	inline void SetSystemForward(glm::vec3 _pos){ m_SoundSystemPtr.SetForward(_pos); }
	inline void SetSystemUp(glm::vec3 _pos){ m_SoundSystemPtr.SetUp(_pos); }
	void SetSystemPFU(glm::vec3 _pos, glm::vec3 _for, glm::vec3 _up);
	void UpdateSounds();
	void UpdateRNodes();
	void UpdateSystemRNode();
	void FindDoor();
	void UpdateSoundRNode();
	void Update();
private:
	SoundSystem m_SoundSystemPtr;
	SoundManager m_SoundManagerPtr;
	PresetManager m_PresetManagerPtr;
	RNodeManager m_RNManagerPtr;
	RNode* m_systemRNode;
};
#endif