#include "Echo.h"
#include "math.h"

/** The calculateEcho method for calculating the echo's properties.
	*/
EchoProperties Echo::calculateEcho(float volume, vector<float> boxValues, vector<vector<float>> boxPositions , float soundPosition[3], float distanceScale, float reflectionScale)
{

	bool playEcho = true; /**< A boolean to tell if the echo should be played at all. */

	int delay = 0;/**< The delay before the echo should return. */

	float tRV = 0;/**< A variable keeping the total reflecting value (The sum of reflection values) */

	float distanceVector[3];/**< A distance vector used to calculate the distance between the sound source and different objects. */
	float distance = 0;/**< A distance variable used to calculate what objects to keep in the calculations and which to ignore. */

	vector<float> distances;/**< A list of all the distances from the sound source to the objects */
	float aRD = 0;/** The approximate reflection distance. An approximation of where the sound has bounced to reduce the volume and set the delay appropriately.  */

	vector<float> pWeight;/**< The weights for each reflecting object used to calculate the aRD */

	float sVolume = volume;/**< Save the start volume in sVolume variable */

	//Remove objects that are too far away to consider--
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

	//Calculate the total reflecting value
	for(unsigned int i = 0; i < boxValues.size(); i++)
	{
		tRV += boxValues[i];
	}

	//Calculate the percentually reflection from each box
	for(int u = 0; u < boxValues.size(); u++)
	{
		pWeight.push_back(((float)boxValues[u]/tRV));
	}

	//Scale volume after how much reflection there is
	if(tRV > reflectionScale)
		tRV = reflectionScale;
	float reduce = (1.0f -((float)tRV/reflectionScale));
	volume -= reduce;

	//Calculate the delay of the sound
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
	
	//Scale the delay after the distance
	delay = (int)(aRD*distanceScale);

	//Simulate soundloss due to distance
	reduce = (float)(aRD*0.0007);
	volume -= reduce;
	//If distance too far and volume negative, make it zero
	if(volume < 0.0f)
		volume = 0.0f;

	//If the sound return too quickly, don't play an echo
	if(delay <= 40)
	{
		playEcho = false;
	}
	//Create and return an EchoProperties object
	EchoProperties eP = EchoProperties(volume, delay, playEcho);
	return eP;
}