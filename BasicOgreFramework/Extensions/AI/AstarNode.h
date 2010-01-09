#ifndef ANODE_H
#define ANODE_H

/** @brief Class to symbolize node objects for the A* algorithm
    @author Jonathan Johansson
 */
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
	/**
    Default constructor, sets the variables that always needs to be set.
	*/
	AstarNode();
	/**
	A get method for the neighbors.
	@return The nodes neighbors.
	*/
	vector<AstarNode*> getNeighbors();
	/**
	Adds a weight to the neighbor if walking from the current node.
	@param node - Neighbor that the weight should be added to.
	@param weight - the weight that should be added to the neighbor
	*/
	void addWeightToNeighbor(AstarNode node, int weight);
	/**
	Adds a neighbor to the node
	@param node - Neighbor that is added.
	*/
	void addNeighbor(AstarNode* node);
	/**
	Gets the amount of neighbors the current node has
	@return Amount of neighbors
	*/
	int getAmountOfNeighbors();
	/**
	Operator for comparing to check if two nodes equal each other.
	@param other - Node to compare to.
	@return True if the nodes are the same otherwise false.
	*/
	bool operator==(const AstarNode other) const;
	/**
	Operator for comparing to check if two nodes equal each other.
	@param other - Node to compare to.
	@return True if the nodes are the same otherwise false.
	*/
	bool operator==(const AstarNode* other) const;
	/**
	Operator for comparing to check if a node is larger then the other.
	@param other - Node to compare to.
	@return always returns false.
	*/
	bool operator<(const AstarNode other) const;
	/**
	Returns the x coordinate
	@return x coordinate
	*/
	int getX();
	/**
	Sets the x coordinate
	@param x - the new x coordinate.
	*/
	void setX(int x);
	/**
	Returns the y coordinate
	@return y coordinate
	*/
	int getY();
	/**
	Sets the y coordinate
	@param y - the new y coordinate.
	*/
	void setY(int y);
	/**
	Gets the nodes estimated cost.
	@return The nodes estimated cost.
	*/
	int getEstimatedCost();
	/**
	Sets the estimated cost of the node.
	@param estimatedCost - the nodes new estimated cost.
	*/
	void setEstimatedCost(int estimatedCost);
	/**
	Gets the nodes estimated cost to goal.
	@return The nodes estimated cost to goal.
	*/
	int getEstimatedCostToGoal();
	/**
	Sets the estimated cost to the goal of the node.
	@param estimatedCost - the nodes new estimated cost to goal.
	*/
	void setEstimatedCostToGoal(int estimatedCostToGoal);
	/**
	Sets the cost to travel to the node.
	@param costToGetHere - the nodes new travelling cost.
	*/
	void setCostToGetHere(int costToGetHere);
	/**
	Gets the nodes travelling cost.
	@return returns the travelling cost-
	*/
	int getCostToGetHere();
	/**
	Gets the weight between the current node and the specified neighbor.
	@param node - The neighbor
	@return The weight between the node and the neighbor
	*/
	int getWeight(AstarNode node);
	/**
	Sets the weight between the current node and the specified neighbor.
	@param node - The neighbor
	@param weight - Weight that should be added between the node and the neighbor
	*/
	void setWeight(AstarNode node, int weight);
	/**
	Checks if the node is on the open list
	@return true or false, depending on if the node is on the list or not.
	*/
	bool isOpen();
	/**
	Sets the open value of the node.
	@param open - The value open should be set to
	*/
	void setOpen(bool open);
	/**
	Checks if the node is on the closed list
	@return true or false, depending on if the node is on the list or not.
	*/
	bool isClosed();
	/**
	Sets the closed value of the node.
	@param closed - The value closed should be set to
	*/
	void setClosed(bool closed);
	/**
	Checks if the node is walkable.
	@return true or false, depending on if the node is walkable or not.
	*/
	bool isWalkable();
	/**
	Sets the walkable value of the node.
	@param walkable - The value walkable should be set to
	*/
	void setWalkable(bool walkable);
	/**
	Checks if the node is enclosed.
	@return true or false, depending on if the node is enclosed or not.
	*/
	bool isEnclosed();
	/**
	Sets the enclosed value of the node.
	@param enclosed - The value enclosed should be set to
	*/
	void setEnclosed(bool enclosed);
	/**
	Sets the parent of the node
	@param parent - The nodes new parent.
	*/
	void setParent(AstarNode* parent);
	/**
	Gets the parent of the node.
	@return The parent node.
	*/
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