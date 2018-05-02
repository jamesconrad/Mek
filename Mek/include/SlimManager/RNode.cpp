#include "RNode.h"

RNode::RNode(){
	m_freverb = NULL;
	m_path.empty();
	m_links.empty();
}
void RNode::Init(SoundSystem* _fsystem,std::string _name,bool _isDoor, Preset* _preset, float _x, float _y, float _z, float _min, float _max){
	m_SoundSystemPtr = _fsystem;
	m_SoundSystemPtr->GetSystemPtr()->createReverb(&m_freverb);
	m_preset = _preset;
	FMOD_REVERB_PROPERTIES prop = m_preset->GetReverbProperties();
	m_name = _name;
	m_isDoor = _isDoor;
	m_freverb->setProperties(&prop);
	m_reverbPos.x = _x;
	m_reverbPos.y = _y;
	m_reverbPos.z = _z;
	m_minDistance = _min;
	m_maxDistance = _max;
	m_distanceToSystem = 99999;
	m_freverb->set3DAttributes(&m_reverbPos, m_minDistance, m_maxDistance);
	m_freverb->setActive(true);
	m_destruct = false;
}

//update functions
void RNode::Update(){
	UpdateDistanceToSystem();
	UpdateFacingSystem();
}
void RNode::UpdateDistanceToSystem(){
	//std::cout << m_name << " " << m_distanceToSystem << std::endl;
	FMOD_VECTOR sp = GetRNodePos();
	FMOD_VECTOR ssp = m_SoundSystemPtr->GetSystemPosition();
	float x = sp.x - ssp.x;
	float y = sp.y - ssp.y;
	float z = sp.z - ssp.z;
	float distance = sqrt(abs(pow(x, 2) + pow(y, 2) + pow(z, 2)));
	m_distanceToSystem = distance;
}
void RNode::UpdateFacingSystem(){
	glm::vec3 npos = FVECToGLM3(GetRNodePos());
	glm::vec3 spos = FVECToGLM3(m_SoundSystemPtr->GetSystemPosition());
	glm::vec3 sfor = glm::vec3{ -m_SoundSystemPtr->GetSystemFor().x, -m_SoundSystemPtr->GetSystemFor().y, -m_SoundSystemPtr->GetSystemFor().z };

	glm::vec3 dif = npos - spos;
	float d = glm::dot(sfor, dif);
	if (d >= 0){
		m_facingSystem = true;
	}
	else{
		m_facingSystem = false;
	}
}
//set functions
void RNode::SetLinks(std::vector<RNode*> _nodes){
	for (int c = 0; c < m_stringLinks.size(); c++){
		for (int j = 0; j < _nodes.size(); j++){
			if (m_stringLinks[c] == _nodes[j]->GetRNodeName()){
				m_links.push_back(_nodes[j]->GetRNode());
			}
		}
	}
}
RNode* RNode::FindSystemRNode(){
	if (m_distanceToSystem <= m_maxDistance){
		return this;
	}
	else{
		return NULL;
	}
}
//set functions


//get functions
//display functions
void RNode::PrintNode(){
	std::cout << "*************NODE INFO*******************" << std::endl << std::flush;
	if (this != NULL){
		std::cout << m_name << "  iD?: " << m_isDoor << " prN: " << m_preset->GetPresetName() << " dToS: " << m_distanceToSystem << " m: " << m_minDistance << " M: " << m_maxDistance << std::endl << std::flush << std::flush;
		std::cout << "Facing System: " << m_facingSystem << std::endl << std::flush;
		if (m_path.size() != NULL){
			PrintFMODVector(m_reverbPos);
			std::cout << "*************PATH INFO*******************" << std::endl << std::flush;
			for (int c = 0; c < m_path.size(); c++){
				std::cout << m_path[c].name << " " << m_path[c].distance << " " << m_path[c].volume << " -> " << std::flush << std::flush;
			}
		}
		std::cout << std::endl << std::endl << std::flush;
	}
}

RNode::~RNode(){
	if (false){
		m_links.clear();
		m_path.clear();
		m_stringLinks.clear();
		delete m_preset;
		m_freverb->release(); 
		delete m_freverb;
	}
	
}