#include <EchoProperties.h>
#include <vector>

using namespace std;

	/** @brief The class which can do an calculation to find out the properties of an echo.
		@author Alexander Nordh
		*/
class Echo
{
	public:	
		/**The calcualtion method, which does the echo calculation. It can be called statically and returns an EchoProperties object which describes the echo.
			@param volume - The volume of the sound.
			@param boxValues - The reflection values of the objects reflecting sounds in order to create the echo.
			@param boxPositions - The positions of the objects reflecting sounds in order to create the echo.
			@param soundPositions - The position of the sound source, listed as x, then y, then z in the array.
			@param distanceScale - The higher this scale is, the faster the sound reduces due to distance, and the longer time it will take for the sound to return after being made.
			@param reflectionScale - The higher reflection scale, the higher is the sound of the reflection.
			@return An EchoProperties object which contains the echo's volume, the delay before it should be played and if it should be played.
			*/
		static EchoProperties Echo::calculateEcho(float volume, vector<float> boxValues, vector<vector<float>> boxPositions, float soundPosition[3], float distanceScale, float reflectionScale);
};