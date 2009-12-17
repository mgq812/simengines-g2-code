//This class is meant to be instancified for describing the echo.
class EchoProperties
{
public:
	float newVolume;
	int delay;
	bool playEcho;
	EchoProperties(float newVolume, int delay, bool playEcho)
	{
		this->newVolume = newVolume;
		this->delay = delay;
		this->playEcho = playEcho;
	}
	float EchoProperties::getVolume()
	{
		return newVolume;
	}
	int EchoProperties::getDelay()
	{
		return delay;
	}
	bool EchoProperties::getPlayEcho()
	{
		return playEcho;
	}
};


