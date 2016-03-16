
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
#include "ConsoleMagic.h"

#include "include\fmod\fmod.hpp"
#include "include\fmod\fmod_errors.h"

#include "lib\glm\glm.hpp"

inline static FMOD_VECTOR GlmVecToFVec(glm::vec3 _pos){ FMOD_VECTOR fpos = { _pos.x, _pos.y, _pos.z }; return fpos; }


class FSystem{
public:
	ConsoleMagic* cm;
	FMOD::System* SystemPtr;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	FMOD_VECTOR sysPos, sysVel, sysFor, sysUp;
	int numberDrivers, key;
	unsigned int version;

	FSystem();
	~FSystem();
	void Update();
	void Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up, FMOD_VECTOR _vel);
	void Clear();
	inline void printPos(){ std::cout << "SysPos" << sysPos.x << " " << sysPos.y << " " << sysPos.z << std::endl; }

};
#endif