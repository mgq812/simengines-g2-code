#ifndef ANODE_H
#define ANODE_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h> 
#include <map>

using namespace std;
class AstarNode
{	
public:
	AstarNode();
	vector<AstarNode*> getNeighbors();
	void addWeightToNeighbor(AstarNode node, int weight);
	void addNeighbor(AstarNode*);
	int getAmountOfNeighbors();
	bool operator==(const AstarNode other) const;
	bool operator==(const AstarNode* other) const;
	bool operator<(const AstarNode other) const;
	int getX();
	void setX(int x);
	int getY();
	void setY(int y);
	int getEstimatedCost();
	void setEstimatedCost(int estimatedCost);
	int getEstimatedCostToGoal();
	void setEstimatedCostToGoal(int estimatedCostToGoal);
	int getWeight(AstarNode);
	void setCostToGetHere(int costToGetHere);
	int getCostToGetHere();
	void setWeight(AstarNode, int weight);
	bool isOpen();
	void setOpen(bool open);
	bool isClosed();
	void setClosed(bool closed);
	bool isWalkable();
	void setWalkable(bool walkable);
	bool isEnclosed();
	void setEnclosed(bool enclosed);
	void setParent(AstarNode* parent);
	AstarNode* getParent();
private:
	vector<AstarNode*> neighbors;
	map<AstarNode, int> weights;
	AstarNode* parent;
	int xpos;
	int ypos;
	bool walkable;
	bool enclosed;
	bool closed;
	bool open;
	int weight;
	int estimatedCost;
	int estimatedCostToGoal;
	int costToGetHere;
};
#endif