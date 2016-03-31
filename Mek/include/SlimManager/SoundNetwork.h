#ifndef SOUNDNETWORK_H
#define SOUNDNETWORK_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "SoundSystem.h"
#include "RNode.h"
class ReverbNode;
using namespace std;
struct link // contains the general information about a link
{
	RNode* originNode;
	RNode* linkNode;
	string originCity; // origin of the link
	string linkCity; // destination of the link
	float distance; // distance (weight) of the link
	bool active; // state of the link
};
struct BFlink
{

	string origin; // name of origin
	string destination; //  name of destination
	int source; // integer value of origin
	int dest; // integer value of destination
	int distance; // distance
};
class Network
{
public:
	Network(){}
	void init(std::vector<RNode*> _nodes);
	void printLinks();
	void printNodes();
	void weightChange();
	std::vector<path> bellmanFord(std::string _source);
private:
	std::vector<RNode*> nodes;
	// global variables
	int linkCount; // used to count number of links
	link links[200]; // array of links (the graph #1)
	int menuChoice; // variables used to navigate the user
	// link variable
	string nodeNames[200];
	int nodeCount;
	BFlink rip[200]; 
	int ripCounter;
};
#endif