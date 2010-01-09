#include "AstarNode.h"
AstarNode::AstarNode(){
	//Sets default values to the variables that always needs to be set.
	parent = NULL;
	closed = false;
	open = false;
	walkable = true;
	enclosed = false;
}
void AstarNode::setParent(AstarNode* parent){
	this->parent = parent;
}
AstarNode* AstarNode::getParent(){
	return parent;
}
bool AstarNode::isOpen(){
	return open;
}
void AstarNode::setOpen(bool open){
	this->open = open;
}
bool AstarNode::isClosed(){
	return closed;
}
void AstarNode::setClosed(bool closed){
	this->closed = closed;
}
bool AstarNode::isWalkable(){
	return walkable;
}
void AstarNode::setWalkable(bool walkable){
	this->walkable = walkable;
}
bool AstarNode::isEnclosed(){
	return walkable;
}
void AstarNode::setEnclosed(bool enclosed){
	this->enclosed = enclosed;
}
int AstarNode::getX(){
	return xpos;
}
void AstarNode::setX(int x){
	xpos = x;
}
int AstarNode::getY(){
	return ypos;
}
void AstarNode::setY(int y){
	ypos = y;
}
int AstarNode::getEstimatedCost(){
	return estimatedCost;
}
void AstarNode::setEstimatedCost(int estimatedCost){
	this->estimatedCost = estimatedCost;
}
int AstarNode::getEstimatedCostToGoal(){
	return estimatedCostToGoal;
}
void AstarNode::setEstimatedCostToGoal(int estimatedCostToGoal){
	this->estimatedCostToGoal = estimatedCostToGoal;
}
int AstarNode::getWeight(AstarNode node){
	return weights[node];
}
void AstarNode::setCostToGetHere(int costToGetHere){
	this->costToGetHere = costToGetHere;
}
int AstarNode::getCostToGetHere(){
	return costToGetHere;
}
void AstarNode::setWeight(AstarNode node,int weight){
	weights[node] = weight;
}
void AstarNode::addNeighbor(AstarNode* node){
	this->neighbors.push_back(node);
}
int AstarNode::getAmountOfNeighbors(){
	return neighbors.size();
}
void AstarNode::addWeightToNeighbor(AstarNode node, int weight){
	this->weights[node] = weight;
}
vector<AstarNode*> AstarNode::getNeighbors(){
	return neighbors;
}
bool AstarNode::operator<(const AstarNode other) const {
	//Always returns false so that the map never tries to sort itself. as we don't need a sorted map for nodes.
	return false;
}
bool AstarNode::operator==(const AstarNode other) const {
  return (xpos == other.xpos) && (ypos == other.ypos);
}
bool AstarNode::operator==(const AstarNode* other) const {
  return (xpos == other->xpos) && (ypos == other->ypos);
}