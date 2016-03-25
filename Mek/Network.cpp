#include "Network.h"
void network::init(std::vector<ReverbNode*> _nodes)
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
			for (int j = 0; j < _nodes[c]->links.size(); j++){
				FMOD_VECTOR oP = _nodes[c]->pos;
				FMOD_VECTOR lP = _nodes[c]->links[j]->pos;
				float distB = sqrt((pow(oP.x - lP.x, 2)) + (pow(oP.y - lP.y, 2)) + (pow(oP.z - lP.z, 2)));
				//std::cout << _nodes[c]->name << " to " << _nodes[c]->links[j]->name << " is: " << distB << std::endl;
				tlink.originNode = _nodes[c];
				tlink.originCity = _nodes[c]->name;
				tlink.linkNode = _nodes[c]->links[j];
				tlink.linkCity = _nodes[c]->links[j]->name;
				tlink.distance = distB;
				tlink.active = true;
				links[linkCount] = tlink;
				linkCount++;
			}
		}
		// initialize the first node
		if (bfnodes.size() != NULL){
			bfnodes.push_back(links[0].originNode);
		}
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
				bfnodes.push_back(links[i].originNode);
				nodeNames[nodeCount] = links[i].originCity;
				// increase the count
				nodeCount++;
			}
		}
	}

void network::menu(){ // function which leads to all other functions based on user input

		//while (true) // runs forever unless user presses "key" to exit
		//{
		//	cout << endl;
		//	cout << "You will know be promted to the menu" << endl;
		//	system("Pause");
		//	system("CLS");
		//
		//	// menu header
		//	cout << "--------------------------------------------------------------------------------";
		//	cout << "\t""\t""\t""\t""\t""  Welcome  \n";
		//	cout << "--------------------------------------------------------------------------------" << endl;
		//
		//	// what the user sees
		//	cout << "(1) Delete a link" << endl;
		//	cout << "(2) Delete a node" << endl;
		//	cout << "(3) Change the weight of a link" << endl;
		//	cout << "(4) Add a link" << endl;
		//	cout << "(5) Add a node" << endl;
		//	cout << "(6) Kruskal Algorithm" << endl;
		//	cout << "(7) Bellman Ford Algorith" << endl;
		//	cout << "(8) Print all Links" << endl;
		//	cout << "(9) Print all nodes" << endl;
		//	cout << "(10) To toggle on/off a link" << endl;
		//	cout << "(11) Exit" << endl << endl;
		//	cout << "Please select an option: ";
		//	cin >> menuChoice; // get the user input
		//
		//	// do one of these depending on input
		//	if (menuChoice == 1)
		//		deleteLink();
		//	else if (menuChoice == 2)
		//		deleteNode();
		//	else if (menuChoice == 3)
		//		weightChange();
		//	else if (menuChoice == 4)
		//		addLink();
		//	else if (menuChoice == 5)
		//		addNode();
		//	else if (menuChoice == 6)
		//		kruskal();
		//	else if (menuChoice == 7)
		//		bellmanFord();
		//	else if (menuChoice == 8)
		//		printLinks();
		//	else if (menuChoice == 9)
		//		printNodes();
		//	else if (menuChoice == 10)
		//		activateDiactivateLink();
		//	else if (menuChoice == 11)
		//		break;
		//	else
		//		cout << "Not a Valid Option" << endl; // error message
		//}
	}

void network::addLink(){
		// intilaize varibales to make sure no duplicates exist
		bool foundOrigin = false;
		bool foundLink = false;
		bool foundAll = false;
		string tempOrigin, tempLink;
		float tempDistance;
		// prompt user, get input, store it in the links
		system("CLS");
		cout << "Here are the current nodes, in the network " << endl;
		cout << "You can add a link that does not have any of the current nodes" << endl;
		printNodes();
		cout << endl;
		cout << "Please enter the city of origin: ";
		cin >> tempOrigin;
		cout << endl;
		cout << "To what city will it be linking to: ";
		cin >> tempLink;
		cout << endl;
		cout << "What is the distance between the two cities: ";
		cin >> tempDistance;
		cout << endl;
		// loop through array to find a duplicate
		for (int l = 0; l < linkCount; l++)
		{
			// if a duplicate is found -> set a foundALL to true and exit the loop
			if ((tempOrigin == links[l].originCity) && (tempLink == links[l].linkCity) && (tempDistance == links[l].distance))
			{
				foundAll = true;
				break;
			}
		}
		// if link exists notify user and exit function
		if (foundAll)
		{
			cout << "Link Already exists" << endl;
			return;
		}
		// if link is new
		else if (!foundAll)
		{
			// assign varibles to links
			links[linkCount].originCity = tempOrigin;
			links[linkCount].linkCity = tempLink;
			links[linkCount].distance = tempDistance;
			links[linkCount].active = true;
			// increment counter
			linkCount++;
		}
		// for loop to add the necessary links to nodes
		for (int i = 0; i < nodeCount; i++)
		{
			// the links origin is in the nodes
			if (nodeNames[i] == links[linkCount - 1].originCity)
			{
				// set to true (will not be added)
				foundOrigin = true;
			}
			// if the links destination is in the nodes
			if (nodeNames[i] == links[linkCount - 1].linkCity)
			{
				// set to true (will not be added)
				foundLink = true;
			}
		}
		// if origin is not in the nodeNames
		if (!foundOrigin)
		{
			// add the new node and increment counter
			nodeNames[nodeCount] = links[linkCount - 1].originCity;
			nodeCount++;
		}
		// if destination is not in the nodeNames
		if (!foundLink)
		{
			// add the new node and increment the counter
			nodeNames[nodeCount] = links[linkCount - 1].linkCity;
			nodeCount++;
		}
	}

