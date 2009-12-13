#include <EchoProperties.h>
#include <vector>

using namespace std;

class Echo
{
	public:	
		static EchoProperties Echo::calculateEcho(float volume, vector<int> boxValues, vector<vector<float>> boxPositions, float soundPosition[3]);
};