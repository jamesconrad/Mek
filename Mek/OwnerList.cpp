#include "OwnerList.h"

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
void OwnerList::UpdateOwnerSoundPos(const glm::vec3* _pos){
	for (int c = 0; c < list.size(); c++){
		list[c]->UpdateSoundPos(*_pos);
	}

}
void OwnerList::GetListSpectrum(char* _tag){
	for (int c = 0; c < list.size(); c++){
		list[c]->GetSpectrum();
	}
}
