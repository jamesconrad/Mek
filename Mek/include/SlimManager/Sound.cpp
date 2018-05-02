#include "Sound.h"
//init functions
Sound::Sound(){
	m_SoundPtr = NULL;
	m_ChannelPtr = NULL;
	m_SoundSystemPtr = NULL;
	m_mode = NULL;

	m_soundPos.x = 0.0; m_soundPos.y = 0.0f; m_soundPos.z = 0.0f;
	m_soundVel.x = 0.0f; m_soundVel.y = 0.0f; m_soundVel.z = 0.0f;
	m_activePos = m_soundPos;
	m_activeVel = m_soundVel;
	
	m_minDistance = 0.5f;
	m_maxDistance = 100.0f;
	m_activeMaxDistance = m_maxDistance;
	m_activeMinDistance = m_minDistance;
	m_activeVolume = 1;
	
	m_soundName = "";
	m_soundTag = "";
	m_soundOwner = "";
	m_Node = false;
	bool m_isFastforward = false;
}

bool Sound::Create(SoundSystem* _system,const char* _fileName, bool _is3d, bool _loop)
{
	m_SoundSystemPtr = _system;
	FMOD_MODE mode;
	m_fullPathName = _fileName;
	if (_is3d) mode = FMOD_3D | FMOD_3D_LINEARROLLOFF; else{
		mode = FMOD_2D;
	}
	if (_loop) mode = mode | FMOD_LOOP_NORMAL;
	m_mode = mode;
	m_result = m_SoundSystemPtr->GetSystemPtr()->createSound(m_fullPathName, m_mode, 0, &m_SoundPtr);
	m_SoundPtr->set3DMinMaxDistance(m_activeMinDistance, m_activeMaxDistance); //how far away the sound can be heard

	if (m_result != FMOD_OK) {
		std::cout << "c:S,f:Create Failed to Create Sound " << _fileName << std::endl;
		return false;
	}
	return true;
}
FMOD::Channel* Sound::Play()
{
	FMOD_RESULT result;
	FMOD_MODE   mode;
	
	result = m_SoundSystemPtr->GetSystemPtr()->playSound(FMOD_CHANNEL_FREE, m_SoundPtr, true, &m_ChannelPtr);
	ERRCHECK(result);

	if (result != FMOD_OK)
	{
		std::cout << "c:S,f:Play Failed to Create Sound " << GetFullName() << std::endl;
		return NULL;
	}

	if (m_mode & FMOD_3D)
	{
		result = m_ChannelPtr->set3DAttributes(&m_activePos, &m_activeVel);
		if (result != FMOD_OK){
			std::cout << "c:S,f:Play Failed to set 3D settings " << GetFullName() << std::endl;
		}
		ERRCHECK(result);
	}

	m_ChannelPtr->setPaused(false);

	return m_ChannelPtr;
}

//info functions
void Sound::PrintSoundInformation(){
	std::cout << "*************SOUND INFO*******************" << std::endl;
	if (m_soundName.compare("") && m_soundOwner.compare("") && m_soundTag.compare("")){
		std::cout << "Sound Name: " << m_soundName << std::endl;
		std::cout << "Sound Owner: " << m_soundOwner << std::endl;
		std::cout << "Sound Tag: " << m_soundTag << std::endl;
	}
	else{
		std::cout << "Sound Name: " << m_fullPathName << std::endl;
	}
	std::cout << "Position: "; PrintFMODVector(m_soundPos);
	std::cout << "Ative Position: "; PrintFMODVector(m_activePos);
	std::cout << "Active Volume: " << m_activeVolume << std::endl;
	std::cout << "Min: " << m_minDistance << "Active min: " << m_activeMinDistance << " Max: " <<m_activeMaxDistance << " Active max: " << m_activeMaxDistance << std::endl;
	if (m_rnode != NULL)
	m_rnode->PrintNode();
	std::cout << std::endl;
}
void Sound::PrintDistanceToSystem(){
	std::cout << m_soundName << " " << m_distanceToSystem << std::endl;
}
//set functions
void Sound::SetSoundPosition(glm::vec3 _pos){
	m_soundPos = Glm3ToFVEC(_pos);
}
void Sound::FastForward(bool _fastForward){
	if (_fastForward){
		m_ChannelPtr->setFrequency(22000);
	}
	else{
		m_ChannelPtr->setFrequency(44000);
	}
}
//GetFunctions
void Sound::UpdateRNode(std::vector<RNode*> _rnodes){
	m_rnodes = _rnodes;
}

