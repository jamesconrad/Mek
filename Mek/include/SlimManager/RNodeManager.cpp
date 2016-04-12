#include "RNodeManager.h"

void RNodeManager::Init(SoundSystem* _system, std::string _filePath, std::string _fileName, PresetManager* _presets){
	m_setPath = true;
	m_SoundSystemPtr = _system;
	m_PManagerPtr = _presets;
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
				tokens.push_back(buf);

			RNode* node = new RNode;
			std::string  n = tokens[0];
			bool d = std::stoi(tokens[1], NULL);
			Preset* p = m_PManagerPtr->FindPreset(tokens[2])->GetPreset();
			float x = (float)std::stod(tokens[3], NULL);
			float y = (float)std::stod(tokens[4], NULL);
			float z = (float)std::stod(tokens[5], NULL);
			float min = (float)std::stod(tokens[6], NULL);
			float max = (float)std::stod(tokens[7], NULL);
			node->Init(m_SoundSystemPtr,n,d,p, x, y, z, min, max);
			for (int c = 8; c < tokens.size(); c++){
				node->AddStringLink(tokens[c]);
			}
			m_nodes.push_back(node);
		}
	}
	UpdateRNodes();
	m_networkPtr.Init(m_nodes);
	UpdatePath();
}

//display functions
void RNodeManager::PrintRNodes(){
	for (int c = 0; c < m_nodes.size(); c++){
		m_nodes[c]->PrintNode();
	}
}
//set functions
void RNodeManager::SetRNodePos(std::string _name, glm::vec3 _pos){
	FindNode(_name)->SetRNodePos(_pos);
}
void RNodeManager::SetDestructAll(bool _destruct){
	for(int c = 0; c < m_nodes.size(); c++){
		m_nodes[c]->SetDestruct(_destruct);
	}
}
//get functions
RNode* RNodeManager::FindNode(std::string _nodeName){
	for (int c = 0; c < m_nodes.size(); c++){
		if (m_nodes[c]->GetRNodeName() == _nodeName){
			return m_nodes[c]->GetRNode();
		}
	}
	std::cout << "c:RNM,f:FN Could not find node " << _nodeName << " WRONG/MISSING TAG/NODE Check nodes.txt" << std::endl;
}
void RNodeManager::FindSystemRNode(){
	for (int c = 0; c < m_nodes.size(); c++){
		float distance = m_nodes[c]->GetDistanceToSystem();
		if (distance != 99999){
			float min = m_nodes[c]->GetMaxDistance();
			if (distance <= min){
				//std::cout << m_nodes[c]->GetRNodeName() << " " << m_nodes[c]->GetDistanceToSystem() << std::endl;
			}
		}
	}
}
void RNodeManager::UpdateRNodes(){
	for (int c = 0; c < m_nodes.size(); c++){
		m_nodes[c]->Update();
		FindSystemRNode();
		if (m_setPath){
			m_nodes[c]->SetLinks(m_nodes);
		}
	}
	m_setPath=false;
}
void RNodeManager::UpdatePath(){
	for (int c = 0; c < m_nodes.size(); c++){
		std::string nodeName = m_nodes[c]->GetRNodeName();
		m_nodes[c]->SetPath(m_networkPtr.BellmanFord(nodeName));
	}
}
RNodeManager::~RNodeManager(){
	delete m_SoundSystemPtr;
	m_nodes.clear();
	delete m_PManagerPtr;
	delete m_systemRNode;
}
 