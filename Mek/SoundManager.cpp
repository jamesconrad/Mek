#include "SoundManager.h"
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" << " " << result << " " << FMOD_ErrorString(result);
		//	exit(-1);
	}
}
FSystem::FSystem(){
	sysVel = { 0.0, 0.0, 0.0 };
	sysFor = { 0.0, 0.0, 0.0 };
	sysUp = { 0.0, 0.0, 0.0 };
	sysPos = { 0.0, 0.0, 0.0 };

	result = FMOD::System_Create(&SystemPtr);
	if (result != FMOD_OK){
		std::cout << "Failed to create system! Error: "; ERRCHECK(result); std::cout << std::endl;
	}

	result = SystemPtr->getNumDrivers(&numberDrivers);
	if (result != FMOD_OK){
		std::cout << "Failed to get number of drivers! Error: "; ERRCHECK(result); std::cout << std::endl;
	}

	result = SystemPtr->getVersion(&version);
	if (result != FMOD_OK){
		std::cout << "Failed to get system version! Error: "; ERRCHECK(result); std::cout << std::endl;
	}

	if (numberDrivers == 0){
		result = SystemPtr->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if (result != FMOD_OK){
			std::cout << "Failed to set system output to no sound! Error: "; ERRCHECK(result); std::cout << std::endl;
		}
	}
	else{
		//retrieves current system's speakermode;
		result = SystemPtr->getDriverCaps(0, &caps, 0, &speakermode);
		if (result != FMOD_OK){
			std::cout << "Failed to get speakermode! Error: "; ERRCHECK(result); std::cout << std::endl;
		}
		//sets speakermode to current system speakermode
		result = SystemPtr->setSpeakerMode(speakermode);
		if (result != FMOD_OK){
			std::cout << "Failed to set speakermode! Error: "; ERRCHECK(result); std::cout << std::endl;
		}

		if (caps & FMOD_CAPS_HARDWARE_EMULATED){
			result = SystemPtr->setDSPBufferSize(1024, 10);
			if (result != FMOD_OK){
				std::cout << "Failed to set DSP Buffer Size! Error: "; ERRCHECK(result); std::cout << std::endl;
			}
		}
		
		result = SystemPtr->getDriverInfo(0, name, 256, 0);
		if (result != FMOD_OK){
			std::cout << "Failed to get Driver Info! Error: "; ERRCHECK(result); std::cout << std::endl;
		}
	}

	result = SystemPtr->init(1000, FMOD_INIT_NORMAL, 0);
	//if speakermode is not supported by the system switch it to stereo
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER){
		result = SystemPtr->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if (result != FMOD_OK){
			std::cout << "Failed to set Speakermode to Stereo! Error: "; ERRCHECK(result); std::cout << std::endl;
		}
		
		result = SystemPtr->init(100, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK){
			std::cout << "Failed to Initialize System Stereo! Error: "; ERRCHECK(result); std::cout << std::endl;
		}
	}

	result = SystemPtr->set3DSettings(1.0, 1.0, 1.0);
	if (result != FMOD_OK){
		std::cout << "Failed to set 3D Settings! Error: "; ERRCHECK(result); std::cout << std::endl;
	}
}
FSystem::~FSystem(){
	result = SystemPtr->close();
	if (result != FMOD_OK){
		std::cout << "Failed to Close System: "; ERRCHECK(result); std::cout << std::endl;
	}
	Clear();
}
void FSystem::Update(){
	result = SystemPtr->set3DListenerAttributes(0, &sysPos, &sysVel, &sysFor, &sysUp); //up=
	if (result != FMOD_OK){
		std::cout << "Failed to set 3D Listerner Attributes in System: "; ERRCHECK(result); std::cout << std::endl;
	}
	SystemPtr->update();
}
void FSystem::Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up){
	sysPos = _pos;
	sysFor = _for;
	sysUp = _up;
}
void FSystem::Clear(){
	SystemPtr = NULL;
	delete SystemPtr;
}

