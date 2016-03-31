#include "SoundNetwork.h"
void Network::init(std::vector<RNode*> _nodes)
	{
		nodes = _nodes;
		// inialize variables
		linkCount = 0;
		nodeCount = 0;
		link tlink;
		//while (!file.eof())
		//{
		//
		//	file >> links[linkCount].originCity;
		//	file >> links[linkCount].linkCity;
		//	file >> links[linkCount].distance;
		//	links[linkCount].active = true;
		//	std::cout << links[linkCount].originCity << std::endl;
		//	linkCount++;
		//}
		// loop to read the file and add information into the links
		for (int c = 0; c < _nodes.size(); c++){
			int size = _nodes[c]->GetStringLinks().size();
			for (int j = 0; j < _nodes[c]->GetLinks().size(); j++){
				FMOD_VECTOR oP = _nodes[c]->GetRNodePos();
				FMOD_VECTOR lP = _nodes[c]->GetLinks()[j]->GetRNodePos();
				float distB = sqrt((pow(oP.x - lP.x, 2)) + (pow(oP.y - lP.y, 2)) + (pow(oP.z - lP.z, 2)));
				//std::cout << _nodes[c]->name << " to " << _nodes[c]->links[j]->name << " is: " << distB << std::endl;
				links[linkCount].originNode = _nodes[c];
				links[linkCount].originCity = _nodes[c]->GetRNodeName();
				links[linkCount].linkNode = _nodes[c]->GetLinks()[j];
				links[linkCount].linkCity = _nodes[c]->GetLinks()[j]->GetRNodeName();
				links[linkCount].distance = distB;
				links[linkCount].active = true;
				linkCount++;
			}
		}
		// initialize the first node
		//if (bfnodes.size() != NULL){
		//	bfnodes.push_back(links[0].originNode);
		//}
		nodeNames[0] = links[0].originCity;
		nodeCount++;
		// load the node array (used for output purposes)
		for (int i = 1; i < linkCount; i++)
		{
			// check if the node reappears
			if (links[i].originCity == nodeNames[nodeCount - 1])
			{
				// don't add the repeating node
				continue;
			}
			// if the node name is new
			else if (links[i].originCity != nodeNames[nodeCount - 1])
			{
				// add the node
				//bfnodes.push_back(links[i].originNode);
				nodeNames[nodeCount] = links[i].originCity;
				// increase the count
				nodeCount++;
			}
		}
	}

void Network::printLinks()
	{		
		// loop to output all links
		for (int i = 0; i < linkCount; i++)
		{
			cout << links[i].originCity << " ";
			cout << links[i].linkCity << " ";
			cout << links[i].distance << endl;
		}
		cout << "Total Number is Links is: " << linkCount << endl;
	}

void Network::printNodes()
	{
		system("CLS");
		// loop to output name of nodes in order they were added to the array
		for (int i = 0; i < nodeCount; i++)
		{
			if (i % 4 == 0)
				cout << endl;
			cout << i + 1 << " " << nodeNames[i] << "\t";
		}
		cout << endl;
		cout << "Total Number of nodes is " << nodeCount << endl;
	}

void Network::weightChange() // Function to change the weight (distance) on a link
	{
		// temporary variables used for input
		string tempOrigin, tempDestination;
		int tempDistance;
		bool found = 0;
		system("CLS");
		cout << "Here are the current links" << endl;
		printLinks();
		cout << "Please enter the origin city of the link: "; // prompt user
		cin >> tempOrigin; // get origin
		cout << "Please enter the destination city of the link: "; // prompt user
		cin >> tempDestination; // get destination (linkCity)
		for (int i = 0; i < linkCount; i++) // loop to find the link specified by the user
		{
			if ((links[i].originCity == tempOrigin) && (links[i].linkCity == tempDestination)) // if the link is found
			{
				found = 1; // set found to true
				cout << "Enter the new distance for the link: "; // prompt user
				cin >> tempDistance; // get distance (weight)
				links[i].distance = tempDistance; // assign new distance (weight)
			}
		}
		if (found == 0) // if the link is not in the Network
		{
			cout << "Origin or Destination not found" << endl; // output message to user
		}
	} // return to menu

std::vector<path> Network::bellmanFord(std::string _source){
	string source = _source;
	// declare and initlaize variabes used in this function
	int maxWeight = 99999;
	ripCounter = 0;
	// loop to copy graph into new one (used only in this function)
	for (int i = 0; i < linkCount; i++)
	{
		// if the link is not active -> ignore it
		if (links[i].active == false)
			continue;
		// if the link is active -> add it and increment counter
		else
		{
			rip[i].origin = links[i].originCity;
			rip[i].destination = links[i].linkCity;
			rip[i].distance = links[i].distance;
			ripCounter++;
		}
	}
	// for loop to map string origin and destination to integer values
	for (int i = 0; i < ripCounter; i++)
	{
		for (int j = 0; j < nodeCount; j++)
		{
			if (rip[i].origin == nodeNames[j])
				rip[i].source = j;
			if (rip[i].destination == nodeNames[j])
				rip[i].dest = j;
		}
	}
	// initialize list used to finialize distances (will be updated throughout bellman ford)
	vector <int> distances;
	distances.resize(nodeCount);
	// step 1. Initialization
	// set everything in infinity except the source (0)
	for (int i = 0; i < nodeCount; i++)
	{
		if (nodeNames[i] == source)
			distances[i] = 0;
		else
			distances[i] = maxWeight;
	}
	// step 2. go through each neighbor to get the best routes o destinations
	// add better routes to the list (also erases old not-best-routes from the list)
	// (relax all links)
	for (int i = 1; i <= nodeCount - 1; i++)
	{
		for (int j = 0; j < ripCounter; j++)
		{
			int u = rip[j].source;
			int v = rip[j].dest;
			int weight = rip[j].distance;
			// if a better distance is found
			//cout << distances[u] + weight << " " << distances[v] << std::endl;
			if (distances[u] + weight < distances[v]){
				//if (distances[u] + weight < distances[v])
				distances[v] = distances[u] + weight; // update the list
			}
		}
	}
	// output the final results
	std::vector<path> nodepath;
	float dist = 0;
	float d = 0;
	float v =0;
	for (int c = 0; c < nodeCount; c++){
		v = 0;
		//cout << nodeNames[c] << " " << distances[c] << endl;
		path path;
		path.name = nodeNames[c];
		path.distance = distances[c];
		d = distances[c];
		if (d == 0)
		 path.volume = 1;
		else{
			//std::cout << d << " ";
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