void network::addNode(){
		// initliaze found variable and tempNode variable
		bool found = false;
		string tempNode;
		//// prompt user and get input
		//system("CLS");
		//cout << "Here are the current nodes, please do not add a node that already exists" << endl;
		//printNodes();
		//cout << endl;
		//cout << "Please enter the node name: ";
		//cin >> tempNode;
		//cout << endl;
		//// looo through the nodeNames array
		for (int i = 0; i < nodeCount; i++)
		{
			// if node exists
			if (tempNode == nodeNames[i])
				found = true; // set to true (will not be added)
		}
		// if new node
		if (!found)
		{
			// add node and increment counter
			nodeNames[nodeCount] = tempNode;
			nodeCount++;
		}
		// if already exists notify the user
		else if (found)
			std::cout << "node FOund! ";
	}

void network::deleteLink(){
		// declare temporary variables (only used here)
		string tempOriginCity, tempLinkCity;
		int tempDist;
		// prompt for and get the variables
		system("CLS");
		cout << "Here are the current links ";
		printLinks();
		cout << endl;
		cout << "What is the city of the origin link: ";
		cin >> tempOriginCity;
		cout << endl;
		cout << "What is the city of the linked link: ";
		cin >> tempLinkCity;
		cout << endl;
		cout << "What is distance between the cities: ";
		cin >> tempDist;
		cout << endl;
		// loop to find the link
		for (int i = 0; i < linkCount; i++)
		{
			// if found
			if ((links[i].originCity == tempOriginCity) && (links[i].linkCity == tempLinkCity) && (links[i].distance == tempDist))
			{
				linkCount--; // decrease node count
				for (int j = i; j < linkCount; j++) // for loop to shift every link down one
				{
					links[j].originCity = links[j + 1].originCity; // shift origin
					links[j].linkCity = links[j + 1].linkCity; // shift destination
					links[j].distance = links[j + 1].distance; // shift distance
					links[j].active = links[j + 1].active; // shift state (active / not active)
				}
				// clear the last link
				links[linkCount].originCity = " "; // clear origin
				links[linkCount].linkCity = " "; // clear destination
				links[linkCount].distance = 0; // clear distance
				links[linkCount].active = NULL; // clear state
			}
		}

	}

void network::deleteNode(){
		string tempOrigin; // initialize variable (only used here)
		// prompt for and get varible
		system("CLS");
		cout << "Here are the current nodes" << endl;
		printNodes();
		cout << endl;
		cout << "Please enter the name of the city that you wish to delete: ";
		cin >> tempOrigin;
		cout << endl;
		// loop through to find links with said name
		for (int i = 0; i < linkCount; i++)
		{
			// if link mentioned the said node
			if ((links[i].originCity == tempOrigin) || (links[i].linkCity == tempOrigin))
			{
				linkCount--; // decreae link counter
				// delete the node
				for (int j = i; j < linkCount; j++) // for loop to traverse and shift every link down 1
				{
					links[j].originCity = links[j + 1].originCity; // shift origin
					links[j].linkCity = links[j + 1].linkCity; // shift destination 
					links[j].distance = links[j + 1].distance; // shift distance
					links[j].active = links[j + 1].active; // shift state
				}
				// clear the last link
				links[linkCount].originCity = " "; // clear origin
				links[linkCount].linkCity = " "; // clear destination
				links[linkCount].distance = 0; // clear distance
				links[linkCount].active = NULL; // clear state
				i = i - 1; // decrease i to check the new link that was shifted to current position
			}
		}
		// loop through nodeNames array
		for (int j = 0; j < nodeCount; j++)
		{
			// if nodeName is the same (deleting node) variable
			if (nodeNames[j] == tempOrigin)
			{
				// decrease counter
				nodeCount--;
				// loop to shift each nodeName down 1
				for (int k = j; k < nodeCount; k++)
				{
					nodeNames[k] = nodeNames[k + 1];
				}
				// nulify the last nodeName
				nodeNames[nodeCount] = " ";
			}
		}
	}

