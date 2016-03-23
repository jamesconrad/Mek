#include "FSound.h"

namespace {
	FMOD_RESULT result;

	void ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cout << "FMOD error! (%d) %s\n" << " " << result << " " << FMOD_ErrorString(result);
			//	exit(-1);
		}
	}
}

//FSOUND________________________________________________________________________________________
//VARIABLES: FSystem && ConstChar* Name && SOUND_TYPE
FSound::FSound(){
	FSystemPtr = NULL;
	SoundPtr = NULL;
	ChannelPtr = NULL;
}
FSound::FSound(FSystem* _fsystem, std::string _name, SOUND_TYPE _soundType){
	FSystemPtr = _fsystem;
	fsname = _name.c_str();
	soundType = _soundType;
	soundPos = { 99.0, 99.0, 99.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	isPlaying = false;
	fastForward = false;
	LoadSound();
	playDoor = false;
	activeVel = { 0.0, 0.0, 0.0 };
}
FSound::FSound(FSystem* _fsystem, std::string _name, SOUND_TYPE _soundType, ROLLOFF_TYPE _rolloff, float _minDist, float _maxDist){
	FSystemPtr = _fsystem;
	fsname = _name.c_str();
	soundType = _soundType;
	soundPos = { 99.0, 99.0, 99.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	rollOff = _rolloff;
	isPlaying = false;
	fastForward = false;
	LoadSound(_minDist, _maxDist);
	playDoor = false;
	activeVel = { 0.0, 0.0, 0.0 };
	activeMax = _maxDist;
	activeMin = _minDist; 
	activeVoulme = 1;
}
FSound::~FSound(){
	result = SoundPtr->release();
	if (result != FMOD_OK){
		std::cout << "Failed to Release Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
	}
	Clear();
}
void FSound::LoadSound(){
	//Picks appropriate SOUND_TYPE and creates a sound with that type

	switch (soundType){
	case SOUND_TYPE_2D:
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_OFF);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_2D_LOOP:
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_NORMAL);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_3D:
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(0.5, 100);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
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
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(0.5, 100);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		switch (rollOff){
		case ROLLOFF_LINEAR:
			result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF && FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_LINEARSQUARE:
			result = SoundPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF && FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		case ROLLOFF_INVERSE:
			result = SoundPtr->setMode(FMOD_3D_INVERSEROLLOFF && FMOD_LOOP_NORMAL);
			if (result != FMOD_OK){
				std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
			}
			break;
		}
		break;

	}
	result = SoundPtr->getLength(&length, FMOD_TIMEUNIT_MS);
	if (result != FMOD_OK){
		std::cout << "Failed to get Sound Length in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
	}

}
void FSound::LoadSound(float _minDist, float _maxDist){
	switch (soundType){
	case SOUND_TYPE_2D:
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_OFF);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_2D_LOOP:
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_2D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->setMode(FMOD_LOOP_NORMAL);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound Mode: "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_3D:
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(_minDist, _maxDist);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
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
		result = FSystemPtr->SystemPtr->createSound(fsname, FMOD_3D, 0, &SoundPtr);
		if (result != FMOD_OK){
			std::cout << "Failed to Create Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = SoundPtr->set3DMinMaxDistance(_minDist, _maxDist);
		if (result != FMOD_OK){
			std::cout << "Failed to set Sound 3D Min/Max Distance in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
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
		std::cout << "Failed to get Sound Length in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
	}

}
void FSound::Update(){
	distToSys = sqrt((pow((FSystemPtr->sysPos.x - soundPos.x), 2)) + (pow((FSystemPtr->sysPos.y - soundPos.y), 2)) + (pow((FSystemPtr->sysPos.z - soundPos.z), 2)));
	UpdateDistToNode();
	if (!soundInsideNode){
			activePos = soundPos;
	}
	ChannelPtr->isPlaying(&isPlaying);
	//GetSpectrum();
	//FSystemPtr->cm->Update();
	if (fastForward){
		
		ChannelPtr->setFrequency(22000);
	}
	if (!fastForward){
		ChannelPtr->setFrequency(44000);
	}

	if ((isPlaying && soundType == SOUND_TYPE_3D) || (isPlaying && soundType == SOUND_TYPE_3D_LOOP)){
		ChannelPtr->setVolume(activeVoulme);
		ChannelPtr->set3DMinMaxDistance(activeMin,activeMax);
		if (attribute == "Moving")
			std::cout << "v: " << activeVoulme << "min: " << activeMin << "max: " << activeMax << " ip: " << isPlaying << " p: " << activePos.x << " " << activePos.y << " " << activePos.z << std::endl;
		result = ChannelPtr->set3DAttributes(&activePos, &activeVel);
		if (result != FMOD_OK){
			std::cout << "cFS->U Failed to set Channel 3D Attributes in: " << sname << " "; ERRCHECK(result); std::cout << std::endl;

		}
	}
}
void FSound::Clear(){
	FSystemPtr = NULL;
	delete FSystemPtr;

	SoundPtr = NULL;
	delete SoundPtr;

	ChannelPtr = NULL;
	delete ChannelPtr;

	fsname = NULL;
	delete fsname;
}
FMOD::Channel* FSound::Play(){
	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, true, &ChannelPtr);
	if (result != FMOD_OK){
		std::cout << "Failed to Play Sound: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
	}

	switch (soundType){
	case SOUND_TYPE_3D:
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		if (result != FMOD_OK){
			std::cout << "Failed to set Channel 3D Attributes in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_OFF);
		if (result != FMOD_OK){
			std::cout << "Failed to set Mode: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setPaused(false);
		if (result != FMOD_OK){
			std::cout << "Failed to Unpause: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}
		break;
	case SOUND_TYPE_3D_LOOP:
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		if (result != FMOD_OK){
			std::cout << "Failed to set Channel 3D Attributes in: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_NORMAL);
		if (result != FMOD_OK){
			std::cout << "Failed to set Mode: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
		}

		result = ChannelPtr->setPaused(false);
		if (result != FMOD_OK){
			std::cout << "Failed to Unpause: " << fsname << " "; ERRCHECK(result); std::cout << std::endl;
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
void FSound::DistanceToFSystem(){
	FMOD_VECTOR sp = FSystemPtr->sysPos;
	FMOD_VECTOR ssp = soundPos;

	std::cout << sname << " to system: " << sqrt(pow((sp.x - ssp.x), 2) + pow((sp.y - ssp.y), 2 + pow((sp.z - ssp.z), 2))) << std::endl;
}
void FSound::printSound(){
	std::cout << "Name: " << sname << std::endl;;
	std::cout << "Owner: " << owner << std::endl;
	std::cout << "Tag: " << attribute << std::endl;
	std::cout << "SoundType: " << soundType << std::endl;
	std::cout << "RollOff: " << rollOff << std::endl;
	std::cout << "min: " << min << std::endl;
	std::cout << "Max: " << max << std::endl;
	printPos();
}
void FSound::GetSpectrum(){
	float volume = 0;
	//ChannelPtr->getVolume(&volume);
	if (soundType==SOUND_TYPE_3D || soundType==SOUND_TYPE_3D_LOOP){
		volume = distToSys / max;
	}
	this->ChannelPtr->getSpectrum(spectrum, 128, 0, FMOD_DSP_FFT_WINDOW_RECT);
	for (int i = 0; i<100; i++)
	{	//console only supportrs 100 characters
		int freq = int(spectrum[i] * 100.0f*(1-volume));
		for (int y = 0; y < freq; y++)
		{	
			if (owner == std::string("Player")){
				FSystemPtr->cm->SetForegroundColor(i, 49 - y, 12);
				FSystemPtr->cm->SetBackgroundColor(i, 49 - y, 4);
			}
			else if (owner == std::string("Background")){
				FSystemPtr->cm->SetForegroundColor(i, 49 - y, 1);
				FSystemPtr->cm->SetBackgroundColor(i, 49 - y, 4);
			}
			else if (owner == std::string("Target")){
				FSystemPtr->cm->SetForegroundColor(i, 49 - y, 2);
				FSystemPtr->cm->SetBackgroundColor(i, 49 - y, 4);
			}
			else if (owner == std::string("Tutorial")){
				FSystemPtr->cm->SetForegroundColor(i, 49 - y, 1);
				FSystemPtr->cm->SetBackgroundColor(i, 49 - y, 2);
			}
		}
	}
}
void FSound::UpdateDistToNode(){
	if (FSystemPtr->nodes.size() != NULL){
		for (int c = 0; c < FSystemPtr->nodes.size(); c++){
			float rdistToNode = sqrt((pow(FSystemPtr->nodes[c]->pos.x - soundPos.x, 2)) + (pow(FSystemPtr->nodes[c]->pos.y - soundPos.y, 2)) + (pow(FSystemPtr->nodes[c]->pos.z - soundPos.z, 2)));
			if ((rdistToNode <= FSystemPtr->nodes[c]->min)){
				node = FSystemPtr->nodes[c];
				soundInsideNode = true;
				distToNode = rdistToNode;
				FindDoor();
				USoundSets();
				if (FSystemPtr->nodes[c]->canSee.x >-(FSystemPtr->nodes[c]->min/2) && FSystemPtr->nodes[c]->canSee.x <(FSystemPtr->nodes[c]->min/2)){
					seeNode = true;
				}
				else{
					seeNode = false;
				}
			}
			else{
				soundInsideNode = false;
				activePos = soundPos;
			}
		}
	}
	
}
void FSound::FindDoor(){
	ReverbNode* temp = new ReverbNode;
	bool isFound = false;
	float closest = 99999;
	if (FSystemPtr->nodes.size() != NULL){		
		for (int c = 0; c < FSystemPtr->nodes.size(); c++){
			if ((FSystemPtr->nodes[c]->isDoor)){
				//std::cout << FSystemPtr->nodes[c]->name << " " << FSystemPtr->nodes[c]->distToSys << " ";
				//FSystemPtr->nodes[c]->PrintPos();
				//std::cout << std::endl;
				if (FSystemPtr->nodes[c]->distToSys < closest){
					closest = FSystemPtr->nodes[c]->distToSys;
					door = FSystemPtr->nodes[c];
					if (door != NULL && node != NULL){
						float tv = sqrt((pow(door->pos.x - node->pos.x, 2)) + (pow(door->pos.y - node->pos.y, 2)) + (pow(door->pos.z - node->pos.z, 2)));
						doorVolume = 1/tv*5;
					}
				}
			}
		}
	}
	if (isFound){
		//std::cout << door->name << " " << closest << std::endl;
	}
}
void FSound::USoundSets(){
	if (node != NULL && door != NULL){
		if (attribute == "two"){
		if (node->distToSys > node->min){
			sysOut = true;
		}
		if (node->distToSys < node->min || seeNode){
			soundInsideNode = true;
			sysOut = false;
		}
		if (seeNode && node->distToSys < min){
			playDoor = false;
		}
		else{

			FMOD_VECTOR posd = door->pos;
			soundPosD = posd;
			playDoor = true;
			soundInsideNode = false;
		}
			//std::cout << "Inside: " << soundInsideNode << std::endl;
			//std::cout << "PLayDoor " << playDoor << std::endl;
			//std::cout << "SeeNode " << seeNode << std::endl;
			//std::cout << "SysOut " << sysOut << std::endl;
			float v, tmin, tmax;
			ChannelPtr->getVolume(&v);
			ChannelPtr->get3DMinMaxDistance(&tmin, &tmax);
			if (soundInsideNode){
				activeMax = node->max;
				activeMin = node->max;
				activePos = soundPos;
				activeVoulme = 1;
				//std::cout << node->name << "volume: " << v << " min: " << tmin << " max" << tmax << std::endl;
			}
			else if(playDoor){
				activeMax = node->max*doorVolume/10;
				activeMin = node->max*doorVolume/10;
				activePos = soundPos;
				activeVoulme = doorVolume / 10;
				//std::cout << door->name << "volume: " << v << " min: " << tmin << " max" << tmax << std::endl;
			}
		}
	}
}

