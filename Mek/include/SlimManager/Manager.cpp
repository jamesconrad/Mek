#include "Manager.h"

void Manager::Init(){
	std::string pathName = "../debug/media/";
	m_SoundSystemPtr.Init();

	m_PresetManagerPtr.Init(pathName, "presets.txt");

	m_RNManagerPtr.Init(&m_SoundSystemPtr, pathName, "nodes.txt", &m_PresetManagerPtr);

	m_SoundManagerPtr.Init(&m_SoundSystemPtr, m_RNManagerPtr.GetNodes(), pathName, "sounds.txt");

	m_RNManagerPtr.PrintRNodes();
}

//display fucntions
void Manager::DisplaySystemNode(){
	std::cout << "*************SYSTEM NODE INFO*******************" << std::endl;
	m_systemRNode->PrintNode();
}
void Manager::DisplayAll(){
	
}
//update functions
void Manager::UpdateRNodes(){
	m_RNManagerPtr.UpdateRNodes();
	UpdateSystemRNode();
}
void Manager::UpdateSounds(){
	m_SoundManagerPtr.Update();
}
void Manager::UpdateSystemRNode(){
	float distance = 999999;
	m_systemRNode = NULL;
	bool found = false;
	float ds;
	float m;
	RNode* node = new RNode;
	for (int c = 0; c < m_RNManagerPtr.GetNodes().size(); c++){
		ds = m_RNManagerPtr.GetNodes()[c]->GetDistanceToSystem();
		m = m_RNManagerPtr.GetNodes()[c]->GetMaxDistance();
		
		if (ds <= m){
			found = true;
			if (ds <= distance){
				distance = ds;
				node = m_RNManagerPtr.GetNodes()[c];
			}
		}
	}
	if (found){
		m_systemRNode = node;
		//std::cout << "In node: " << ds << " " << m << node->GetRNodeName() << std::endl;
	}
	else{
		FindDoor();
	}
}
void Manager::FindDoor(){
	float distance = 999999;
	float ds;
	float m;
	bool found = false;
	RNode* node = new RNode;
	for (int c = 0; c < m_RNManagerPtr.GetNodes().size(); c++){
		ds = m_RNManagerPtr.GetNodes()[c]->GetDistanceToSystem();
		m = m_RNManagerPtr.GetNodes()[c]->GetMaxDistance();
		if (m_RNManagerPtr.GetNodes()[c]->GetIsDoor() == 1){
			if (ds <=distance ){
				found = true;
				distance = ds;
				node = m_RNManagerPtr.GetNodes()[c];
			}
		}
	}
	if (found){
		m_systemRNode = node;
		//std::cout << "In door: " << ds << " " << m << node->GetRNodeName() << std::endl;
	}
}
void Manager::Update(){
	UpdateRNodes();
	UpdateSounds();
	m_SoundManagerPtr.UpdateSystemRNode(m_systemRNode);
	m_SoundSystemPtr.Update();

}
