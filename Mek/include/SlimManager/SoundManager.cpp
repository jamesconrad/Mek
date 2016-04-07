#include "SoundManager.h"

void SoundManager::Init(SoundSystem* _system, std::vector<RNode*> _rnodes, std::string _filePath, std::string _fileName){
	m_SoundSystemPtr = _system;
	m_rnodes = _rnodes;
	std::ifstream m_file;
	std::string name = _filePath + _fileName;//_filePath just the file path without filename, _filename = just fie name together = complete path
	m_file.open(((name).c_str())); //opens file for reading

	std::string output;
	std::string line;

	if (m_file.is_open())
	{
		//getline(file, line);
		while (m_file.good())
		{
			//system("CLS");
			getline(m_file, line);

			std::string str(line);//reads in a line from a file into a string
			std::string buf; // Have a buffer string
			std::stringstream ss(str); // Insert the string into a stream 

			std::vector<std::string> tokens; // Create vector to hold our words

			while (ss >> buf)
				tokens.push_back(buf);//pushes words delimited by " " into a vector

			std::string strname = (_filePath + tokens[0]);
			char *cstr = new char[strname.length() + 1];
			strcpy(cstr, strname.c_str());
			const char* pname = cstr;
			Sound* sound = new Sound;
			if (tokens.size() == 5){//if 4 word line, means user didnt specify min max Dist, defualt 0.5 100
				sound->SetSoundName(tokens[0]);
				sound->SetSoundOwner(tokens[1]);
				sound->SetSoundTag(tokens[2]);
				bool is2d = std::stoi(tokens[3], NULL);
				bool isloop = std::stoi(tokens[4], NULL);
				sound->Create(m_SoundSystemPtr, pname, is2d, isloop);
				m_soundList.push_back(sound);
			}

			if (tokens.size() == 7){
				sound->SetSoundName(tokens[0]);
				sound->SetSoundOwner(tokens[1]);
				sound->SetSoundTag(tokens[2]);
				bool is2d = std::stoi(tokens[3], NULL);
				bool isloop = std::stoi(tokens[4], NULL);
				float min = (float)std::stod(tokens[5], NULL);
				float max = (float)std::stod(tokens[6], NULL);
				sound->SetMinMaxDistance(min, max);
				sound->SetOminmax(min, max);
				sound->Create(m_SoundSystemPtr, pname, is2d, isloop);
				m_soundList.push_back(sound);
			}
			m_SoundSystemPtr->Update();
			bool Found = false;
			if (m_uOwners.size() == NULL){//if uOwner vector is empty push back first sound loaded form txt file
				m_uOwners.push_back(sound->GetSoundOwner());
				//std::cout << towner;
			}
			else{
				for (int i = 0; i < m_uOwners.size(); i++){//goes through vector of unique owners and compares already added owners to latest sound loaded from a txt file
					if (m_uOwners[i] == sound->GetSoundOwner())
						Found = true;//if owner in latest sound is in uOwners lists bool a true

				}
				if (Found == false)//if last sounds owner type was not in the uOwner list, added
					m_uOwners.push_back(sound->GetSoundOwner());
			}
			//
		}
	}
	Sort();
	UpdateSoundRNode();
}
SoundManager::~SoundManager(){

}
//control functions
void SoundManager::StopAll(){
	for (int c = 0; c < m_oList.size(); c++){
		m_oList[c]->StopList();
	}
}
void SoundManager::StopOwner(char* _owner, bool _pause){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->StopList();
	}
}
void SoundManager::PauseOwner(char* _owner, bool _pause){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->PauseList(_pause);
	}
}
void SoundManager::MuteOwner(char* _owner, bool _mute){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->MuteList(_mute);
	}
}
void SoundManager::PauseSound(char* _owner, char* _tag, bool _mute){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->PauseSound(_tag, _mute);
	}
}
void SoundManager::MuteSound(char* _owner, char* _tag, bool _mute){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->MuteSound(_tag, _mute);
	}
}
void SoundManager::FindAndPlay(char* _owner, char* _tag){
	Sound* sound = FindSound(_owner, _tag);
	if (sound != NULL){
		sound->Play();
	}
}
void SoundManager::PlayAndPause(char* _owner, char* _tag, bool _pause){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->PlayAndPause(_tag, _pause);
	}
}
void SoundManager::PlayAll(){
	for (int c = 0; c < m_oList.size(); c++){
		m_oList[c]->PlayAll();
	}
}
void SoundManager::PauseAll(bool _pause){
	for (int c = 0; c < m_oList.size(); c++){
		m_oList[c]->PauseAll(_pause);
	}
}
//get functions
Sound* SoundManager::FindSound(char* _owner, char* _tag){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		return list->FindSound(_tag);
	}
	std::cout << "c:SM,f:FS Could not find sound " << _tag << " WRONG/MISSING OWNER/FILE" << std::endl;
	return NULL;

}
SoundList* SoundManager::GetOwnerList(char* _owner){
	for (int c = 0; c < m_oList.size(); c++){
		if (m_oList[c]->GetOwnerName() == _owner)
			return m_oList[c];
	}
	std::cout << "c:SM,f:GOL Could not find Owner list " << _owner << " WRONG/MISSING OWNER/FILE" << std::endl;
	return NULL;
}
bool SoundManager::IsPlaying(char* _owner, char* _tag){
	Sound* s = FindSound(_owner, _tag);
	if (s != NULL){
		return s->GetIsPlaying();
	}
}
//set functions
void SoundManager::SetOwnerPosition(char* _owner, glm::vec3 _pos){
	SoundList* list = GetOwnerList(_owner);
	if (list != NULL){
		list->UpdateListPosition(_pos);
	}
}
void SoundManager::Sort(){ //vector of unique owners
	std::vector<int> nuOwners; // vector of unique sounds for a specific owner

	nuOwners.resize(m_uOwners.size());
	int j = 0;
	while (j < m_uOwners.size()){//Gets unique owner 
		int count = 0;
		for (int i = 0; i < m_soundList.size(); i++){//goes through vector of sounds and campares owners in sound to uOwners
			if (m_uOwners[j] == m_soundList[i]->GetSoundOwner()) // if match is found incremnt count
				count++;
		}

		nuOwners[j] = count;//pushes back number of sounds for a unique owner found in txt file
		j++;//goes to next unique owner and restart loop and utill all are counted
	}

	std::vector<Sound*> tempList;
	int p = 0;
	while (p < m_uOwners.size()){//goes through vector of Unique owners
		for (int i = 0; i < m_soundList.size(); i++){//goes through list of all sounds
			if (m_uOwners[p] == m_soundList[i]->GetSoundOwner()){ //if uOwner tag is the same as in sound
				tempList.push_back(m_soundList[i]);//push sound in tempList
			}
		}
		if (tempList.size() == nuOwners[p]){//checks if all sounds with specif owner were added to tempVector
			//std::cout << "\n List Full";
			SoundList* list = new SoundList;//creates a new OwnerList with owner tag if that group
			list->Init(tempList[0]->GetSoundOwner(), tempList);
			m_oList.push_back(list);//pushes OwnerList into vector of Owners
			tempList.clear(); //clears tempList
		}

		p++;//goes to next unique owner
	}

}
void SoundManager::UpdateSoundRNode(){
	for (int c = 0; c < m_soundList.size(); c++){
		m_soundList[c]->UpdateRNode(m_rnodes);
	}
}
void SoundManager::UpdateSystemRNode(RNode* _node){
	for (int c = 0; c < m_soundList.size(); c++){
		m_soundList[c]->SetSystemRNode(_node);
	}
}
void SoundManager::FastForwardAll(bool _fastForward){
	for (int c = 0; c < m_oList.size(); c++){
		m_oList[c]->FastForwardList(_fastForward);
	}
}
//update functions
void SoundManager::Update(){
	for (int c = 0; c < m_soundList.size(); c++){
		m_soundList[c]->Update();
	}
}
void SoundManager::PrintList(){
	for (int c = 0; c < m_oList.size(); c++){
		m_oList[c]->PrintList();
	}
}
void SoundManager::PrintDistance(){
	for (int c = 0; c < m_oList.size(); c++){
		m_oList[c]->PrintDistance();
	}
}