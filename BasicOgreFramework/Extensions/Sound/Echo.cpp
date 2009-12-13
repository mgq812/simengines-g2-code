//|||||||||||||||||||||||||||||||||||||||||||||||

#include "Echo.h"
#include "math.h"

EchoProperties Echo::calculateEcho(float volume, vector<int> boxValues, vector<vector<float>> boxPositions , float soundPosition[3])
{
	//-----Initialization of used variables-----
	//--The delay of the echo--
	int delay = 0;

	//--The total reflecting value--
	float tRV = 0;

	//--Distance vector between sound source and boxes for calculations and the actual distance--
	float distanceVector[3];
	float distance = 0;

	//--List of distances for calculating the delay--
	vector<float> distances;
	int amountOfBoxes;
	float meanDistance = 0;

	//--Starting volume--
	float sVolume = volume;

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
		distance = (float)sqrt(pow((double)distanceVector[0], 2) + pow((double)distanceVector[1], 2) + pow((double)distanceVector[2], 2));
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
		distanceVector[0] = (float)boxPositions[i][0];
		distanceVector[1] = (float)boxPositions[i][1];
		distanceVector[2] = (float)boxPositions[i][2];
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

	//--Calculate the total reflecting value--
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		tRV += boxValues[i];
	}

	//--Scale volume after how much reflection there is--
	if(tRV > 1000000.0f)
		tRV = 1000000.0f;
	float reduce = (1.0f -((float)tRV/1000000));
	volume -= reduce;

	//--Calculate the delay of the sound--
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		//Calculate the distance and the distance vector
		distanceVector[0] = boxPositions[i][0];
		distanceVector[1] = boxPositions[i][1];
		distanceVector[2] = boxPositions[i][2];
		distanceVector[0] -= soundPosition[0];
		distanceVector[1] -= soundPosition[1];
		distanceVector[2] -= soundPosition[2];
		
		//Add the distance to the list
		distances.push_back((float)sqrt(pow((double)distanceVector[0], 2) + pow((double)distanceVector[1], 2) + pow((double)distanceVector[2], 2)));
	}
	//Calculate the mean distance
	amountOfBoxes = (int)distances.size();
	for(int i = 0; i < amountOfBoxes; i++)
		meanDistance += distances[i];
	meanDistance /= amountOfBoxes;
	
	//--Scale the delay after the distance--
	delay = (int)(meanDistance*1.5);

	//--Simulate soundloss due to distance--
	reduce = (float)(meanDistance*0.0006);
	volume -= reduce;

	//--Fix lower than 0 volume bugg and stop too high echos
	if(volume < 0.0f)
		volume = 0.0f;
	if(volume > sVolume)
		volume = sVolume;

	//--Create and return an EchoProperties object--
	EchoProperties eP = EchoProperties(volume, delay);
	return eP;
}