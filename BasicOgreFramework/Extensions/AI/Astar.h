#ifndef ASTAR_H
#define ASTAR_H
#include "AstarNode.h"

class Astar{
public:
	static vector<COORD> GenerateAstarPath(AstarNode& start, AstarNode& end, vector<vector<AstarNode*>> graphMap);
	static vector<vector<AstarNode*>> GenerateGraphMap(int size);
	static COORD convertAstarToOgreCoords(COORD coord, int ogreSize, int astarSize);
	static COORD convertOgreToAstarCoords(COORD coord, int ogreSize, int astarSize);
private:
	static int CalculateManhattan(AstarNode node, AstarNode* endNode, vector<vector<AstarNode*>> graphMap);
	static int CalculateCostSoFar(AstarNode node, AstarNode* startNode);
	static void AddNeighborsToOpen(AstarNode* node, AstarNode* startNode, AstarNode* endNode,vector<AstarNode*>* open, vector<vector<AstarNode*>> graphMap);
};
#endif