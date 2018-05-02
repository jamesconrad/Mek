#ifndef SOUNDNETWORK_H
#define SOUNDNETWORK_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "SoundSystem.h"
#include "RNode.h"
class ReverbNode;
struct Link{
	RNode* n_onode;
	RNode* n_lnode;
	std::string m_onode;
	std::string m_lnode;
	float m_distance;
	bool m_active; // state of the link
};
struct BFlink{

	std::string m_origin;
	std::string m_destination;
	int m_source;
	int m_dest;
	int m_distance;
};
class Network{
public:
	Network(){}
	void Init(std::vector<RNode*> _nodes);
	void PrintLinks();
	void PrintNodes();
	std::vector<path> BellmanFord(std::string _source);
private:
	std::vector<RNode*> m_nodes;
	int m_linkCount;
	Link m_links[10];

	std::string m_nodeNames[200];
	int m_nodeCount;
	BFlink m_rip[50]; 
	int m_ripCounter;
};
#endif