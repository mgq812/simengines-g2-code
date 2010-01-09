#ifndef ASTAR_H
#define ASTAR_H
#include "AstarNode.h"

class Astar{
public:
	/**
	A method that generates and returns an A* path between two nodes.
	@param start - The start node.
	@param end - The end node.
	@param graphMap - the graph map to traverse over.
	@return A vector of coordinates to show which path that is the shortest between the start and end node.
	*/
	static vector<COORD> GenerateAstarPath(AstarNode& start, AstarNode& end, vector<vector<AstarNode*>> graphMap);
	/**
	A method for generating an NxN sized graphmap where N is the input size.
	@param size - The size you want the graph map in.
	@return The generated graph map.
	*/
	static vector<vector<AstarNode*>> GenerateGraphMap(int size);
	/**
	Converts A* coords to Ogre coords.
	@param coord - Coord you want to convert.
	@param ogreSize - the size of the A* nodes in ogre coords.
	@param astarSize - the size of the graph map used by the A* algorithm.
	@return the converted coordinate.
	*/
	static COORD convertAstarToOgreCoords(COORD coord, int ogreSize, int astarSize);
	/**
	Converts Ogre coords to A* coords.
	@param coord - Coord you want to convert.
	@param ogreSize - the size of the A* nodes in ogre coords.
	@param astarSize - the size of the graph map used by the A* algorithm.
	@return the converted coordinate.
	*/
	static COORD convertOgreToAstarCoords(COORD coord, int ogreSize, int astarSize);
private:
	/**
	Calculates the manhattan value between the current node and the end node.
	@param node - the current node.
	@param endNode - the end node.
	@param graphMap - the graph map the method should traverse over.
	@return The manhattan method.
	*/
	static int CalculateManhattan(AstarNode node, AstarNode* endNode, vector<vector<AstarNode*>> graphMap);
	/**
	Calculates the cost to travel to the current node from the start position
	@param node - the current node.
	@param startNode - the start node.
	@return The travelling cost between the start node and the current node.
	*/
	static int CalculateCostSoFar(AstarNode node, AstarNode* startNode);
	/**
	Tries to add all of the current nodes neighbors to the open list.
	@param node - The current node
	@param startNode - the start node
	@param endNode - the end node
	@param open - the open list
	@param graphMap - the graph map to traverse over
	*/
	static void AddNeighborsToOpen(AstarNode* node, AstarNode* startNode, AstarNode* endNode,vector<AstarNode*>* open, vector<vector<AstarNode*>> graphMap);
};
#endif