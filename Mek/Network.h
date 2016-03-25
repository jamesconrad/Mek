#ifndef NETWORK_H
#define NETWORK_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "FSystem.h"
class ReverbNode;
using namespace std;
struct link // contains the general information about a link
{
	ReverbNode* originNode;
	ReverbNode* linkNode;
	string originCity; // origin of the link
	string linkCity; // destination of the link
	float distance; // distance (weight) of the link
	bool active; // state of the link
};

struct kruskalLink // used for kruskal's algorithm
{
	string originCity; // origin of the link
	string linkCity; // destination of the link
	int distance; // distance (weight) of the link 
	bool active; // state of the link
	vector<string> linksList;//adjecency list
};

struct BFlink
{
	string origin; // name of origin
	string destination; //  name of destination
	int source; // integer value of origin
	int dest; // integer value of destination
	int distance; // distance
};
struct PNODE{
	string name;
	float distance;
};
class network
{
public:
	network(){}
	void init(std::vector<ReverbNode*> _nodes);
	void menu();
	void addLink();
	void addNode();
	void deleteLink();
	void deleteNode();
	void printLinks();
	void printNodes();
	void weightChange(); // Function to change the weight (distance) on a link
	void activateDiactivateLink();
	void kruskal();
	void bellmanFord(std::string _source, std::string _destinaton);
	inline std::vector<PNODE> GetPNodes(){ return pnodes; }
	void PrintPnodes();
	std::vector<PNODE> pnodes;
	float distance;
	std::string next;
	inline void SetPath(std::vector<std::string > _path){ path = _path; }
	inline std::vector<std::string > GetPath(){ return path; }
private:
	
	std::vector<ReverbNode*> bfnodes;
	std::vector<ReverbNode*> nodes;
	std::vector<std::string >path;
	// global variables
	int linkCount; // used to count number of links
	link links[200]; // array of links (the graph #1)
	int menuChoice; // variables used to navigate the user
	// link variable
	string nodeNames[200]; // array of strings (used to output nodes)
	int nodeCount; // counter of nodes
	// kruskal variable
	int kruskalCount; // counter for stp
	int kruskalPosition; // current position of stp
	int stpWeight; // used to provide weight of a graph 
	kruskalLink stp[200]; // called it stp because we associate kruskal with stp (networking protocol) (Graph #2)
	// bellman-ford variables
	BFlink rip[200]; // called it rip because we associate bellman-ford with rip (routing protocol) (Graph #3)
	int ripCounter; // counter for rip
};
#endif