//update functions
void Sound::UpdateSoundSettings(){
	if (m_rnode){
		if (m_rnode->GetRNodeName() == m_systemRNode->GetRNodeName()){
			std::cout << "System in Sound Node" << " " << m_rnode->GetDistanceToSystem() <<m_rnode->GetRNodeName() << std::endl;
			m_activePos = m_soundPos;
			m_activeVolume = 1;
			m_activeMinDistance = m_minDistance;
			m_activeMaxDistance = m_maxDistance;
			
		}
		if (m_systemRNode->GetDistanceToSystem() < m_systemRNode->GetMinDistance() / 2){
				std::cout << "Looking for closer Node from node! " << m_systemRNode->GetRNodeName() << std::endl;
				if (FindCloserNode() != NULL){
					m_activePos = FindCloserNode()->GetRNodePos();
					m_activeVolume = 1;
					m_activeMinDistance = FindCloserNode()->GetMinDistance();
					m_activeMaxDistance = FindCloserNode()->GetMaxDistance();
					for (int c = 0; c < m_rnode->GetPath().size(); c++){
						if (m_rnode->GetPath()[c].node->GetRNodeName() == FindCloserNode()->GetRNodeName()){
							if (DistanceBetween2FVECTORS(m_rnode->GetRNodePos(), FindCloserNode()->GetRNodePos()) < 11)
								m_activeVolume = 0.9;
							else{
								m_activeVolume = m_rnode->GetPath()[c].volume;
							}
						}
					}
				}
			}
		if(m_rnode->GetRNodeName()!=m_systemRNode->GetRNodeName()){
			std::cout << "System NOT in Sound Node" << " " << m_rnode->GetDistanceToSystem() << m_rnode->GetRNodeName() << std::endl;
			m_activePos = m_systemRNode->GetRNodePos();
			m_activeVolume = 1;
			m_activeMinDistance = m_systemRNode->GetMinDistance();
			m_activeMaxDistance = m_systemRNode->GetMaxDistance();
			for (int c = 0; c < m_rnode->GetPath().size(); c++){
				if (m_rnode->GetPath()[c].node->GetRNodeName() == m_systemRNode->GetRNodeName()){
					if (DistanceBetween2FVECTORS(m_rnode->GetRNodePos(), m_systemRNode->GetRNodePos()) < 11)
						m_activeVolume = 0.9;
					else{
						m_activeVolume = m_rnode->GetPath()[c].volume;
					}
				}
			}
		}
	}
}
void Sound::UpdateDistanceToSystem(){
	FMOD_VECTOR sp = GetSoundPostion();
	FMOD_VECTOR ssp = m_SoundSystemPtr->GetSystemPosition();
	float distance = DistanceBetween2FVECTORS(sp, ssp);
	m_distanceToSystem = distance;
}
void Sound::UpdateRNode(){
	float distance = 999999;
	float ds;
	float m;
	bool found = false;
	RNode* node = new RNode;
	for (int c = 0; c < m_rnodes.size(); c++){
		ds = DistanceBetween2FVECTORS(m_soundPos, m_rnodes[c]->GetRNodePos());
		m = m_rnodes[c]->GetMaxDistance();
		if (ds < distance){
			if (ds < m){
				distance = ds;
				node = m_rnodes[c]->GetRNode();
				found = true;
			}
		}
	}
	if (found){
		m_rnode = node;
	}
	else{
		m_rnode = NULL;
	}
}
RNode* Sound::FindCloserNode(){
	float d = 99999;
	bool Found = false;
	RNode* node = new RNode;
	for (int c = 0; c < m_systemRNode->GetLinks().size(); c++){
		if (m_rnode == m_systemRNode)
			break;
		for (int j = 0; j < m_systemRNode->GetPath().size(); j++){
				if ((m_systemRNode->GetStringLinks()[c] == m_systemRNode->GetPath()[j].node->GetRNodeName())){
					if (DistanceBetween2FVECTORS(m_activePos, m_systemRNode->GetPath()[j].node->GetRNodePos()) < d){
						d = DistanceBetween2FVECTORS(m_activePos, m_systemRNode->GetPath()[j].node->GetRNodePos());
						node = m_systemRNode->GetPath()[j].node->GetRNode();
						Found = true;
					}
			}
		}

	}
	if (Found){
		return node;
		std::cout << "Found next node " << node->GetRNodeName() << std::endl;
	}

}
void Sound::Update(){
	m_ChannelPtr->isPlaying(&m_isPlaying);
	if (m_isPlaying){
		UpdateDistanceToSystem();
		if (m_mode & FMOD_3D){
			UpdateRNode();
			if (m_rnode != NULL) UpdateSoundSettings(); else m_activePos = m_soundPos;

			m_ChannelPtr->setVolume(m_activeVolume);
			if (m_result != FMOD_OK){
				std::cout << "cFS->U Failed to set Active Volume in: " << GetFullName() << " "; ERRCHECK(m_result); std::cout << std::endl;
		
			}
			m_ChannelPtr->set3DMinMaxDistance(m_activeMinDistance, m_activeMaxDistance);
			if (m_result != FMOD_OK){
				std::cout << "cFS->U Failed to set Min Max Distance in: " << GetFullName() << " "; ERRCHECK(m_result); std::cout << std::endl;
		
			}
			m_result = m_ChannelPtr->set3DAttributes(&m_activePos, &m_activeVel);
			if (m_result != FMOD_OK){
				std::cout << "cFS->U Failed to set Channel 3D Attributes in: " << GetFullName() << " "; ERRCHECK(m_result); std::cout << std::endl;
		
			}
		}

	}
}
Sound::~Sound(){	
	if (m_SoundPtr) m_SoundPtr->release();
	delete m_SoundPtr;
	m_SoundPtr = NULL;
}