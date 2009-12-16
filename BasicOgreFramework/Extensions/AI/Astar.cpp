                                                                     
                                                                     
                                                                     
                                             
#include "Astar.h"
vector<vector<AstarNode*>> Astar::GenerateGraphMap(int size){
	//Creating x,y variables and a vector that contains vectors with nodes to represent a 2d field.
	int x = 0;
	int y = 0;
	vector<vector<AstarNode*>> graphMap;
	//Filling the first vector with vectors and all other vectors with nodes
	for(x = 0; x<size;x++){
		graphMap.push_back(vector<AstarNode*>());
		for(y = 0; y<size;y++){
			AstarNode *temp = new AstarNode();
			graphMap[x].push_back(temp);	
			//Setting the x,y values of the node and also makes it walkable.

			(graphMap[x][y])->setX(x);
			(graphMap[x][y])->setY(y);
		}
	}
	//Looping through all nodes in the graph and linking each node together with it neighbors and added in weights between them.
	for(x = 0; x<size;x++){
		for(y = 0; y<size;y++){
			if(x-1 >= 0){
				if(y-1 >= 0){
					(graphMap[x][y])->addNeighbor(graphMap[x-1][y-1]);
					(graphMap[x][y])->addWeightToNeighbor(*graphMap[x-1][y-1],1);
				}
				(graphMap[x][y])->addNeighbor(graphMap[x-1][y]);
				(graphMap[x][y])->addWeightToNeighbor(*graphMap[x-1][y],1);
				if(y+1 < size){
					(graphMap[x][y])->addNeighbor(graphMap[x-1][y+1]);
					(graphMap[x][y])->addWeightToNeighbor(*graphMap[x-1][y+1], 1);
				}
			}
			if(x+1 < size){
				if(y-1 >= 0){
					(graphMap[x][y])->addNeighbor(graphMap[x+1][y-1]);
					(graphMap[x][y])->addWeightToNeighbor(*graphMap[x+1][y-1],1);
				}
				(graphMap[x][y])->addNeighbor(graphMap[x+1][y]);
				(graphMap[x][y])->addWeightToNeighbor(*graphMap[x+1][y],1);
				if(y+1 < size){
					(graphMap[x][y])->addNeighbor(graphMap[x+1][y+1]);
					(graphMap[x][y])->addWeightToNeighbor(*graphMap[x+1][y+1],1);
				}
			}
			if(y-1 >= 0){
				(graphMap[x][y])->addNeighbor(graphMap[x][y-1]);
				(graphMap[x][y])->addWeightToNeighbor(*graphMap[x][y-1],1);
			}
			if(y+1 < size){
				(graphMap[x][y])->addNeighbor(graphMap[x][y+1]);
				(graphMap[x][y])->addWeightToNeighbor(*graphMap[x][y+1], 1);
			}
		}
	}
	return graphMap;
}

COORD Astar::convertAstarToOgreCoords(COORD coord, int ogreSize, int astarSize){
	COORD ogreCoords;
	ogreCoords.X = -ogreSize/2 + 2*coord.X*ogreSize/astarSize;
	ogreCoords.Y = ogreSize/2 - 2*coord.Y*ogreSize/astarSize;
	return ogreCoords;
}

int Astar::CalculateManhattan(AstarNode node,AstarNode* endNode, vector<vector<AstarNode*>> graphMap){
	int cost = 0;
	//Loop while the current node is not the end node we want to find.
	while(!(node == endNode)){
		//Calculating if we have the longest way to go until we reach the end node in the x or y direction.
		//When we found out with direction that has the longest way still to go, we take one step in that direction,
		//and exchanges the current node with the new one, and add its cost to the manhattan cost.
		if(abs(node.getX() - endNode->getX()) > abs(node.getY() - endNode->getY())){
			if(node.getX() > endNode->getX()){
				cost += node.getWeight(*graphMap[node.getX()-1][node.getY()]);
				node = *graphMap[node.getX()-1][node.getY()];
			}
			else{
				cost += node.getWeight(*graphMap[node.getX()+1][node.getY()]);
				node = *graphMap[node.getX()+1][node.getY()];
			}
		}
		else{
			if(node.getY() > endNode->getY()){
				cost += node.getWeight(*graphMap[node.getX()][node.getY()-1]);
				node = *graphMap[node.getX()][node.getY()-1];
			}
			else{
				cost += node.getWeight(*graphMap[node.getX()][node.getY()+1]);
				node = *graphMap[node.getX()][node.getY()+1];
			}
		}
	}
	return cost;
}

int Astar::CalculateCostSoFar(AstarNode node, AstarNode* startNode){
	int cost = 0;
	//Takes the current node, and exchanges it for its parent node while it parent node is not the start node,
	//while adding up the cost between all the nodes on the way.
	while(!(node.getParent() == startNode)){
		cost += node.getParent()->getWeight(node);
		node = *node.getParent();
	}
	cost += node.getParent()->getWeight(node);
	return cost;
}

