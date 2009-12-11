//|||||||||||||||||||||||||||||||||||||||||||||||

#include "Echo.h"
#include "math.h"

EchoProperties Echo::calculateEcho(int volume, vector<int> boxValues, vector<vector<int>> boxPositions , float soundPosition[3])
{
	//-----Initialization of used variables-----
	//--The delay of the echo--
	int delay = 0;

	//--The total reflecting area--
	int tRA = 0;

	//--Distance vector between sound source and boxes for calculations and the actual distance--
	float distanceVector[3];
	int distance = 0;

	//--List of distances for calculating the delay--
	vector<int> distances;
	int amountOfBoxes;
	int meanDistance;

	//-----Methods for determine what boxes that should be used-----

	//--Remove objects that are too far away to consider--
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector[0] = boxPositions[i][0];
		distanceVector[1] = boxPositions[i][1];
		distanceVector[2] = boxPositions[i][2];
		distanceVector[0] -= soundPosition[0];
		distanceVector[1] -= soundPosition[1];
		distanceVector[2] -= soundPosition[2];
		distance = sqrt(pow(distanceVector[0], 2) + pow(distanceVector[1], 2) + pow(distanceVector[2], 2));
		//If close enough, save the boxes
		if(distance > volume*750)
		{
			boxValues.erase(boxValues.begin() + i);
			boxPositions.erase(boxPositions.begin() + i);
		}

	}

	//--Remove boxes that are too close--
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector[0] = boxPositions[i][0];
		distanceVector[1] = boxPositions[i][1];
		distanceVector[2] = boxPositions[i][2];
		distanceVector[0] -= soundPosition[0];
		distanceVector[1] -= soundPosition[1];
		distanceVector[2] -= soundPosition[2];
		distance = sqrt(pow(distanceVector[0], 2) + pow(distanceVector[1], 2) + pow(distanceVector[2], 2));

		//If close enough, save the boxes
		if(distance < 75)
		{
			boxValues.erase(boxValues.begin() + i);
			boxPositions.erase(boxPositions.begin() + i);
		}
	}

	//-----Calculations of areas, echo, distance delay and return-----

	//--Calculate the total reflecting area--
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		tRA += boxValues[i];
	}

	//--Scale sound after the total reflacting area--
	
	//TEMP
	tRA = 1000;
	volume *= tRA*0.001;

	//--Calculate the delay of the sound--
	for(int i = 0; i < boxValues.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector[0] = boxPositions[i][0];
		distanceVector[1] = boxPositions[i][1];
		distanceVector[2] = boxPositions[i][2];
		distanceVector[0] -= soundPosition[0];
		distanceVector[1] -= soundPosition[1];
		distanceVector[2] -= soundPosition[2];
		
		//Add the distance to the list
		distances.push_back(sqrt(pow(distanceVector[0], 2) + pow(distanceVector[1], 2) + pow(distanceVector[2], 2)));
	}
	//Calculate the mean distance
	amountOfBoxes = distances.size();
	for(int i = 0; i < amountOfBoxes; i++)
		meanDistance += distances[i];
	meanDistance /= amountOfBoxes;
	
	//Scale the delay after the distance
	delay = meanDistance*2;

	//--Create and return an EchoProperties object--
	EchoProperties eP = EchoProperties(volume, delay);
	return eP;
}