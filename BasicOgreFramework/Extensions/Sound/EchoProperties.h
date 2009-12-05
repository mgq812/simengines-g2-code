//This class is meant to be instancified for describing the echo.
class EchoProperties
{
public:
	float newVolume;
	int delay;
	EchoProperties(float newVolume, int delay)
	{
		this->newVolume = newVolume;
		this->delay = delay;
	}
	float EchoProperties::getVolume()
	{
		return newVolume;
	}
	int EchoProperties::getDelay()
	{
		return delay;
	}
};


