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
	activeNetwork = false;
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
	activeNetwork = false;
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
	//if (attribute == "two")
	//if (!soundInsideNode){
	//	//std::cout << "Sound Not Inside Node" << std::endl << std::flush;
	//	if (!playDoor){
	//		//std::cout << "Sound Not at Door" << std::endl << std::flush;
	//		activePos = soundPos;
	//	}
	//	activePos = door->pos;
	//}
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
		//if (attribute == "two")
		//	std::cout << "distance Sound: " << distToSys << " v: " << activeVoulme << "min: " << activeMin << "max: " << activeMax << " ip: " << isPlaying << " p: " << activePos.x << " " << activePos.y << " " << activePos.z << std::endl;
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
				distToNode = rdistToNode;
				door = FSystemPtr->closestDoor;
				USoundSets();
			}
			else{
				node = NULL;
			}
		}
	}
	
}
void FSound::FindDoor(){
	//ReverbNode* temp = new ReverbNode;
	//bool isFound = false;
	//float closest = 99999;
	//if (FSystemPtr->nodes.size() != NULL){
	//	for (int c = 0; c < FSystemPtr->nodes.size(); c++){
	//		if ((FSystemPtr->nodes[c]->isDoor)){
	//			if (FSystemPtr->nodes[c]->distToSys < closest){
	//				closest = FSystemPtr->nodes[c]->distToSys;
	//				door = FSystemPtr->nodes[c];
	//			}
	//		}
	//	}
	//}
	//for (int c = 0; c < nodes.size(); c++){
	//
	//}
}
void FSound::USoundSets(){
	//system("cls");
	if (node != NULL && door != NULL){
		if (!activeNetwork){
			activeNetwork = true;
			InitNetwork();
		}
		if (attribute == "two"){
			
		networkPtr->bellmanFord(node->name, FSystemPtr->closestDoor->name);
		//std::cout << "Distance " << networkPtr->distance << std::endl;
		
		if (node->name == FSystemPtr->closestDoor->name){
			playDoor = false;
			soundInsideNode = true;
			Gate = false;
			//std::cout << "inisde " << std::endl;
		}
		if ((node->name != FSystemPtr->closestDoor->name)){
				sysOut = true;
				soundInsideNode = false;
				playDoor = true;
				Gate = false;
				//std::cout << "Door " << std::endl;
			}
		if (node->name != FSystemPtr->closestDoor->name && FSystemPtr->closestDoor->distToSys < FSystemPtr->closestDoor->min/2){
				soundInsideNode = false;
				Gate = true;
				sysOut = false;
				playDoor = true;
				//std::cout << "Gate " << std::endl;
			}
			//if(node->distToSys <=node->min){
			//	
			//}
			UpdateTypeActivePos();
			switch (typeOfActivePos){
			case 0:
				activeMin = node->max;
				activeMax = node->min;
				activePos = door->pos;
				activeVoulme = 1-(1 /networkPtr->distance);
			//	std::cout << "v " << activeVoulme << "am: " << activeMin << " aM: " << activeMax << " p:" << door->name << " n: " << networkPtr->next << " p: " << activePos.x << " " << activePos.y << " " << activePos.z << std::endl;
				break;
			case 1:
				activePos = soundPos;
				activeMax = networkPtr->distance;
				activeVoulme = 1 - (1 / networkPtr->distance);
				//std::cout << "am: " << activeMin << " aM: " << activeMax << " p:" << door->name << " n: " << networkPtr->next << std::endl;
				break;
			case 2:
				//std::cout << "v " << activeVoulme << "am: " << activeMin << " aM: " << activeMax << " p:" << door->name << " n: " << networkPtr->next << " p: " << activePos.x << " " << activePos.y << " " << activePos.z << std::endl;
				activeMin = min;
				activeMax = max;
				activePos = soundPos;
				activeVoulme = (1);
				break;
			}
			//std::cout << "typeOfActivePos: " << typeOfActivePos << std::endl << std::flush;
			//std::cout << "Insider " << soundInsideNode << std::endl<< std::flush;
			//std::cout << "PLayDoor " << playDoor << std::endl<< std::flush;
			//std::cout << "Gate " << Gate << std::endl << std::flush;
		}
	}
}
void FSound::UpdateTypeActivePos(){
	if (node != NULL && door != NULL){
		//Play at Door
		if (!soundInsideNode && playDoor && !Gate){
			typeOfActivePos = 0;
		}
		//Play at gate
		else if (Gate && playDoor && !soundInsideNode){
			typeOfActivePos = 1;
		}
		//Play Insider
		else if (soundInsideNode && !playDoor && !Gate){
			typeOfActivePos = 2;
		}
	}
}
void FSound::InitNetwork(){
	networkPtr = new network;
	std::vector<ReverbNode*> list = FSystemPtr->GetNodes();
	std::cout << list.size();
	networkPtr->init(list);
	networkPtr->bellmanFord(node->name, door->name);
}