void network::printLinks()
	{
		system("CLS");
		// loop to output all links
		for (int i = 0; i < linkCount; i++)
		{
			cout << links[i].originCity << " ";
			cout << links[i].linkCity << " ";
			cout << links[i].distance << endl;
		}
		cout << "Total Number is Links is: " << linkCount << endl;
	}

void network::printNodes()
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

void network::weightChange() // Function to change the weight (distance) on a link
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
		if (found == 0) // if the link is not in the network
		{
			cout << "Origin or Destination not found" << endl; // output message to user
		}
	} // return to menu

void network::activateDiactivateLink()
	{
		// declare and initialize variables
		string tempOrigin, tempLink;
		int tempDistance;
		bool found = false;
		// prompt and get input from user
		system("CLS");
		cout << "Here are the current links: ";
		printLinks();
		cout << endl;
		cout << "Please enter the origin of the link: ";
		cin >> tempOrigin;
		cout << endl;
		cout << "Please enter the destination of the link: ";
		cin >> tempLink;
		cout << endl;
		cout << "Please enter the distance of the link: ";
		cin >> tempDistance;
		cout << endl;
		// loop to check if link exists
		for (int i = 0; i < linkCount; i++)
		{
			// if link exists
			if ((tempOrigin == links[i].originCity) && (tempLink == links[i].linkCity) && (tempDistance == links[i].distance))
			{
				// set found to true, toggle state of link
				found = true;
				if (links[i].active = false)
					links[i].active = true;
				else if (links[i].active = true)
					links[i].active = false;
				break; // exit founction
			}
		}
		// if link does not exist in the database -> notify user
		if (!found)
			cout << "No such link exists" << endl;
	}

	// To better understand Kruskal algorithm, we used http://en.wikipedia.org/wiki/Kruskal's_algorithm for the pseudo code, and examples of how it works
	// Also the slides were used (Lecture_08_grpah) to understand the whole idea of a graph as well as how kruskal's algorithm works.
	// And Finally http://www.youtube.com/watch?v=5xosHRdxqHA video was used to further our understand of how kruskal works and helped us write the origin pseudo code
	// no .h files were includes since none were provided in any of the sources.
