#include "Echo.h"
#include "math.h"

//A method for generating an echo that depends on the surroundings
EchoProperties Echo::calculateEcho(float volume, vector<float> boxValues, vector<vector<float>> boxPositions , float soundPosition[3], float distanceScale, float reflectionScale)
{
	//--Variables--

	//Does the sound return too quickly?
	bool playEcho = true;

	//The delay of the echo
	int delay = 0;

	//The total reflecting value
	float tRV = 0;

	//Distance vector between sound source and boxes for calculations and the actual distance
	float distanceVector[3];
	float distance = 0;

	//List of distances, the mean distance and the amount of boxes for calculating the delay
	vector<float> distances;
	float aRD = 0;

	//Procentual weight for the box values, used to see how much they should affect the approximate reflection distance
	vector<float> pWeight;

	//The starting volume
	float sVolume = volume;

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
		
		//If too far away, erase the box
		if(distance > volume*1000)
		{
			boxValues.erase(boxValues.begin() + i);
			boxPositions.erase(boxPositions.begin() + i);
		}
	}

	//--Calculate the total reflecting value--
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		tRV += boxValues[i];
	}

	//--Calculate the percentually reflection from each box--
	for(int u = 0; u < boxValues.size(); u++)
	{
		pWeight.push_back(((float)boxValues[u]/tRV));
	}

	//--Scale volume after how much reflection there is--
	if(tRV > reflectionScale)
		tRV = reflectionScale;
	float reduce = (1.0f -((float)tRV/reflectionScale));
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
		
		//Add the distance to the distance list
		distances.push_back((float)sqrt(pow((double)distanceVector[0], 2) + pow((double)distanceVector[1], 2) + pow((double)distanceVector[2], 2)));
	}
	//Calculate the approximate reflection distance
	for(int u = 0; u < pWeight.size(); u++)
	{
		aRD += (pWeight[u]*distances[u]);
	}
	
	//--Scale the delay after the distance--
	delay = (int)(aRD*distanceScale);

	//--Simulate soundloss due to distance--
	reduce = (float)(aRD*0.0006);
	volume -= reduce;
	//If distance too far and volume negative, make it zero
	if(volume < 0.0f)
		volume = 0.0f;

	//If the sound return too quickly, don't play an echo
	if(delay <= 40)
	{
		playEcho = false;
	}
	//--Create and return an EchoProperties object--
	EchoProperties eP = EchoProperties(volume, delay, playEcho);
	return eP;
}