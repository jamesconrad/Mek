#ifndef COMPOINENTSOUND_H
#define COMPONENTSOUND_H
#include "GameObject.h"
#include "SoundManager.h"
#include "Program.h"
class ComponentSound : public Component{
public:
	ComponentSound(){}
	inline OwnerList* getList(){ return list; }
	void SetList(OwnerList* _list);
	bool FindOwner(char* _owner);
	void update();
	OwnerList* list;
	
};
#endif