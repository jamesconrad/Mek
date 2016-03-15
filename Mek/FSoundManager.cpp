#include "FSoundManager.h"
signed short* dataPtr = NULL;
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
	else{
		return SOUND_TYPE_2D;
	}
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
	else
		return ROLLOFF_LINEAR;
}


//_________________________SOUND MANAGER________________________________________________________________________

FSoundManager::FSoundManager(FSystem* _FSystemPtr, std::string& _filePath, std::string& _fileName){
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
				FSound* temp = new FSound(FSystemPtr, pname, STYPE, RTYPE, minDist, maxDist);
				temp->min = minDist;
				temp->max = maxDist;
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
FSoundManager::~FSoundManager(){
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
void FSoundManager::List(){
	for (int i = 0; i < vSounds.size(); i++)
		for (int j = 0; j < nuOwners[i]; j++)
			std::cout << "\n" << "O:" << i << " " << "A:" << j << " " << vSounds[i][j]->sname << " " << vSounds[i][j]->owner << " " << vSounds[i][j]->attribute;
}
void FSoundManager::Sort(){
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

void FSoundManager::printOList(){
	for (int i = 0; i < oList.size(); i++)
		for (int j = 0; j < oList[i]->list.size(); j++)
			std::cout << i << " " << j << " " << oList[i]->owner << " " << oList[i]->list[j]->attribute << std::endl;
}
int FSoundManager::FindOwner(char* _owner){
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
int FSoundManager::FindAttribute(char* _tag){
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
FSound* FSoundManager::FindSound(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1)
		return oList[owner]->FindSound(_tag);
	else
		std::cout << "cSM->PS FAIL PLAY PAUSE! WRONG/MISSING TARGET/FILE" << std::endl;
}
void FSoundManager::FindAndPlay(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1)
		oList[owner]->FindAndPlay(_tag);
	else{
		std::cout << "cSM->Did not find sound: " << _owner << " " << _tag << std::endl;
	}
}
OwnerList* FSoundManager::GetOwnerList(char* _owner){
	int owner = FindOwner(_owner);

	if (owner != -1)
		return oList[owner];
}
void FSoundManager::PlayAndPause(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->PlayAndPause(_tag);
	}
	else{
		std::cout << "cSM->PAP FAIL PLAY AND PAUSE! WRONG/MISSING TARGET/FILE" << std::endl;
	}
}
void FSoundManager::PlayAndPauseAll(){//goes to OwnerList->PlayAndPauseAll
	for (int i = 0; i < oList.size(); i++){
		oList[i]->PlayAndPauseAll();
	}
}
void FSoundManager::FindAndUnpause(char* _owner, char* _tag){//goes to OwnerList->PlayAndUnpause
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->FindAndUnpause(_tag);
	}
}
void FSoundManager::Update(){
	for (int i = 0; i < oList.size(); i++)
		for (int j = 0; j < oList[i]->list.size(); j++)
			oList[i]->list[j]->Update();
}

bool FSoundManager::IsPlaying(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	return oList[owner]->IsPlaying(_tag);
}
void FSoundManager::Stop(char* _owner, char* _tag){
	int owner = FindOwner(_owner);

	if (owner != -1){
		oList[owner]->Stop(_tag);
	}
	else{
		std::cout << "cSM->S FAIL STOP SOUND! WRONG/MISSING OWNER/FILE" << std::endl;
	}
}
void FSoundManager::FindAndPause(char* _owner, char* _tag){
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->FindAndPause(_tag);
	}
	else{
		std::cout << "cSM->FAPause FAIL STOP SOUND! WRONG/MISSING OWNER/FILE" << std::endl;
	}
}
void FSoundManager::UpdateOwnerSoundPos(char* _owner, glm::vec3 _pos){
	int owner = FindOwner(_owner);
	if (owner != -1){
		oList[owner]->UpdateOwnerSoundPos(&_pos);
	}
	else{
		std::cout << "cSM->UWSP FAIL UPDATE SOUND POS WRONG/MISSING OWNER/FILE" << std::endl;
	}
}
void FSoundManager::SoundPlaying(){
	for (int c = 0; c < oList.size(); c++){
		for (int i = 0; i < oList[c]->list.size(); i++){
			bool p = oList[c]->list[i]->IsPlaying();
			if (p)
				std::cout << oList[c]->list[i]->sname << std::endl;
		}
	}
}
FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *sound, void *data, unsigned int datalen)
{
	unsigned int count;
	static long current = 0;
	signed short *stereo16bitbuffer = (signed short *)data;

	for (count = 0; count<datalen >> 1; count++)        // >>2 = 16bit stereo (4 bytes per sample)
	{
		*stereo16bitbuffer++ = dataPtr[current];    // left channel
		// *stereo16bitbuffer++ = dataPtr[current+1];    // right channel

		current++;//+= 2;
	}

	return FMOD_OK;
}
FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND *sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
{
	/*
	This is useful if the user calls Channel::setPosition and you want to seek your data accordingly.
	*/
	return FMOD_OK;
}
void FSoundManager::Amplify(char* _file, float _amp){
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	int  channels;
	float sample;
	float amp = _amp;
	std::vector<signed short> rawData;

	LoadRawData(_file, FSystemPtr->SystemPtr, &rawData, &channels);
	//LoadRawData("Sounds/drumloop.wav", system, &rawData, &channels);

	//Now we can make changes to the raw data
	std::vector<signed short> newData;
	newData.resize(rawData.size(), 0);

	//Loop through all the samples making changes
	if (channels == 1)//make sure it's a mono sound
	{
		float sample;
		//
		////Amplify
		float amp = 80.0f;
		for (unsigned int s = 0; s<rawData.size(); s++)
		{
			sample = float(rawData[s]);
			sample = sample*amp;

			sample = int(sample / 32767.0f * 3.0f) * (32767.0f / 3.0f);

			//Make sure it's not out of bounds
			if (sample >= 32767.0f){ sample = 32767.0f; }
			if (sample<-32767.0f){ sample = -32767.0f; }
			newData[s] = (signed short)sample;
		}
	}

	//Now create a playable sound from the raw data
	CreateSoundFromData(FSystemPtr->SystemPtr, &SoundPtr, channels, 44000, true, &newData);

	//Play the sound
	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, false, &ChannelPtr);
	ERRCHECK(result);
}
void FSoundManager::Echo(char* _file, int _delay, int _echos){
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	int  channels;
	float sample;

	std::vector<signed short> rawData;

	LoadRawData(_file, FSystemPtr->SystemPtr, &rawData, &channels);
	//LoadRawData("Sounds/drumloop.wav", system, &rawData, &channels);

	//Now we can make changes to the raw data
	std::vector<signed short> newData;
	newData.resize(rawData.size(), 0);

	//Loop through all the samples making changes
	int delay = 8000;
	int echos = 5;
	float falloff = 0.5f;
	for(unsigned int s = 0; s<rawData.size(); s++)
	{
		sample = float(rawData[s]);
		float amount = falloff;
		for(unsigned int e = 1; e<=echos; e++)
		{
			sample += float(rawData[(s+e*delay)%(rawData.size())])*amount;
			amount *= falloff;
		}
		
		//Make sure it's not out of bounds
		if(sample>=32767.0f){sample =  32767.0f;}
		if(sample<-32767.0f){sample = -32767.0f;}
		newData[s] = (signed short)sample;
	}

	CreateSoundFromData(FSystemPtr->SystemPtr, &SoundPtr, channels, 44000, true, &newData);

	//Play the sound
	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, false, &ChannelPtr);
	ERRCHECK(result);

}
void FSoundManager::Convolution(char* _file, int _filterWidth, int _amp){
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	int  channels;
	float sample;

	std::vector<signed short> rawData;

	LoadRawData(_file, FSystemPtr->SystemPtr, &rawData, &channels);
	//LoadRawData("Sounds/drumloop.wav", system, &rawData, &channels);

	//Now we can make changes to the raw data
	std::vector<signed short> newData;
	newData.resize(rawData.size(), 0);

	int filterWidth = _filterWidth;
	float amp = _amp;
	for (unsigned int s = 0; s<rawData.size(); s++)
	{
		sample = 0.0f;
		for (unsigned int f = 0; f<filterWidth; f++)
		{
			sample += float(rawData[(s + f) % (rawData.size())]) / float(filterWidth);
		}
		sample = sample*amp;

		//Make sure it's not out of bounds
		if (sample >= 32767.0f){ sample = 32767.0f; }
		if (sample<-32767.0f){ sample = -32767.0f; }
		newData[s] = (signed short)sample;
	}

	CreateSoundFromData(FSystemPtr->SystemPtr, &SoundPtr, channels, 44000, true, &newData);

	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, false, &ChannelPtr);
	ERRCHECK(result);
}
void FSoundManager::Robot(char* _file, int _pitch, int _distortion, int _amp){
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	int  channels;
	float sample;

	std::vector<signed short> rawData;

	LoadRawData(_file, FSystemPtr->SystemPtr, &rawData, &channels);
	//LoadRawData("Sounds/drumloop.wav", system, &rawData, &channels);

	//Now we can make changes to the raw data
	std::vector<signed short> newData;
	newData.resize(rawData.size(), 0);

	int pitch = _pitch;
	int distortion = _distortion;
	float amp = _amp;
	float weight = 0.0f;
	for (unsigned int s = 0; s<rawData.size(); s++)
	{
		sample = 0.0f;
		for (unsigned int d = 0; d <= distortion; d++)
		{
			weight = 1.0f / float(distortion);
			sample += float(rawData[(s + d*pitch) % (rawData.size())])*weight;
		}
		sample *= amp;

		//Make sure it's not out of bounds
		if (sample >= 32767.0f){ sample = 32767.0f; }
		if (sample<-32767.0f){ sample = -32767.0f; }
		newData[s] = (signed short)sample;
	}

	CreateSoundFromData(FSystemPtr->SystemPtr, &SoundPtr, channels, 44000, true, &newData);

	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, false, &ChannelPtr);
	ERRCHECK(result);
}
void FSoundManager::Fishman(char* _file, float _volume, float _amount){
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	int  channels;
	float sample;

	std::vector<signed short> rawData;

	LoadRawData(_file, FSystemPtr->SystemPtr, &rawData, &channels);
	//LoadRawData("Sounds/drumloop.wav", system, &rawData, &channels);

	//Now we can make changes to the raw data
	std::vector<signed short> newData;
	newData.resize(rawData.size(), 0);

	float volume = _volume;
	float amount = _amount;
	for (unsigned int s = 0; s<rawData.size(); s++)
	{
		int offset = int(amount*(sin(float(s*0.00157f)) + 1.0f));
		sample = float(rawData[(s + offset) % (rawData.size())]);
		sample = sample*volume;

		//Make sure it's not out of bounds
		if (sample >= 32767.0f){ sample = 32767.0f; }
		if (sample<-32767.0f){ sample = -32767.0f; }
		newData[s] = (signed short)sample;
	}

	CreateSoundFromData(FSystemPtr->SystemPtr, &SoundPtr, channels, 44000, true, &newData);

	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, false, &ChannelPtr);
	ERRCHECK(result);
}
void FSoundManager::BadConnection(char* _file, float _levels){
	FMOD::Sound* SoundPtr;
	FMOD::Channel* ChannelPtr;
	int  channels;
	float sample;

	std::vector<signed short> rawData;

	LoadRawData(_file, FSystemPtr->SystemPtr, &rawData, &channels);
	//LoadRawData("Sounds/drumloop.wav", system, &rawData, &channels);

	//Now we can make changes to the raw data
	std::vector<signed short> newData;
	newData.resize(rawData.size(), 0);

	float levels = _levels;
	float tempLevel;
	float radioStatic = 1000.0f;
	for (unsigned int s = 0; s<rawData.size(); s++)
	{
		tempLevel = (float(rand() % 1000) / 999.0f)*levels;
		sample = float(rawData[s]) / 32767.0f;
		sample = float(int(sample*tempLevel + 0.5f));
		sample = (sample / tempLevel)*32767.0f;
		sample = sample + (float(rand() % 1000) / 1000.0f + 0.5f)*radioStatic;

		//Make sure it's not out of bounds
		if (sample >= 32767.0f){ sample = 32767.0f; }
		if (sample<-32767.0f){ sample = -32767.0f; }
		newData[s] = (signed short)sample;
	}

	CreateSoundFromData(FSystemPtr->SystemPtr, &SoundPtr, channels, 44000, true, &newData);

	result = FSystemPtr->SystemPtr->playSound(FMOD_CHANNEL_FREE, SoundPtr, false, &ChannelPtr);
	ERRCHECK(result);
}
bool FSoundManager::LoadRawData(const char * file_name, FMOD::System* system, std::vector<signed short>* rawData, int* channels)
{
	FMOD::Sound* sound;

	FMOD_RESULT result;
	result = system->createStream(file_name, FMOD_OPENONLY | FMOD_ACCURATETIME, 0, &sound);
	ERRCHECK(result);

	void *data;
	unsigned int length = 0, read;

	//Get the number of samples
	result = sound->getLength(&length, FMOD_TIMEUNIT_PCMBYTES);
	ERRCHECK(result);

	//Get the number of channels
	FMOD_SOUND_TYPE type;
	FMOD_SOUND_FORMAT format;
	int bits;
	sound->getFormat(&type, &format, channels, &bits);

	//Get the raw data
	rawData->clear();
	rawData->reserve(length / 2);
	signed short tmpData;
	data = (void*)&tmpData;

	for (unsigned int i = 0; i<length / 2; i++)
	{
		result = sound->readData((char *)data, 2, &read);
		if (result == FMOD_OK && read == 2)
		{
			rawData->push_back(tmpData);
		}
		else
		{
			i = length / 2;
		}
	}

	return true;
}
bool FSoundManager::CreateSoundFromData(FMOD::System* system, FMOD::Sound **sound, int channels, unsigned int frequency, bool loop, std::vector<signed short>* rawData)
{
	FMOD_RESULT   result;
	FMOD_MODE mode = FMOD_2D | FMOD_OPENUSER | FMOD_HARDWARE;
	if (loop){ mode = mode | FMOD_LOOP_NORMAL; }
	FMOD_CREATESOUNDEXINFO  createsoundexinfo;
	dataPtr = &((*rawData)[0]);

	if (channels<1){ channels = 1; }
	if (channels>2){ channels = 2; }

	memset(&createsoundexinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	createsoundexinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);              /* required. */
	createsoundexinfo.decodebuffersize = rawData->size();                                       /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
	createsoundexinfo.length = rawData->size() * 2;//44100 * channels * sizeof(signed short) * 5; /* Length of PCM data in bytes of whole song (for Sound::getLength) */
	createsoundexinfo.numchannels = channels;                                    /* Number of channels in the sound. */
	createsoundexinfo.defaultfrequency = frequency;                                       /* Default playback rate of sound. */
	createsoundexinfo.format = FMOD_SOUND_FORMAT_PCM16;                     /* Data format of sound. */
	createsoundexinfo.pcmreadcallback = pcmreadcallback;                             /* User callback for reading. */
	createsoundexinfo.pcmsetposcallback = pcmsetposcallback;                           /* User callback for seeking. */

	result = system->createSound(0, mode, &createsoundexinfo, sound);
	ERRCHECK(result);

	return true;
}