//FSOUND________________________________________________________________________________________
FSound::FSound(FSystem* _fsystem, char* _name, SOUND_TYPE _soundType){
	FSystemPtr = _fsystem;
	name = _name;
	soundType = _soundType;
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };

	LoadSound();
}
FSound::FSound(FSystem* _fsystem, char* _name, SOUND_TYPE _soundType,ROLLOFF_TYPE _rolloff, float _minDist, float _maxDist){
	FSystemPtr = _fsystem;
	name = _name;
	soundType = _soundType;
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	rollOff = _rolloff;
	LoadSound(_minDist, _maxDist);
}
FSound::~FSound(){
	result = SoundPtr->release();
	if (result != FMOD_OK){
		std::cout << "Failed to Release Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
	}
	Clear();
}
void FSound::LoadSound(){
	switch (soundType){
	case SOUND_TYPE_2D:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_OFF);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_2D_LOOP:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_NORMAL);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_3D:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(0.5, 100);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}
		switch (rollOff){
		case ROLLOFF_LINEAR:
			result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_LINEARSQUARE:
			result = SoundPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_INVERSE:
			result = SoundPtr->setMode(FMOD_3D_INVERSEROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		}		
		break;
	case SOUND_TYPE_3D_LOOP:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(0.5, 100);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		switch (rollOff){
		case ROLLOFF_LINEAR:
			result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_LINEARSQUARE:
			result = SoundPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_INVERSE:
			result = SoundPtr->setMode(FMOD_3D_INVERSEROLLOFF | FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		}
		break;

	}
	result = SoundPtr->getLength(&length, FMOD_TIMEUNIT_MS);
	if (result != FMOD_OK){
		std::cout << "Failed to get Sound Length in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
	}

}
void FSound::LoadSound(float _minDist, float _maxDist){
	switch (soundType){
	case SOUND_TYPE_2D:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_OFF);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_2D_LOOP:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_NORMAL);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_3D:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(_minDist, _maxDist);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		switch (rollOff){
		case ROLLOFF_LINEAR:
			result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_LINEARSQUARE:
			result = SoundPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_INVERSE:
			result = SoundPtr->setMode(FMOD_3D_INVERSEROLLOFF | FMOD_LOOP_OFF);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		}
		break;
	case SOUND_TYPE_3D_LOOP:
		result = FSystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(_minDist, _maxDist);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		switch (rollOff){
		case ROLLOFF_LINEAR:
			result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_LINEARSQUARE:
			result = SoundPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_INVERSE:
			result = SoundPtr->setMode(FMOD_3D_INVERSEROLLOFF | FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		}
		break;

	}
	result = SoundPtr->getLength(&length, FMOD_TIMEUNIT_MS);
	if (result != FMOD_OK){
		std::cout << "Failed to get Sound Length in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
	}

}
void FSound::Update(){
	result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
	if (result != FMOD_OK){
		std::cout << "Failed to set Channel 3D Attributes in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
	}

	result = FSystemPtr->SystemPtr->set3DListenerAttributes(0, &FSystemPtr->sysPos,&FSystemPtr->sysVel, &FSystemPtr->sysFor, &FSystemPtr->sysUp);
	if (result != FMOD_OK){
		std::cout << "Failed to set 3D Listener Attributes: "; ERRCHECK(result); std::cout << std::endl;
	}

	FSystemPtr->Update();
}
void FSound::Clear(){
	FSystemPtr = NULL;
	delete FSystemPtr;

	SoundPtr = NULL;
	delete SoundPtr;

	ChannelPtr = NULL;
	delete ChannelPtr;

	name = NULL;
	delete name;
}
FMOD::Channel* FSound::Play(){
	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, true, &ChannelPtr);
	if (result != FMOD_OK){
		std::cout << "Failed to Play Sound: " << name << " "; ERRCHECK(result); std::cout << std::endl;
	}

	switch (soundType){
	case SOUND_TYPE_3D:
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		if (result != FMOD_OK){
			std::cout << "Failed to set Channel 3D Attributes in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_OFF);
		if (result != FMOD_OK){
			std::cout << "Failed to set Mode: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setPaused(false);
		if (result != FMOD_OK){
			std::cout << "Failed to Unpause: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_3D_LOOP:
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		if (result != FMOD_OK){
			std::cout << "Failed to set Channel 3D Attributes in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_NORMAL);
		if (result != FMOD_OK){
			std::cout << "Failed to set Mode: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setPaused(false);
		if (result != FMOD_OK){
			std::cout << "Failed to Unpause: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		break;
	default:
		result = ChannelPtr->setPaused(false);
		ERRCHECK(result);
	}
	
	return ChannelPtr;
}