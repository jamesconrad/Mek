#include "FMODmanager.h"
const int INTERFACE_UPDATETIME = 50;
const float DISTANCEFACTOR = 1.0;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << "FMOD error! (%d) %s\n" << " " << result << " " << FMOD_ErrorString(result);
		//	exit(-1);
	}
}
char* getfPath(){
	string filename;
	char *fPath = new char[128];
	cout << "Enter file path of the sound:";
	cin >> filename;
	for (int i = 0; i < filename.length(); i++){
		if (filename[i] == '\\'){
			filename[i] = '/';
			fPath[i] = filename[i];
		}
	}
	return fPath;
}
void FSystem::Update(){
	//do{
	//	if (_kbhit())
	//	{
	//		key = _getch();
	//	}
	//	SystemPtr->update();
	//	Sleep(INTERFACE_UPDATETIME - 1);
	//} while (key != 27);
	result = SystemPtr->set3DListenerAttributes(0, &listenerpos,&vel, &forward, &up); //up=
	ERRCHECK(result);
	SystemPtr->update();
}
FSystem::FSystem(){
	vel = { 0.0, 0.0, 0.0 };
	forward = { 0.0, 0.0, 1.0 };
	up = { 0.0, 1.0, 0.0 };
	listenerpos = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
	bool             listenerflag = false;
	result = FMOD::System_Create(&SystemPtr);
	ERRCHECK(result);

	result = SystemPtr->getNumDrivers(&numdrivers);
	ERRCHECK(result);

	result = SystemPtr->getVersion(&version);
	ERRCHECK(result);
	cout << "FMOD version:" << version << endl;
	if (numdrivers == 0)
	{
		result = SystemPtr->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(result);
	}
	else
	{
		result = SystemPtr->getDriverCaps(0, &caps, 0, &speakermode); //setting fmods speakmode to sameone as windows
		ERRCHECK(result);

		result = SystemPtr->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
		ERRCHECK(result);

		if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
		{                                                   /* You might want to warn the user about this. */
			result = SystemPtr->setDSPBufferSize(1024, 10);
			ERRCHECK(result);
		}

		result = SystemPtr->getDriverInfo(0, name, 256, 0);
		ERRCHECK(result);
	}

	result = SystemPtr->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
	{
		result = SystemPtr->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ERRCHECK(result);

		result = SystemPtr->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
		ERRCHECK(result);
	}


	/*
	Set the distance units. (meters/feet etc).
	*/

	//doppler and roll off
	result = SystemPtr->set3DSettings(1.0, 1, 1.0f);
	ERRCHECK(result);
	isUp = true;
}
FSystem::~FSystem(){
	result = this->SystemPtr->close();
	ERRCHECK(result);
	//result = this->SystemPtr->release();
	//ERRCHECK(result);
	clear();
}
void FSystem::clear(){
	this->SystemPtr = NULL;
	delete this->SystemPtr;
	numdrivers = NULL;

}
void FSystem::set(FMOD_VECTOR _pos, FMOD_VECTOR _forward, FMOD_VECTOR _up){
	listenerpos = _pos;
	forward = _forward;
	up = _up;
}
FSound::FSound(){
	isSystem = false;
	if (!isSystem){
		FSystem* system1 = new FSystem;
		SystemPtr = system1;
		isSystem = true;
	}
	name = "media/swish.wav";
	sound_type = SOUND_TYPE_2D;
	listenerpos = { 0.0, 0.0, 0.0 };
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	loadSound();
}
FSound::FSound (char *_name, SOUND_TYPE _sound_type){
	isSystem = true;
	name = _name;
	sound_type = _sound_type;
	listenerpos = { 0.0, 0.0, 0.0 };
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	vel = { 0.0, 0.0, 0.0 };
	forward = { 0.0, 0.0, 1.0 };
	up = { 0.0, 1.0, 0.0 };
	loadSound();
}
FSound::FSound(FSystem* _system, char *_name, SOUND_TYPE _sound_type){
	isSystem = true;
	SystemPtr = _system;
	name = _name;
	sound_type = _sound_type;
	listenerpos = { 0.0, 0.0, 0.0 };
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	vel = { 0.0, 0.0, 0.0 };
	forward = { 0.0, 0.0, 1.0 };
	up = { 0.0, 1.0, 0.0 };
	loadSound();
}
FSound::FSound(FSystem* _system, char *_name, SOUND_TYPE _sound_type,float minDist,float maxDist){
	isSystem = true;
	SystemPtr = _system;
	name = _name;
	sound_type = _sound_type;
	listenerpos = { 0.0, 0.0, 0.0 };
	soundPos = { 0.0, 0.0, 0.0 };
	soundVel = { 0.0, 0.0, 0.0 };
	vel = { 0.0, 0.0, 0.0 };
	forward = { 0.0, 0.0, 1.0 };
	up = { 0.0, 1.0, 0.0 };
	loadSound(minDist,maxDist);
}
void FSound::loadSound(){
	switch (sound_type){
	case SOUND_TYPE_2D:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_LOOP_OFF);
		ERRCHECK(result);
		break;
	case SOUND_TYPE_2D_LOOP:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_LOOP_NORMAL);
		ERRCHECK(result);
		break;
	case SOUND_TYPE_3D:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->set3DMinMaxDistance(0.5, 100);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
		ERRCHECK(result);
		//result = ChannelPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
		//ERRCHECK(result);
		break;
	case SOUND_TYPE_3D_LOOP:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->set3DMinMaxDistance(0.5, 100);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL);
		ERRCHECK(result);
		//result = ChannelPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL);
		//ERRCHECK(result);
		break;

	}
	SoundPtr->getLength(&length, FMOD_TIMEUNIT_MS);
}
void FSound::loadSound(float minDist, float maxDist){
	switch (sound_type){
	case SOUND_TYPE_2D:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_LOOP_OFF);
		ERRCHECK(result);
		break;
	case SOUND_TYPE_2D_LOOP:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_2D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_LOOP_NORMAL);
		ERRCHECK(result);
		break;
	case SOUND_TYPE_3D:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->set3DMinMaxDistance(minDist, maxDist);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
		ERRCHECK(result);
		//result = ChannelPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF);
		//ERRCHECK(result);
		break;
	case SOUND_TYPE_3D_LOOP:
		result = SystemPtr->SystemPtr->createSound(name, FMOD_3D, 0, &SoundPtr);
		ERRCHECK(result);
		result = SoundPtr->set3DMinMaxDistance(minDist, maxDist);
		ERRCHECK(result);
		result = SoundPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL);
		ERRCHECK(result);
		//result = ChannelPtr->setMode(FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL);
		//ERRCHECK(result);
		break;

	}
	SoundPtr->getLength(&length, FMOD_TIMEUNIT_MS);
}
FMOD::Channel* FSound::play(){
	result = SystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, true, &ChannelPtr);
	ERRCHECK(result);
	switch (sound_type){
	case SOUND_TYPE_3D:
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		ERRCHECK(result);
		result = ChannelPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_OFF);
		ERRCHECK(result);
		result = ChannelPtr->setPaused(false);
		ERRCHECK(result);
		break;
	case SOUND_TYPE_3D_LOOP:
		result = ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		ERRCHECK(result);
		result = ChannelPtr->setMode(FMOD_3D_LINEARSQUAREROLLOFF | FMOD_LOOP_NORMAL);
		ERRCHECK(result);
		result = ChannelPtr->setPaused(false);
		ERRCHECK(result);
		break;
	default:
		result = ChannelPtr->setPaused(false);
		ERRCHECK(result);
	}
	cout << "Playing: " << name << endl;
	return ChannelPtr;
}
FSound::~FSound(){
	result = this->SoundPtr->release();
	ERRCHECK(result);
}
void FSound::clear(){
	this->SystemPtr = NULL;
	delete this->SystemPtr;
	this->SoundPtr = NULL;
	delete this->SoundPtr;
	this->ChannelPtr = NULL;
	delete this->ChannelPtr;
	listenerpos = { NULL, NULL, NULL };
	sound_type = SOUND_TYPE_NULL;
	char *name = NULL;
	delete name;
	int key = NULL;
	listenerflag = NULL;
}
void FSound::update(){
	
		ChannelPtr->set3DAttributes(&soundPos, &soundVel);
		result = SystemPtr->SystemPtr->set3DListenerAttributes(0, &SystemPtr->listenerpos, &SystemPtr->vel, &SystemPtr->forward, &SystemPtr->up); //up=
		ERRCHECK(result);
		SystemPtr->Update();
}
FSound* FSound::getSound(){
	return this;
}
