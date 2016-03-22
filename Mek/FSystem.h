
#pragma once
#ifndef FSYSTEM_H
#define FSYSTEM_H
#include <stdlib.h>
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
#include "include\GL\glew.h"
#include "include\GL\glfw3.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\gtx\vector_angle.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"
#include "lib\glm\gtx\rotate_vector.hpp"
#include "include\IL\ilut.h"
#include "ConsoleMagic.h"

#include "include\fmod\fmod.hpp"
#include "include\fmod\fmod_errors.h"

#include "lib\glm\glm.hpp"

inline static FMOD_VECTOR GlmVecToFVec(glm::vec3 _pos){ FMOD_VECTOR fpos = { _pos.x, _pos.y, _pos.z }; return fpos; }
class ReverbNode;
class FSystem{
public:
	ConsoleMagic* cm;
	FMOD::System* SystemPtr;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	FMOD_VECTOR sysPos, sysVel, sysFor, sysUp;
	std::vector<ReverbNode*> nodes;
	int numberDrivers, key;
	unsigned int version;
	FSystem();
	~FSystem();
	void Update();
	void Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up, FMOD_VECTOR _vel);
	void Clear();
	inline void printPos(){ std::cout << "SysPos" << sysPos.x << " " << sysPos.y << " " << sysPos.z << std::endl; }

	ReverbNode* CreateReverb(char* _name,FMOD_REVERB_PROPERTIES _props, float _x, float _y, float _z, float _min, float _max,bool _isDoor);

	void SetReverbNodesActive();
	void SetReverbNodesActiveFalse();

	void NodeDistToSys();
	void UpdateNodes();
};

class ReverbNode{
public:
	char* name;
	bool isDoor;
	FMOD::Reverb *freverb;
	FMOD_REVERB_PROPERTIES props;
	FMOD_VECTOR pos;
	FSystem* FSystemPtr;
	float min, max,distToSys;
	bool destruct;
	ReverbNode();
	void init(FSystem* _fsystem, FMOD_REVERB_PROPERTIES _props, float _x, float _y, float _z, float _min, float _max);
	~ReverbNode();
	void Update();
	void UpdateDist();
	inline void SetActive(){ freverb->setActive(true); }
	inline void SetActiveFalse(){ freverb->setActive(false); }
	void PrintDistToSys();
	void UpdateDoor();
};
#endif