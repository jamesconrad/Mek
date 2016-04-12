#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"
#include "../../lib/glm\glm.hpp"
#include "../../lib/glm/gtx/transform.hpp"
	static void ERRCHECK(FMOD_RESULT m_result)
	{
		if (m_result != FMOD_OK)
		{
			std::cout << "FMOD error! (%d) %s\n" << " " << m_result << " " << FMOD_ErrorString(m_result);
			//	exit(-1);
		}
	}
	static FMOD_VECTOR Glm3ToFVEC(glm::vec3 _vec){
		FMOD_VECTOR vec{ _vec.x, _vec.y, _vec.z };
		return vec;
	}
	static glm::vec3 FVECToGLM3(FMOD_VECTOR _vec){
		glm::vec3 vec{ _vec.x, _vec.y, _vec.z };
		return vec;
	}
	static void PrintFMODVector(FMOD_VECTOR _vec){
		std::cout << "x: "<< _vec.x << " y: " << _vec.y << " z: " << _vec.z << std::endl << std::flush << std::flush;
	}
	static float DistanceBetween2FVECTORS(FMOD_VECTOR _vec1,FMOD_VECTOR _vec2){
		FMOD_VECTOR sp = _vec1;
		FMOD_VECTOR ssp = _vec2;
		float x = sp.x - ssp.x;
		float y = sp.y - ssp.y;
		float z = sp.z - ssp.z;
		float distance = sqrt(abs(pow(x, 2) + pow(x, 2) + pow(x, 2)));
		return distance;
	}
class RNodeManager;
class Sound;
class SoundSystem{
public:
	//init fuctions
	SoundSystem(){}
	void Init();
	~SoundSystem();
	void InitManagers();
	///update functions
	void Update();
	//set functions
	inline void SetPosition(glm::vec3 _pos){ m_systemPos = Glm3ToFVEC(_pos); }
	inline void SetForward(glm::vec3 _for){ m_systemForward = FMOD_VECTOR{ -_for.x, -_for.y, -_for.z}; }
	inline void SetUp(glm::vec3 _up){ m_systemUp = Glm3ToFVEC(_up); }

	void AddSoundToList(Sound* _sound);
	
	//Get Functions
	inline FMOD::System* GetSystemPtr(){ return m_SystemPtr; }
	inline SoundSystem* GetSoundSystem() { return this; }
	inline FMOD_VECTOR GetSystemPosition(){ return m_systemPos; }
	inline FMOD_VECTOR GetSystemFor(){ return m_systemForward; }
	//inline RNodeManager* GetRNodeManager(){ return m_RNodeManagerPtr; }

	//display functions
	inline void PrintSounds();
	void GetChannelsPlaying();
	
private:
	FMOD::System* m_SystemPtr;
	FMOD_SPEAKERMODE m_speakermode;
	FMOD_CAPS m_caps;
	int m_numberDrivers;
	unsigned int m_version;
	FMOD_RESULT m_result;
	char* m_name;

	FMOD_VECTOR m_systemVel;
	FMOD_VECTOR m_systemPos;
	FMOD_VECTOR m_systemForward;
	FMOD_VECTOR m_systemUp;	
};
#endif