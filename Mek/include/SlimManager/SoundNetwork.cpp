#include "SoundNetwork.h"
void Network::Init(std::vector<RNode*> _nodes){
		m_nodes = _nodes;
		m_linkCount = 0;
		m_nodeCount = 0;
		Link tlink;

		for (int c = 0; c < _nodes.size(); c++){
			int size = _nodes[c]->GetStringLinks().size();
			for (int j = 0; j < _nodes[c]->GetLinks().size(); j++){
				FMOD_VECTOR oP = _nodes[c]->GetRNodePos();
				FMOD_VECTOR lP = _nodes[c]->GetLinks()[j]->GetRNodePos();
				float distB = sqrt((pow(oP.x - lP.x, 2)) + (pow(oP.y - lP.y, 2)) + (pow(oP.z - lP.z, 2)));
				m_links[m_linkCount].n_onode = _nodes[c];
				m_links[m_linkCount].m_onode = _nodes[c]->GetRNodeName();
				m_links[m_linkCount].n_lnode = _nodes[c]->GetLinks()[j];
				m_links[m_linkCount].m_lnode = _nodes[c]->GetLinks()[j]->GetRNodeName();
				m_links[m_linkCount].m_distance = distB;
				m_links[m_linkCount].m_active = true;
				m_linkCount++;
			}
		}
		m_nodeNames[0] = m_links[0].m_onode;
		m_nodeCount++;
		for (int i = 1; i < m_linkCount; i++){
			if (m_links[i].m_onode== m_nodeNames[m_nodeCount - 1]){
				continue;
			}
			else if (m_links[i].m_onode != m_nodeNames[m_nodeCount - 1]){
				m_nodeNames[m_nodeCount] = m_links[i].m_onode;
				m_nodeCount++;
			}
		}
}

void Network::PrintLinks(){
		for (int c = 0; c < m_linkCount; c++){
			std::cout << m_links[c].m_onode << " ";
			std::cout << m_links[c].m_lnode << " ";
			std::cout << m_links[c].m_distance << std::endl;
		}
		std::cout << "Total Number is Links is: " << m_linkCount << std::endl;
	}

void Network::PrintNodes(){
		for (int c = 0; c < m_nodeCount; c++)
		{
			if (c % 4 == 0)
				std::cout << std::endl;
			std::cout << c + 1 << " " << m_nodeNames[c] << "\t";
		}
		std::cout << std::endl;
		std::cout << "Total Number of nodes is " << m_nodeCount << std::endl;
}

std::vector<path> Network::BellmanFord(std::string _source){
	std::string source = _source;
	int maxWeight = 99999;
	m_ripCounter = 0;
	for (int i = 0; i < m_linkCount; i++){
		if (m_links[i].m_active == false)
			continue;
		else{
			m_rip[i].m_origin = m_links[i].m_onode;
			m_rip[i].m_destination = m_links[i].m_lnode;
			m_rip[i].m_distance = m_links[i].m_distance;
			m_ripCounter++;
		}
	}
	for (int i = 0; i < m_ripCounter; i++){
		for (int j = 0; j < m_nodeCount; j++){
			if (m_rip[i].m_origin == m_nodeNames[j])
				m_rip[i].m_source = j;
			if (m_rip[i].m_destination == m_nodeNames[j])
				m_rip[i].m_dest = j;
		}
	}
	std::vector <int> distances;
	distances.resize(m_nodeCount);
	// Step 1: initialize graph
	for (int i = 0; i < m_nodeCount; i++)
	{
		if (m_nodeNames[i] == source)
			distances[i] = 0;
		else
			distances[i] = maxWeight;
	}
	// Step 2: relax edges repeatedly
	for (int i = 1; i <= m_nodeCount - 1; i++)
	{
		for (int j = 0; j < m_ripCounter; j++)
		{
			int u = m_rip[j].m_source;
			int v = m_rip[j].m_dest;
			int weight = m_rip[j].m_distance;
			// Step 3: check for negative-weight cycles
			if (distances[u] + weight < distances[v]){
				distances[v] = distances[u] + weight;
			}
		}
	}
	std::vector<path> nodepath;
	float dist = 0;
	float d = 0;
	float v =0;
	for (int c = 0; c < m_nodeCount; c++){
		v = 0;
		path path;
		path.name = m_nodeNames[c];
		path.distance = distances[c];
		for (int c = 0; c < m_nodes.size(); c++){
			if (path.name == m_nodes[c]->GetRNodeName()){
				path.node = m_nodes[c]->GetRNode();
				break;
			}
		}
		d = distances[c];
		if (d == 0)
		 path.volume = 1;
		else{
			if (d > 5){
				v = 0.9;
			}
			v = (1 / d) * 10;
			if (v > 1){
				v = 1;
			}
			if (v < 0){
				v = 0;
			}
			path.volume = v;
		}
		//std::cout << v << std::endl;
		nodepath.push_back(path);
	}
	//std::cout << std::endl;
	return nodepath;
}
