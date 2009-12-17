#include <EchoProperties.h>
#include <vector>

using namespace std;

class Echo
{
	public:	
		static EchoProperties Echo::calculateEcho(float volume, vector<float> boxValues, vector<vector<float>> boxPositions, float soundPosition[3], float distanceScale, float reflectionScale);
};