void network::kruskal()
	{
		// message to show start of function
		cout << "Kruskal Algorithm Here" << endl;
		vector <int> list; // declare list
		kruskalCount = 0; // initialize count of kruskal links
		bool checkOrigin, checkDestination; // declare check variables

		for (int i = 0; i <= linkCount; i++) //puts the current graph into a struct that only this function will use
		{
			stp[i].originCity = links[i].originCity; // copies origin
			stp[i].linkCity = links[i].linkCity; // copies destination
			stp[i].distance = links[i].distance; // copies distance
			stp[i].active = links[i].active; // copies state
		}

		for (int i = 0; i <= linkCount; i++) //will put all the links that are mentioned twice into the inactive state so they won't be used
		{
			for (int n = 0; n <= linkCount; n++) // runs trough the array of links
			{
				// if the links are inversly identical i.e. (A B 100) and (B A 100) are inversly identical
				if ((stp[i].originCity == stp[n].linkCity) && (stp[i].linkCity == stp[n].originCity) && (stp[i].active == 1) && (stp[n].active == 1))
				{
					stp[n].active = 0; // deactivate the link (this link will not be used in the algorithm)		
				}
			}
		}

		for (int i = 0; i < linkCount; i++) //loop to find the lowest distance
		{
			kruskalPosition = 0; // change position
			for (int n = 0; n < linkCount; n++) // loop to traverse the links
			{
				if (stp[n].distance <= stp[kruskalPosition].distance) // if distance is lower or equal at search postion to distance of kruskal position
				{
					if (stp[n].active) // and if the link is active
						kruskalPosition = n; // change postion of kruskal
				}
			}

			for (int n = 0; n <= kruskalCount; n++) //searches for a tree to connect the current link to
			{
				// initialize variables
				checkOrigin = false;
				checkDestination = false;

				if (stp[n].linksList.empty()) // the linksList is empty
				{
					stp[n].linksList.push_back(stp[kruskalPosition].originCity); // adds the origin
					stp[n].linksList.push_back(stp[kruskalPosition].linkCity); // adds the destination
					list.push_back(kruskalPosition); // add kruskal position to the list (at the end)
					kruskalCount++; // increase the counter of kruskal links
					stp[kruskalPosition].active = 0; // deactivate the link so it is not used again
					break; // break out of the loop
				}
				else // if the list is not empty
				{
					for (int d = 0; d < stp[n].linksList.size(); d++) // loop to prevents topology loops
					{
						if (stp[kruskalPosition].originCity == stp[n].linksList[d]) // if origin at kruskal position is in the list at d
						{
							checkOrigin = true; // set checkOrigin to true
						}
						else if (stp[kruskalPosition].linkCity == stp[n].linksList[d]) // if destination at kruskal position is in the list at d
						{
							checkDestination = true; // set checkDestination to true; 
						}
					}
				}
				if ((checkOrigin) && (checkDestination)) // if both origin and destination are checked
				{
					stp[kruskalPosition].active = false; // de-activate the link
					break; // break out of the loop
				}
				else if (checkOrigin) // if origin is checker is true
				{
					stp[n].linksList.push_back(stp[kruskalPosition].linkCity); // add the destination into the links list
					list.push_back(kruskalPosition); // add the kruskal position to the list
					break; // break out of the loop
				}
				else if (checkDestination) // if destination checker is true
				{
					stp[n].linksList.push_back(stp[kruskalPosition].originCity); // add the origin to the links list
					list.push_back(kruskalPosition); // add the kruskal position to the list
					break; // break out of the loop
				}
			}

			for (int n = 0; n < kruskalCount; n++) // loop to combine trees //traverse the kruskal links (STP)
			{
				for (int p = n + 1; p < kruskalCount; p++) // loop to traverse the kruskal links
				{
					for (int m = 0; m < stp[n].linksList.size(); m++) // loop to traverse the linksList at n
					{
						for (int q = 0; q < stp[p].linksList.size(); q++) // loop to traverse the linksList at p
						{
							if (stp[n].linksList[m] == stp[p].linksList[q]) // if the list at m  and q of n and p (respectively) equal
							{
								for (int r = 0; r < stp[p].linksList.size(); r++) // loop to add the distances into the list
								{
									stp[n].linksList.push_back(stp[p].linksList[r]); // actually adding the links
								}
								stp[p].linksList.clear(); // clear the other list (since it was copies into the first list (at p) )
							}
						}
					}
				}
			}

		}
		stpWeight = 0; // used to display total weight of the shortest spanning tree
		for (int i = 0; i < list.size(); i++) // loop to display the links that are in the kruskal spanning tree
		{
			cout << stp[list[i]].originCity << " " << stp[list[i]].linkCity << " " << stp[list[i]].distance << endl;
			stpWeight += stp[list[i]].distance;
		}
		cout << "The total distance in the spanning tree is: " << stpWeight << endl;

		stp[0].linksList.clear(); //clears the linksList to conserve memory
	}

	// To better understand the logic of how bellman ford works, we used http://www.geeksforgeeks.org/dynamic-programming-set-23-bellman-ford-algorithm/
	// no .h file was included from there because the implementation used on the source was changed to work with our implementaion
void network::bellmanFord(std::string _source, std::string _destinaton){
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
	// prompt and get input from user
	string source = _source;
	//system("CLS");
	//cout << "Here are the current nodes" << endl;
	//printNodes();
	//cout << "Enter the location of where you would like to begin: ";
	//cin >> source;
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
	std::vector<PNODE*> path;
	std::vector<string> spath;
	path.empty();
	PNODE node;
	float dist = 0;
	int i = 0;
		while (nodeNames[i] != _destinaton){
			node.name = nodeNames[i];
			node.distance = distances[i];
			spath.push_back(nodeNames[i]);
			//std::cout << bfnodes[0]->name;
			dist += distances[i];
		//	cout << nodeNames[i] << " " << distances[i] << endl;
			next = nodeNames[i];
			i++;
		}
		dist += distances[i];
		distance = dist;

		spath.push_back(nodeNames[i]);
		//std::cout << "dist: " << distance << " next: " << next;
		//std::cout << dist << std::endl;
		SetPath(spath);
}
void network::PrintPnodes(){
	for (int c = 0; c < pnodes.size(); c++){
		std::cout << pnodes[c].name << " distance-> " << pnodes[c].distance << " ";
	}
	std::cout << endl;
}