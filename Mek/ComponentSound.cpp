#include "ComponentSound.h">
void ComponentSound::SetList(OwnerList* _list){
	list = new OwnerList(_list->owner);
	list = _list;
}
bool ComponentSound::FindOwner(char* _owner){
	int index = Program::getInstance().getManager()->FindOwner(_owner);
	if (index != -1){
		//Program::getInstance().getManager()->FindAndPlay("Background", "one");
		SetList(Program::getInstance().getManager()->GetOwnerList(_owner));
		return true;
	}
	else{
		std::cout << "Owner not found " << _owner << std::endl;
		return false;
	}
}
void ComponentSound::update(){
	list->UpdateOwnerListPosition(getPos());
}