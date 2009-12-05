//|||||||||||||||||||||||||||||||||||||||||||||||

#include "Echo.h"

//The method for doing the echo calculation.
//volume: The beginning volume of the sound
//boundingBoxes: The list of the bounding boxes in the scene
//soundPosition: The position of the sound source
EchoProperties Echo::calculateEcho(int volume, vector<AxisAlignedBox> boundingBoxes, Ogre::Vector3 soundPosition)
{
	//-----Initialization of used variables-----
	//--The delay of the echo--
	int delay = 0;

	//--The total reflecting area--
	int tRA = 0;

	//--Distance vector between sound source and boxes for calculations and the actual distance--
	Ogre::Vector3 distanceVector;
	int distance = 0;

	//--Temp vector for holding boxes--
	vector<AxisAlignedBox> tempBoxes;

	//--List of distances for calculating the delay--
	vector<int> distances;
	int amounOfBoxes;
	int meanDistance;

	//-----Methods for determine what boxes that should be used-----

	//--Remove objects that are too far away to consider--
	for(unsigned int i = 0; i < boundingBoxes.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector = Ogre::Vector3(boundingBoxes[i].getCenter().x, boundingBoxes[i].getCenter().y, boundingBoxes[i].getCenter().z);
		distanceVector.x -= soundPosition.x;
		distanceVector.y -= soundPosition.y;
		distanceVector.z -= soundPosition.z;
		distance = distanceVector.length();
		
		//If close enough, save the boxes
		if(distance < volume*1000)
			tempBoxes.push_back(boundingBoxes[i]);
	}

	//Make new list and clear old list
	boundingBoxes = tempBoxes;
	tempBoxes.clear();

	//--Remove occluded boxes--

	//--Remove boxes that are too close--
	for(unsigned int i = 0; i < boundingBoxes.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector = Ogre::Vector3(boundingBoxes[i].getCenter().x, boundingBoxes[i].getCenter().y, boundingBoxes[i].getCenter().z);
		distanceVector.x -= soundPosition.x;
		distanceVector.y -= soundPosition.y;
		distanceVector.z -= soundPosition.z;
		distance = distanceVector.length();
		
		//If far away enough, save the boxes
		if(distance > 75)
			tempBoxes.push_back(boundingBoxes[i]);
	}
	
	//Make new list and clear old list
	boundingBoxes = tempBoxes;
	tempBoxes.clear();


	//-----Calculations of areas, echo, distance delay and return-----

	//--Calculate the total reflecting area--
	for(unsigned int i = 0; i < boundingBoxes.size(); i++)
	{
		tRA += boundingBoxes[i].volume();
	}

	//--Scale sound after the total reflacting area--
	
	//TEMP
	tRA = 1000;
	volume *= tRA*0.001;

	//--Calculate the delay of the sound--
	//TEMP
	delay = 500;
	/*for(int i = 0; i < boundingBoxes.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector = Vector3(boundingBoxes[i].getCenter().x, boundingBoxes[i].getCenter().y, boundingBoxes[i].getCenter().z);
		distanceVector.x -= soundPosition.x;
		distanceVector.y -= soundPosition.y;
		distanceVector.z -= soundPosition.z;
		distance = distanceVector.length();
		
		//Add the distance to the list
		distances.push_back(distance);
	}
	amountOfBoxes = distances.size();
	for(int i = 0; i < amountOfBoxes; i++)
		meanDistance += distances[i];
	meanDistance /= amountOfBoxes;

	*/

	//--Create and return an EchoProperties object--
	EchoProperties eP = EchoProperties(volume, delay);
	return eP;
}