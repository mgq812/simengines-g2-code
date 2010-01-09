/** @brief A class made to be instantiated for describing an echo.
    @author Alexander Nordh
    */
class EchoProperties
{
public:
	float newVolume;/**< The volume of the echo. */
	int delay;/**< The delay before the echo should be played. */
	bool playEcho;/**< A boolean that determines if the echo is worth playing (could be too low or to sudden to be noticed). */
	
	/** The constructor of EchoProperties. It takes in all the required input and sets all it's variables to that.
		@param newVolume - The echo's volume.
		@param delay - The delay before the echo is played.
		@param playEcho - A boolean to determine if the echo should be played.
		*/
	EchoProperties(float newVolume, int delay, bool playEcho)
	{
		this->newVolume = newVolume;
		this->delay = delay;
		this->playEcho = playEcho;
	}
	/** A method for getting the volume of the echo.
		@return newVolume - The volume of the echo.
		*/
	float EchoProperties::getVolume()
	{
		return newVolume;
	}
	/** A method for getting the delay before the echo should be played.
		@return delay - The delay before the echo should be played.
		*/
	int EchoProperties::getDelay()
	{
		return delay;
	}
	/** A method for getting the boolean which determines if the echo should be played.
		@return playEcho - The boolean which determines if the echo should be played.
		*/
	bool EchoProperties::getPlayEcho()
	{
		return playEcho;
	}
};


