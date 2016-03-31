#include "SoundList.h"
//init fuctions
void SoundList::Init(std::string _owner, std::vector<Sound*> _list){
	m_ownerName = _owner;
	m_list = _list;
}
//control functions
void SoundList::StopList(){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->StopSound();
	}
}
void SoundList::PauseList(bool _pause){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->PauseSound(_pause);
	}
}
void SoundList::PauseSound(char* _tag, bool _pause){
	Sound* s = FindSound(_tag);
	if (s != NULL){
		s->PauseSound(_pause);
	}
}
void SoundList::MuteList(bool _mute){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->MuteSound(_mute);
	}
}
void SoundList::MuteSound(char* _tag, bool _pause){
	Sound* s = FindSound(_tag);
	if (s != NULL){
		s->MuteSound(_pause);
	}
}
void SoundList::FindAndPlay(char* _tag){
	Sound* s = FindSound(_tag);
	if (s != NULL){
		s->Play();
	}
}
void SoundList::PlayAndPause(char* _tag, bool _pause){
	Sound* s = FindSound(_tag);
	if (s != NULL){
		s->Play();
		s->PauseSound(_pause);
	}
}
void SoundList::PlayAll(){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->Play();
	}
}
void SoundList::PauseAll(bool _pause){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->PauseSound(_pause);
	}
}
//set functions
void SoundList::UpdateListPosition(glm::vec3 _pos){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->SetSoundPosition(_pos);
	}
}
void SoundList::FastForwardList(bool _fastForward){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->FastForward(_fastForward);
	}
}
//get functions

Sound* SoundList::FindSound(char* _tag){
	for (int c = 0; c < m_list.size(); c++){
		if (m_list[c]->GetSoundTag() == _tag)
			return m_list[c];
	}
	std::cout << "c:SL,f:FS Could not find sound " << _tag << " Check TAG/FILE WRONG/MISSING" << std::endl;
	return NULL;
}
void SoundList::PrintList(){
	for (int c = 0; c < m_list.size(); c++){
		std::cout << m_list[c]->GetSoundName() << " " << m_list[c]->GetSoundOwner() << " " << m_list[c]->GetSoundTag()<< " " << m_list[c]->GetDistanceToSystem() << std::endl;
	}
}
void SoundList::PrintDistance(){
	for (int c = 0; c < m_list.size(); c++){
		m_list[c]->PrintDistanceToSystem();
	}
}