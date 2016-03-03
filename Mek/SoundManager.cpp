#include "SoundManager.h"
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (%d) %s\n" << " " << result << " " << FMOD_ErrorString(result);
		//	exit(-1);
	}
}
static enum SOUND_TYPE getSoundType(int _type){
	switch (_type){
	case 0:
		return SOUND_TYPE_NULL;
		break;
	case 1:
		return SOUND_TYPE_2D;
		break;
	case 2:
		return SOUND_TYPE_2D_LOOP;
		break;
	case 3:
		return SOUND_TYPE_3D;
		break;
	case 4:
		return SOUND_TYPE_3D_LOOP;
		break;
	}
}
static enum SOUND_TYPE getSoundType(std::string& _type){
	if (_type == "SOUND_TYPE_2D")
		return SOUND_TYPE_2D;
	else if (_type == "SOUND_TYPE_2D_LOOP")
		return SOUND_TYPE_2D_LOOP;
	else if (_type == "SOUND_TYPE_3D")
		return SOUND_TYPE_3D;
	else if (_type == "SOUND_TYPE_3D_LOOP")
		return SOUND_TYPE_3D_LOOP;
}

static enum ROLLOFF_TYPE getRollOff(int type){
	switch (type){
	case 0:
		return ROLLOFF_LINEAR;
		break;
	case 1:
		return ROLLOFF_LINEARSQUARE;
		break;
	case 2:
		return ROLLOFF_INVERSE;
		break;
	}
}
static enum ROLLOFF_TYPE getRollOff(std::string& _type){
	if (_type == "ROLLOFF_LINEAR")
		return ROLLOFF_LINEAR;
	else if (_type == "ROLLOFF_LINEARSSQUARE")
		return ROLLOFF_LINEARSQUARE;
	else if (_type == "ROLLOFF_INVERSE")
		return ROLLOFF_INVERSE;
}
static enum SOUND_ATT getSoundAtt(std::string& _type){
	if (_type == "BACKGROUND")
		return BACKGROUND;
	else if (_type == "HIT")
		return HIT;
	else if (_type == "SMOVING")
		return SMOVING;
	else if (_type == "DEATH")
		return DEATH;
	else if (_type == "PROJECTILE")
		return PROJECTILE;
}
static enum OWNER getOwner(std::string& _type){
	if (_type == "Background")
		return Background;
	else if (_type == "Player")
		return Player;
	else if (_type == "ETarget")
		return ETarget;
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
//VARIABLES: FSystem && ConstChar* Name && SOUND_TYPE
FSound::FSound(FSystem* _fsystem, std::string _name, SOUND_TYPE _soundType){
	FSystemPtr = _fsystem;
	name = _name.c_str();
	soundType = _soundType;
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };

