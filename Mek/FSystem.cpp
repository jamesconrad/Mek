#include "FSystem.h"
namespace {
	FMOD_RESULT result;
	char name[256];

	void ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cout << "FMOD error! (%d) %s\n" << " " << result << " " << FMOD_ErrorString(result);
			//	exit(-1);
		}
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

	result = SystemPtr->set3DSettings(1, 1, 1);
	if (result != FMOD_OK){
		std::cout << "Failed to set 3D Settings! Error: "; ERRCHECK(result); std::cout << std::endl;
	}
}
FSystem::~FSystem(){
	for (int c = 0; c < nodes.size(); c++){
		nodes[c]->destruct = true;
	}
	result = SystemPtr->close();
	if (result != FMOD_OK){
		std::cout << "Failed to Close System: "; ERRCHECK(result); std::cout << std::endl;
	}
	Clear();
}
void FSystem::Update(){
	if (closestDoor!=NULL)
	std::cout << closestDoor->name << " " << closestDoor->distToSys << std::endl;
	UpdateNodes();
	result = SystemPtr->set3DListenerAttributes(0, &sysPos, &sysVel, &sysFor, &sysUp); //up=
	if (result != FMOD_OK){
		std::cout << "Failed to set 3D Listerner Attributes in System: "; ERRCHECK(result); std::cout << std::endl;
	}
	SystemPtr->update();
}
void FSystem::Set(FMOD_VECTOR _pos, FMOD_VECTOR _for, FMOD_VECTOR _up, FMOD_VECTOR _vel){
	sysPos = _pos;
	sysFor = _for;
	sysUp = _up;
	sysVel = _vel;
}
void FSystem::Clear(){
	SystemPtr = NULL;
	delete SystemPtr;
}
ReverbNode* FSystem::CreateReverb(char* _name, FMOD_REVERB_PROPERTIES _props, float _x, float _y, float _z, float _min, float _max, bool _isDoor){
	ReverbNode* node = new ReverbNode();
	node->isDoor = _isDoor;
	node->name = _name;
	node->init(this,_props, _x, _y, _z, _min, _max);
	nodes.push_back(node);
	return node;
}
void FSystem::SetReverbNodesActive(){
	for (int c = 0; c < nodes.size(); c++){
		nodes[c]->SetActive();
	}
}
void FSystem::SetReverbNodesActiveFalse(){
	for (int c = 0; c < nodes.size(); c++){
		nodes[c]->SetActiveFalse();
	}
}
void FSystem::NodeDistToSys(){
	for (int c = 0; c < nodes.size(); c++){
		nodes[c]->PrintDistToSys();
	}
}
void FSystem::UpdateNodes(){
	for (int c = 0; c < nodes.size(); c++){
		nodes[c]->Update();
	}
	FindDoor();
}
void FSystem::GetChannelsPlaying(){
	int channels,cpu;
	float dsp,stream,geometry,update,total;
	SystemPtr->getChannelsPlaying(&channels);
	SystemPtr->getCPUUsage(&dsp, &stream, &geometry, &update, &total);
	std::cout << "Channels: " << channels << " DSP: " << dsp << " Stream: " << stream << " Update: " << update << " Total: " << total << std::endl;
}
ReverbNode* FSystem::FindNode(std::string _node){
	for (int c = 0; c < nodes.size(); c++){
		if (nodes[c]->name == _node)
			return nodes[c];
	}
	std::cout << _node << " not found" << std::endl;
}
void FSystem::PrintNodesAndLinks(){
	for (int c = 0; c < nodes.size(); c++){
		nodes[c]->PrintLinks();
	}
}
void FSystem::FindDoor(){
	bool Found = false;
	ReverbNode* temp;
	for (int c = 0; c < nodes.size(); c++){
		if (nodes[c]->distToSys <= nodes[c]->min){
			if (closestDoor == NULL){
				closestDoor = nodes[c];
				//std::cout << closestDoor->name << " " << closestDoor->distToSys << std::endl;
			}
			else if (closestDoor->distToSys > nodes[c]->distToSys){
				closestDoor = nodes[c];
				
			}
			  
				//for (int j = 0; j < closestDoor->links.size(); j++){
				//	if (closestDoor->links[j]->distToSys <= closestDoor->min){
				//		nextDoor = closestDoor->links[j];
				//		std::cout<< "Next door " << nextDoor->name << std::endl;
				//	}
				//
			//}
			//else if ((nodes[c]->distToSys < closestDoor->distToSys)){
			//	nextDoor = nodes[c];
			//	std::cout << closestDoor->name << " " << closestDoor->distToSys << " " << nextDoor->name <<std::endl;
			//}
		}
	}
}
//_____________________REVERB NODE___________________________________________
ReverbNode::ReverbNode(){
	freverb = NULL;
}
void ReverbNode::init(FSystem* _fsystem, FMOD_REVERB_PROPERTIES _props, float _x, float _y, float _z, float _min, float _max){
	FSystemPtr = _fsystem;
	FSystemPtr->SystemPtr->createReverb(&freverb);
	freverb->setProperties(&_props);
	this->pos.x = _x;
	this->pos.y = _y;
	this->pos.z = _z;
	min = _min;
	max = _max;
	freverb->set3DAttributes(&pos, _min, _max);
	freverb->setActive(true);
	destruct = false;
}
ReverbNode::~ReverbNode(){
	if (destruct){
		freverb->release();
			std::cout << "ITS OVER";
	}
}
void ReverbNode::Update(){
	UpdateDist();
}
void ReverbNode::UpdateDist(){
	distToSys = sqrt((pow(FSystemPtr->sysPos.x - pos.x, 2)) + (pow(FSystemPtr->sysPos.y - pos.y, 2)) + (pow(FSystemPtr->sysPos.z - pos.z, 2)));
	//if (name == "Warehouse Door" || name == "warehouse")
	//	std::cout << name << " " << distToSys << std::endl;
}
void ReverbNode::PrintDistToSys(){
	std::cout << distToSys << " " << pos.x << " " << pos.y << " " << pos.z << " " << std::endl;
}
void ReverbNode::AddLink(ReverbNode* node){
	links.push_back(node);
}
void ReverbNode::PrintLinks(){
	for (int c = 0; c < links.size();c++){
		std::cout << name << " -> " << links[c]->name << std::endl;
	}
}

