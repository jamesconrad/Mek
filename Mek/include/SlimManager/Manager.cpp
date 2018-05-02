#include "Manager.h"

void Manager::Init(){
	m_systemRNode = new RNode;
	m_systemRNode = NULL;
	std::string pathName = "../debug/media/";
	m_SoundSystemPtr.Init();

	//m_PresetManagerPtr.Init(pathName, "presets.txt");
	
	//m_RNManagerPtr.Init(&m_SoundSystemPtr, pathName, "nodes.txt", &m_PresetManagerPtr);

	m_SoundManagerPtr.Init(&m_SoundSystemPtr, m_RNManagerPtr.GetNodes(), pathName, "sounds.txt");

	//m_RNManagerPtr.PrintRNodes();
}

//display fucntions
void Manager::DisplaySystemNode(){
	std::cout << "*************SYSTEM NODE INFO*******************" << std::endl;
	m_systemRNode->PrintNode();
}
void Manager::DisplayAll(){
	
}
void Manager::SetSystemPFU(glm::vec3 _pos, glm::vec3 _for, glm::vec3 _up){
	m_SoundSystemPtr.SetPosition(_pos);
	m_SoundSystemPtr.SetForward(_for);
	m_SoundSystemPtr.SetUp(_up);
}
//update functions
void Manager::UpdateRNodes(){
	m_RNManagerPtr.UpdateRNodes();
	UpdateSystemRNode();
}
void Manager::UpdateSounds(){
	m_SoundManagerPtr.UpdateSystemRNode(m_systemRNode);
	m_SoundManagerPtr.Update();
}
void Manager::UpdateSystemRNode(){
	bool found = false;
	float d = 99999;
	float ds;
	float m;
	RNode* node = new RNode;
	for (int c = 0; c < m_RNManagerPtr.GetNodes().size(); c++){
		ds = m_RNManagerPtr.GetNodes()[c]->GetDistanceToSystem();
		m = m_RNManagerPtr.GetNodes()[c]->GetMinDistance();
		if (m_systemRNode == NULL) m_systemRNode = m_RNManagerPtr.GetNodes()[c];
		if (ds < m){
				node = m_RNManagerPtr.GetNodes()[c]->GetRNode();
				found = true;
		}
		//if(ds < m/2){
		//	for (int c = 0; c < m_systemRNode->GetLinks().size(); c++){
		//		for (int j = 0; j < m_systemRNode->GetPath().size(); j++){
		//			if ((m_systemRNode->GetStringLinks()[c] == m_systemRNode->GetPath()[j].node->GetRNodeName())){
		//				if (DistanceBetween2FVECTORS(m_syst))
		//			}
		//		}
		//	}
		//
		//}
	}
	if (found){
		m_systemRNode = node;
		found = false;
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
		m = m_RNManagerPtr.GetNodes()[c]->GetMinDistance();
		if (m_RNManagerPtr.GetNodes()[c]->GetIsDoor() == true){
			if (ds < distance){
				distance = ds;
				node = m_RNManagerPtr.GetNodes()[c]->GetRNode();
				found = true;
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
	m_SoundSystemPtr.Update();
}