	LoadSound();
}
FSound::FSound(FSystem* _fsystem, std::string _name, SOUND_TYPE _soundType, ROLLOFF_TYPE _rolloff, float _minDist, float _maxDist){
	FSystemPtr = _fsystem;
	name = _name.c_str();
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
void FSound::setSName(const std::string& _name){
	sname = _name;
}
void FSound::setTarget( const std::string& _name){
	target = _name;
}
void FSound::setOwner(OWNER _owner){
	owner = _owner;
}
//_________________________SOUND MANAGER________________________________________________________________________

SoundManager::SoundManager(FSystem* _FSystemPtr,std::string& _filePath, std::string& _fileName){
	FSystemPtr = _FSystemPtr;
	std::ifstream file;
	std::string name = _filePath + _fileName;
	file.open(((name).c_str()));
	
	std::string output;
	std::string line;
	
	if (file.is_open())
	{
		//getline(file, line);
		while (file.good())
		{
			//system("CLS");
			getline(file, line);

			std::string str(line);
			std::string buf; // Have a buffer string
			std::stringstream ss(str); // Insert the string into a stream 

			std::vector<std::string> tokens; // Create vector to hold our words

			while (ss >> buf)
				tokens.push_back(buf);

			SOUND_TYPE STYPE;
			ROLLOFF_TYPE RTYPE;
			SOUND_ATT ATYPE;
			

			std::string pname = _filePath + tokens[0];

			if (tokens.size() == 4){
				ATYPE = getSoundAtt(tokens[1]);
				STYPE = getSoundType(tokens[2]);
				FSound* temp = new FSound(FSystemPtr,pname,STYPE);
				temp->setSName(tokens[0]);
				temp->setTarget(tokens[3]);
				temp->setOwner(getOwner(tokens[3]));
				temp->setAtt(ATYPE);
				sounds.push_back(temp);

			} 

			if (tokens.size() == 7){
				ATYPE = getSoundAtt(tokens[1]);
				STYPE = getSoundType(tokens[2]);
				RTYPE = getRollOff(tokens[3]);
				float minDist = std::stod(tokens[4], NULL);
				float maxDist = std::stod(tokens[5], NULL);

				FSound* temp = new FSound(FSystemPtr,pname,STYPE,RTYPE,minDist,maxDist);
				temp->setSName(tokens[0]);
				temp->setAtt(ATYPE);
				temp->setTarget(tokens[6]);
				temp->setOwner(getOwner(tokens[6]));
				sounds.push_back(temp);
			}
			//
		}
	}
	else
	{
		std::cout << "Unable to load Sounds: " << _filePath << std::endl;
	}

	Sort();
}
SoundManager::~SoundManager(){
	for (int i = 0; i < sounds.size(); i++)
		sounds.pop_back();
	FSystemPtr = NULL;
	delete FSystemPtr;
}
int SoundManager::findName(std::string& _name){
	//for (int i = 0; i < sounds.size(); i++){
	//	if (sounds[i]->->target == _name)
	//		return i;
	//	else
	//		return 0;
	//}
	return 0;
}

void SoundManager::List(){
	for (int i = 0; i < NOWNERS; i++){
		for (int j = 0; j < vSounds[i].size(); j++){
			if (vSounds[i][j] == NULL)
				continue;
			//vSounds[i][j]->Play();
			std::cout << i << " " << j << vSounds[i][j]->target << " " << vSounds[i][j]->sname << " " << vSounds[i][j]->att << std::endl;
		}
	}
}
void SoundManager::Sort(){
	int nTargets[NOWNERS] = { 0 };
	for (int i = 0; i < sounds.size(); i++){
		if (sounds[i]->target == "Background")
			nTargets[Background] + 1;
		else if (sounds[i]->target == "Player")
			nTargets[Player] + 1;
		else if (sounds[i]->target == "Target")
			nTargets[ETarget] + 1;
	}

	vSounds.resize(NOWNERS);
	for (int i = 0; i < NOWNERS; i++){
		vSounds[i].resize(NOFATT);
	}


	FSound* temp;
	for (int i = 0; i < sounds.size(); i++){
		temp = sounds[i];
		if (temp->target == "Background"){
			vSounds[Background].push_back(temp);
		}
		else if (temp->target == "Target"){
			if (temp->att == HIT)
				vSounds[ETarget][HIT] = temp;
			else if (temp->att ==SMOVING)
				vSounds[ETarget][SMOVING] = temp;
			else if (temp->att == DEATH)
				vSounds[ETarget][DEATH] = temp;

		}
		else if (temp->target == "Player"){
			if (temp->att == HIT)
				vSounds[Player][HIT] = temp;
			else if (temp->att == SMOVING)
				vSounds[Player][SMOVING] = temp;
			else if (temp->att == DEATH)
				vSounds[Player][DEATH] = temp;
			else if (temp->att == PROJECTILE)
				vSounds[Player][PROJECTILE] = temp;
		}
	}
}

//__________________LinkedSNode________________________________________________________________________
