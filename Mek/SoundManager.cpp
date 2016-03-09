#include "SoundManager.h"
//checks result of FMOD function
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

	result = SystemPtr->set3DSettings(1, 1, 1);
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
void FSystem::Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up,FMOD_VECTOR _vel){
	sysPos = _pos;
	sysFor = _for;
	sysUp = _up;
	sysVel = _vel;
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
	isPlaying = false;
	LoadSound();
}
FSound::FSound(FSystem* _fsystem, std::string _name, SOUND_TYPE _soundType, ROLLOFF_TYPE _rolloff, float _minDist, float _maxDist){
	FSystemPtr = _fsystem;
	name = _name.c_str();
	soundType = _soundType;
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	rollOff = _rolloff;
	isPlaying = false;
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
	//Picks appropriate SOUND_TYPE and creates a sound with that type
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
	//stores length of the sound
	result = SoundPtr->getLength(&length, FMOD_TIMEUNIT_MS);
	if (result != FMOD_OK){
		std::cout << "Failed to get Sound Length in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
	}

}
void FSound::Update(){
	ChannelPtr->isPlaying(&isPlaying);
	if ((isPlaying && soundType == SOUND_TYPE_3D) || (isPlaying && soundType == SOUND_TYPE_3D_LOOP)){
		std::cout << "Updating: " << this->sname << std::endl;
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		if (result != FMOD_OK){
			std::cout << "cFS->U Failed to set Channel 3D Attributes in: " << name << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = FSystemPtr->SystemPtr->set3DListenerAttributes(0, &FSystemPtr->sysPos, &FSystemPtr->sysVel, &FSystemPtr->sysFor, &FSystemPtr->sysUp);
		if (result != FMOD_OK){
			std::cout << "cFS->UFailed to set 3D Listener Attributes: "; ERRCHECK(result); std::cout << std::endl;
		}
		FSystemPtr->Update();
	}
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
void FSound::SetSName(const std::string& _name){
	sname = _name;
}
bool FSound::IsPlaying(){
	return isPlaying;
}
//__________________Owner List________________________________________________________________________

int OwnerList::FindAttribute(char* _tag){
	std::string attribute = std::string(_tag);//converts char* to string
	int index = -1;//if Sound with specific attribute is not found defualt return -1
	for (int i = 0; i < this->list.size(); i++){//goes through OwnerList list
		if (this->list[i]->attribute == attribute){//if attribute looked for is the same as in sound
			index = i;//record index
		}
	}
	if (index != -1)//if specific sound was found 
		return index;//return that sounds index
	else{
		std::cout << "cOL->FA->Attribute not found " << attribute << std::endl;
		return -1;
	}
}

void OwnerList::FindAndPlay(char* _tag){
	int index = FindAttribute(_tag);
	if (index != -1){
		list[index]->Play();
	}
	else{
		std::cout << "cOW->FAP-> Could not find and play: " << _tag << std::endl;
	}
}

void OwnerList::PlayAndPauseAll(){//Plays and Pauses all Sounds
	for (int i = 0; i < list.size(); i++){
		list[i]->Play();
		list[i]->ChannelPtr->setPaused(true);
	}
}
void OwnerList::PlayAndPause(char* _tag){//Plays and Pause specific sound
	int index = FindAttribute(_tag);

	if (index != -1){
		list[index]->ChannelPtr->setPaused(true);
	}
	else{
		std::cout << "cOW->FAP-> Could not find and play: " << _tag << std::endl;
	}
}
void OwnerList::FindAndUnpause(char* _tag){//finds and unpauses specific sound
	int index = FindAttribute(_tag);
	if (index != -1){
		list[index]->ChannelPtr->setPaused(false);
	}
	else{
		std::cout << "cOW->FAUP-> Could not find and unpause " << _tag << std::endl;
	}
}
bool OwnerList::IsPlaying(char* _tag){//returns of sound is playing goes to FSOUND->IsPLaying
	int attribute = FindAttribute(_tag);
	if (attribute != -1){
		return list[attribute]->IsPlaying();
	}
	else{
		std::cout << "cOW->IP-> Could not find attribute " << _tag << std::endl;
	}
}
FSound* OwnerList::FindSound(char* _tag){
	int attribute = FindAttribute(_tag);

	if (attribute != -1)
		return list[attribute];
	else
		std::cout << "cSM->Did not find sound: " << _tag << std::endl;
}
void OwnerList::Stop(char* _tag){
	int index = FindAttribute(_tag);
	if (index != -1){
		list[index]->Stop();
	}
	else{
		std::cout << "cOW->S FAIL STOP SOUND! WRONG/MISSING TAG/FILE" << std::endl;
	}
}
void OwnerList::FindAndPause(char* _tag){
	int index = FindAttribute(_tag);
	if (index != -1){
		list[index]->ChannelPtr->setPaused(true);
	}
	else{
		std::cout << "cOW->FAPause FAIL PAUSE SOUND! WRONG/MISSING TAG/FILE" << std::endl;
	}
}
void OwnerList::UpdateOwnerSoundPos(glm::vec3 _pos){
	for (int c = 0; c < list.size(); c++){
		list[c]->UpdateSoundPos(_pos);
	}
}
//_________________________SOUND MANAGER________________________________________________________________________

SoundManager::SoundManager(FSystem* _FSystemPtr, std::string& _filePath, std::string& _fileName){
	FSystemPtr = _FSystemPtr;
	std::ifstream file;
	std::string name = _filePath + _fileName;//_filePath just the file path without filename, _filename = just fie name together = complete path
	file.open(((name).c_str())); //opens file for reading

	std::string output;
	std::string line;

	if (file.is_open())
	{
		//getline(file, line);
		while (file.good())
		{
			//system("CLS");
			getline(file, line);

			std::string str(line);//reads in a line from a file into a string
			std::string buf; // Have a buffer string
			std::stringstream ss(str); // Insert the string into a stream 

			std::vector<std::string> tokens; // Create vector to hold our words

			while (ss >> buf)
				tokens.push_back(buf);//pushes words delimited by " " into a vector

			SOUND_TYPE STYPE;
			ROLLOFF_TYPE RTYPE;
			std::string tattribute; //temp attribute
			std::string towner; //temp owner
			std::string pname = _filePath + tokens[0]; // pname(path name) = filepath + sound name from text file

			if (tokens.size() == 4){//if 4 word line, means user didnt specify min max Dist, defualt 0.5 100
				//0= FName, 1= Owner, 2 = Attribute, 3 = Sound Type, 
				towner = tokens[1];
				tattribute = tokens[2];
				STYPE = getSoundType(tokens[3]);
				FSound* temp = new FSound(FSystemPtr, pname, STYPE);//creates sound
				temp->SetSName(tokens[0]);//sets name
				temp->soundType = getSoundType(tokens[3]);
				temp->SetOwner(towner);
				temp->SetAttribute(tattribute);
				sounds.push_back(temp);

				///next loops find number of unique owners in txt file
				bool uOwnerF = false; //unique owner found
				if (uOwners.size() == NULL){ //if uOwner vector is empty push back first sound loaded form txt file
					uOwners.push_back(towner);
					//std::cout << towner;
				}
				else{
					for (int i = 0; i < uOwners.size(); i++){//goes through vector of unique owners and compares already added owners to latest sound loaded from a txt file
						if (uOwners[i] == towner)
							uOwnerF = true;//if owner in latest sound is in uOwners lists bool a true

					}
					if (uOwnerF == false)//if last sounds owner type was not in the uOwner list, added
						uOwners.push_back(towner);
				}
			}

			if (tokens.size() == 7){
				//0= FName, 1= Owner, 2 = Attribute, 3 = Sound Type, 4 = Rolloff, 5=minDist, 6=maxDist

				STYPE = getSoundType(tokens[3]);
				RTYPE = getRollOff(tokens[4]);
				float minDist = std::stod(tokens[5], NULL);
				float maxDist = std::stod(tokens[6], NULL);

				towner = tokens[1];
				tattribute = tokens[2];
				FSound* temp = new FSound(FSystemPtr, pname, STYPE);
				temp->SetSName(tokens[0]);
				temp->soundType = getSoundType(tokens[3]);
				temp->SetOwner(towner);
				temp->rollOff = RTYPE;
				temp->SetAttribute(tattribute);
				sounds.push_back(temp);

				///next loops find number of unique owners in txt file
				bool uOwnerF = false;//unique owner found
				if (uOwners.size() == NULL){//if uOwner vector is empty push back first sound loaded form txt file
					uOwners.push_back(towner);
					//std::cout << towner;
				}
				else{
					for (int i = 0; i < uOwners.size(); i++){//goes through vector of unique owners and compares already added owners to latest sound loaded from a txt file
						if (uOwners[i] == towner)
							uOwnerF = true;//if owner in latest sound is in uOwners lists bool a true

					}
					if (uOwnerF == false)//if last sounds owner type was not in the uOwner list, added
						uOwners.push_back(towner);
				}
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
	//for (int i = 0; i < vSounds.size(); i++){
	//	for (int c = 0; c < vSounds[i].size(); c++){
	//		vSounds[i][c]->~FSound();
	//	}
	//}
	delete FSystemPtr;
}
void SoundManager::List(){
	for (int i = 0; i < vSounds.size(); i++)
		for (int j = 0; j < nuOwners[i]; j++)
			std::cout << "\n" << "O:" << i << " " << "A:" << j << " " << vSounds[i][j]->sname << " " << vSounds[i][j]->owner << " " << vSounds[i][j]->attribute;
}
void SoundManager::Sort(){
	//resizes vector nuOwners(number unique Owners) to number of unique owners
	nuOwners.resize(uOwners.size());
	int j = 0;
	while (j < uOwners.size()){//Gets unique owner 
		int count = 0;
		for (int i = 0; i < sounds.size(); i++){//goes through vector of sounds and campares owners in sound to uOwners
			if (uOwners[j] == sounds[i]->owner) // if match is found incremnt count
				count++;
		}

		nuOwners[j] = count;//pushes back number of sounds for a unique owner found in txt file
		j++;//goes to next unique owner and restart loop and utill all are counted
	}

	//resizes vSouds first dimention to number of unique owners
	vSounds.resize(uOwners.size());
	//list.resize(uOwners.size());
	int k = 0;
	while (k < uOwners.size()){//goes through vector of unique owner
		for (int l = 0; l < sounds.size(); l++){//goes through vector of sounds 
			if (uOwners[k] == sounds[l]->owner) //if unique owner is found  in sound list, pushes that sound into 2nd dimetion of vSounds
			{
				vSounds[k].push_back(sounds[l]);
				//list[k].push_back(sounds[l]);
			}
		}
		k++;//goes to next uOwner
	}


	//temp list of sounds
	std::vector<FSound*> tempList;
	int p = 0;
	while (p < uOwners.size()){//goes through vector of Unique owners
		for (int i = 0; i < sounds.size(); i++){//goes through list of all sounds
			if (uOwners[p] == sounds[i]->owner){ //if uOwner tag is the same as in sound
				tempList.push_back(sounds[i]);//push sound in tempList
			}
		}
		if (tempList.size() == nuOwners[p]){//checks if all sounds with specif owner were added to tempVector
			//std::cout << "\n List Full";
			OwnerList* list = new OwnerList(tempList[0]->owner);//creates a new OwnerList with owner tag if that group
			list->list = tempList; // assignst tempList of sound to OwnerList
			oList.push_back(list);//pushes OwnerList into vector of Owners
			tempList.clear(); //clears tempList
		}

		p++;//goes to next unique owner
	}
}

void SoundManager::printOList(){
	for (int i = 0; i < oList.size(); i++)
		for (int j = 0; j < oList[i]->list.size(); j++)
			std::cout << i << " " << j << " " << oList[i]->owner << " " << oList[i]->list[j]->attribute << std::endl;
}
int SoundManager::FindOwner(char* _owner){
	std::string owner = std::string(_owner);//converts char* to string
	bool isFound = false;
	int index = 0;
	for (int i = 0; i < oList.size(); i++){//goes through oList
		if (oList[i]->owner == owner){//if mathc is found
			isFound = true;//record
			index = i;//record index
		}
	}

	if (isFound)//if sound was found
		return index;//return index
	else{
		std::cout << owner << " not found";
		return -1;
	}
}
int SoundManager::FindAttribute(char* _tag){
	bool isFound = false;
	int index = -1;
	for (int i = 0; i < oList.size(); i++){//goes through oList
		index = oList[i]->FindAttribute(_tag);
	}
	if (index != -1)
		return index;
	else{
		return -1;
	}
}
FSound* SoundManager::FindSound(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1)
		return oList[owner]->FindSound(_tag);
	else
		std::cout << "cSM->PS FAIL PLAY PAUSE! WRONG/MISSING TARGET/FILE" << std::endl;
}
void SoundManager::FindAndPlay(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1)
		oList[owner]->FindAndPlay(_tag);
	else{
		std::cout << "cSM->Did not find sound: " << _owner << " " << _tag << std::endl;
	}
}
OwnerList* SoundManager::GetOwnerList(char* _owner){
	int owner = FindOwner(_owner);

	if (owner != -1)
		return oList[owner];
}
void SoundManager::PlayAndPause(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->PlayAndPause(_tag);
	}
	else{
		std::cout << "cSM->PAP FAIL PLAY AND PAUSE! WRONG/MISSING TARGET/FILE" << std::endl;
	}
}
void SoundManager::PlayAndPauseAll(){//goes to OwnerList->PlayAndPauseAll
	for (int i = 0; i < oList.size(); i++){
		oList[i]->PlayAndPauseAll();
	}
}
void SoundManager::FindAndUnpause(char* _owner, char* _tag){//goes to OwnerList->PlayAndUnpause
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->FindAndUnpause(_tag);
	}
}
void SoundManager::Update(){
	for (int i = 0; i < oList.size(); i++)
		for (int j = 0; j < oList[i]->list.size(); j++)
			oList[i]->list[j]->Update();
}

bool SoundManager::IsPlaying(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	return oList[owner]->IsPlaying(_tag);
}
void SoundManager::Stop(char* _owner, char* _tag){
	int owner = FindOwner(_owner);

	if (owner != -1){
		oList[owner]->Stop(_tag);
	}
	else{
		std::cout << "cSM->S FAIL STOP SOUND! WRONG/MISSING OWNER/FILE" << std::endl;
	}
}
void SoundManager::FindAndPause(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->FindAndPause(_tag);
	}
	else{
		std::cout << "cSM->FAPause FAIL STOP SOUND! WRONG/MISSING OWNER/FILE" << std::endl;
	}
}
void SoundManager::UpdateOwnerSoundPos(char* _owner, glm::vec3 _pos){
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->UpdateOwnerSoundPos(_pos);
	}
	else{
		std::cout << "cSM->UWSP FAIL UPDATE SOUND POS WRONG/MISSING OWNER/FILE" << std::endl;
	}
}
void SoundManager::SoundPlaying(){
	for (int c = 0; c < oList.size(); c++){
		for (int i = 0; i < oList[c]->list.size(); i++){
			bool p = oList[c]->list[i]->IsPlaying();
			if (p)
				std::cout << oList[c]->list[i]->sname << std::endl;
		}
	}
}