void Astar::AddNeighborsToOpen(AstarNode* node, AstarNode* startNode, AstarNode* endNode,vector<AstarNode*>* open, vector<vector<AstarNode*>> graphMap){
	//Sets so the current node is closed.
	node->setClosed(true);
	//Creates a tempory vector that contains the current node's neighbors
	vector<AstarNode*> neighbors = node->getNeighbors();
	//Looping through all neighbors
	for(int i = 0;i < node->getAmountOfNeighbors();i++){
		//If the neighbor is not on the open or close list and is walkbable add it to the open list, and set the current node as its parent.
		//Also calculats the estimated cost for the node to reach the end node.
		if(neighbors[i]->isWalkable() && !neighbors[i]->isClosed() && !neighbors[i]->isOpen()){
			neighbors[i]->setParent(node);
			neighbors[i]->setEstimatedCostToGoal(Astar::CalculateManhattan(*neighbors[i],endNode, graphMap));
			neighbors[i]->setCostToGetHere(Astar::CalculateCostSoFar(*neighbors[i],startNode));
			neighbors[i]->setEstimatedCost(neighbors[i]->getCostToGetHere() + neighbors[i]->getEstimatedCostToGoal());
			open->push_back(neighbors[i]);
			neighbors[i]->setOpen(true);
		}
		//Check if a node that is already added to a list gets a better estimated cost value with the current node as its parent compared to its current parent.
		else if(neighbors[i]->isWalkable() && neighbors[i]->getParent() != NULL){
			if(node->getCostToGetHere() < neighbors[i]->getParent()->getCostToGetHere()){
				neighbors[i]->setParent(node);
				neighbors[i]->setEstimatedCostToGoal(Astar::CalculateManhattan(*neighbors[i],endNode, graphMap));
				neighbors[i]->setCostToGetHere(Astar::CalculateCostSoFar(*neighbors[i],startNode));
				neighbors[i]->setEstimatedCost(neighbors[i]->getCostToGetHere() + neighbors[i]->getEstimatedCostToGoal());
			}
		}
	}
}
vector<COORD> Astar::GenerateAstarPath(AstarNode& start, AstarNode& end, vector<vector<AstarNode*>> graphMap){
	//Creating pointers to the start and end node.
	AstarNode* startNode = &start;
	AstarNode* endNode = &end;
	//if either the start node or the end node is in a enclosed area but not the other, return an empty vector
	if(startNode->isEnclosed() != endNode->isEnclosed()){
		return *(new vector<COORD>());
	}
	//Creating one vector containing the final path and one vector containing the nodes that're on the open list
	vector<COORD> path;
	vector<AstarNode*> open;
	//Setting the boolean value so that the final node is not yet found.
	bool FoundEndNode = false;
	//Adds the start nodes neighbors to the open list
	Astar::AddNeighborsToOpen(startNode, startNode, endNode, &open, graphMap);
	//Checks if any nodes got added to the open list otherwise it's not necessary to continue
	if(open.size() != 0){
		AstarNode* lowest;
		int lowestAt = 0;
		//Looping until we found the end node or until we run out of nodes in the open list.
		while(!FoundEndNode && open.size() != 0){
			lowest = open[0];
			lowestAt = 0;
			//finds the node on the open list that has the lowest estimated cost to reach the end node.
			for(unsigned int i = 1;i < open.size();i++){
				if(lowest->getEstimatedCost() > open[i]->getEstimatedCost()){
					lowest = open[i];
					lowestAt = i;
				}
			}
			//Checks if the node with the lowest estimated cost is the end node itself.
			if(lowest == endNode){
				FoundEndNode = true;
			}
			//otherwise adds its neighbors to the open list.
			else{
				Astar::AddNeighborsToOpen(lowest,startNode,endNode,&open, graphMap);
			}
			//Deletes the current node from the open list.
			open.erase(open.begin()+lowestAt);	
		}
		AstarNode node = *endNode;
		COORD coord;
		//Takes the end node, and exchanges it for its parent node while it parent node is not the start node, while saving the coordinates for each node in a vector.
		//The cost you get by adding up all the nodes here will be the final cost to travel between the start node and the end node.
		while(!(node == startNode)){
			if(FoundEndNode){
				coord.X = node.getX();
				coord.Y = node.getY();
				path.push_back(coord);
				node = *node.getParent();
			}
			else{
				break;
			}
		}
	}
	//Reversing the path vector as you want to go from the start node to the end note and not vice versa.
	vector<COORD> reversedPath( path.rbegin(), path.rend() );
	return reversedPath;
}