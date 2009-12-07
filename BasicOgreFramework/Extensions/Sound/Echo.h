//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_ECHO_H
#define OGRE_ECHO_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <EchoProperties.h>
#include <vector>

using namespace std;

//A class for generating an echo which depends on the surroundings
class Echo
{
	public:	
		static EchoProperties Echo::calculateEcho(int volume, vector<int> boxValues, vector<vector<int>> boxPositions, float soundPosition[3]);
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||