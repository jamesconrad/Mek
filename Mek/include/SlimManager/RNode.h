#ifndef RNODE_H
#define RNODE_H
//#include "PresetManager.h"
#include "SoundSystem.h"
#include "PresetManager.h"
class RNode;
struct path{
	RNode* node;
	std::string name;
	float distance;
	float volume;
};
class RNode{
public:
	//init functions
	RNode();
	void Init(SoundSystem* _fsystem, std::string _name, bool _isDoor, Preset* _preset, float _x, float _y, float _z, float _min, float _max);
	~RNode();

	//update functions
	void Update();
	void UpdateDistanceToSystem();
	void UpdateFacingSystem();
	
	//set functions
	inline void SetRNodePos(glm::vec3 _pos){m_reverbPos = Glm3ToFVEC(_pos);}
	inline void SetActive(bool _active){ m_freverb->setActive(_active); }
	inline void SetRNodeName(std::string _name){ m_name = _name; }
	inline void SetIsDoor(bool _isDoor){ m_isDoor = _isDoor; }
	inline void SetMinDistance(float _min){ m_minDistance = _min; }
	inline void SetMaxDistance(float _max){ m_maxDistance = _max; }
	inline void SetMinMaxDistance(float _min, float _max){ SetMinDistance(_min); SetMaxDistance(_max); }
	inline void SetPath(std::vector<path> _path){ m_path = _path; }
	inline void AddLink(RNode* _link){ m_links.push_back(_link); }
	inline void AddStringLink(std::string _stringLink){ m_stringLinks.push_back(_stringLink); }
	inline void SetDestruct(bool _destruct){ m_destruct = _destruct; }
	void SetLinks(std::vector<RNode*> _nodes);

	//get functions
	inline RNode* GetRNode(){ return this; }
	inline std::vector<RNode*> GetLinks(){ return m_links; }
	inline std::vector<std::string> GetStringLinks(){ return m_stringLinks; }
	inline std::string GetRNodeName(){ return m_name; }
	inline bool GetIsDoor(){ return m_isDoor; }
	inline float GetMinDistance(){ return m_minDistance; }
	inline float GetMaxDistance(){ return m_maxDistance; }
	inline float GetDistanceToSystem () const { return m_distanceToSystem; }
	inline FMOD_VECTOR& GetRNodePos(){ return m_reverbPos; }
	inline std::vector<path>& GetPath(){ return m_path; }
	inline bool GetFacingSystem(){ return m_facingSystem; }
	RNode* FindSystemRNode();
	//display functions
	void PrintNode();
private:
	SoundSystem* m_SoundSystemPtr;
	std::vector<RNode*> m_links;
	std::vector<path> m_path;
	std::vector<std::string> m_stringLinks;
	FMOD::Reverb *m_freverb;
	Preset* m_preset;
	FMOD_VECTOR m_reverbPos;
	std::string m_name = "";
	bool m_isDoor;
	bool m_destruct;
	bool m_facingSystem;
	float m_minDistance;
	float m_maxDistance;
	float m_distanceToSystem;
};

#endif 