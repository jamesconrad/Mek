#ifndef RNODEMANAGER_H
#define RNODEMANAGER_H
#include <fstream>
#include <ostream>
#include <sstream>
#include "PresetManager.h"
#include "SoundNetwork.h"
class RNodeManager{
public:
	//init functions
	RNodeManager(){}
	void Init(SoundSystem* _system, std::string _filePath, std::string _fileName, PresetManager*);
	~RNodeManager();
	//display functions
	void PrintRNodes();
	//set functions
	void SetRNodePos(std::string _name ,glm::vec3 _pos);
	void SetDestructAll(bool _destruct);
	//get functtions
	RNode* FindNode(std::string _nodeName);
	void FindSystemRNode();
	inline std::vector<RNode*> GetNodes(){ return m_nodes; }
	//update functions
	void UpdateRNodes();
	void UpdatePath();
private:
	SoundSystem* m_SoundSystemPtr;
	Network m_networkPtr;
	std::vector<RNode*> m_nodes;
	PresetManager* m_PManagerPtr;
	RNode* m_systemRNode;

	bool m_setPath;
};
#endif