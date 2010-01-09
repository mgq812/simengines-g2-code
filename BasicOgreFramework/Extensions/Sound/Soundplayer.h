#include <conio.h>
#include <stdlib.h>
#include <al.h>
#include <alc.h>
#include <alut.h>
#include <string>
#include <list>
#include <vector>
#include <Echo.h>
#include <iostream>

using namespace std;

/** @brief The soundplayer class that is built to play sounds through openal, using the echo extension.
    This class plays sounds by using OpenAL and ALut. It requires the Echo class and the EchoProperties class to be usable.
    @author Alexander Nordh
	*/
class Soundplayer
{
	public:	
		/** The default constructor for the Soundplayer class.
			*/
		Soundplayer();
		/** An method for adding sounds into the Soundplayer.
			@param fN - The file name of the sound file to be added.
			@param soundPositionX - The x-position of this sound source.
			@param soundPositionY - The y-position of this sound source.
			@param soundPositionZ - The z-position of this sound source.
			@param soundVelocityX - The x-velocity of this sound source
			@param soundVelocityY - The y-velocity of this sound source
			@param soundVelocityZ - The z-velocity of this sound source
			*/
		void addSound(ALbyte* fN, float soundPositionX, float soundPositionY, float soundPositionZ, float soundVelocityX, float soundVelocityY, float soundVelocityZ);
		
		/** An method for moving the listener.
			@param x - The new x position for the listener.
			@param y - The new y position for the listener.
			@param z - The new z position for the listener.
			*/
		void setListenerPosition(float x, float y, float z);

		/** An method for changing the velocity of the listener.
			@param x - The new x velocity for the listener.
			@param y - The new y velocity for the listener.
			@param z - The new z velocity for the listener.
			*/
		void setListenerVelocity(float x, float y, float z);

		/** An method for changing the position of an added sound source.
			@param index - The index of the sound which should be moved.
			@param x - The new x-position of the sound.
			@param y - The new y-position of the sound.
			@param z - The new z-position of the sound.
			*/
		void setSourcePosition(int index, float x, float y, float z);

		/** An method for setting a new distance and reflection scale.
			@param distanceScale - The new distance scale to use when calculating echoes.
			@param reflectionScale - The new reflection scale to use when calculating echoes.
			*/
		void setScales(float distanceScale, float reflectionScale);
		
		/** An method for stopping a sound source from playing.
			@param index - The index of the sound to stop.
			*/
		void stopSound(int index);

		/** An method for starting to play a sound source.
			@param index - The index of the sound to play.
			@param volume - The volume the sound should be played in (from 0.0 to 1.0).
			@param loop - Decides if the sound should loop or not.
			*/
		void playSound(int index, float volume, bool loop);

		/** An method for starting to play a sound source after a certain time.
			@param index - The index of the sound to play.
			@param volume - The volume the sound should be played in (from 0.0 to 1.0).
			@param delay - The time to wait before playing the sound in milliseconds.
			*/
		void playIn(int index, float volume, float delay);

		/** An method for starting to play a sound source followed by an echo.
			@param index - The index of the sound to play.
			@param volume - The volume the sound should be played in (from 0.0 to 1.0).
			@param boxValues - Values of how much each object reflects the sound.
			@param boxPositions - The positions of the objects reflecting sound.
			*/
		void playSoundWithEcho(int index, float volume, vector<float> boxValues, vector<vector<float>> boxPosition);
		
		/** An method for starting to play a sound source followed by an echo after a certain time.
			@param index - The index of the sound to play.
			@param volume - The volume the sound should be played in (from 0.0 to 1.0).
			@param delay - The time to wait before playing the sound which is then followed by an echo.
			@param boxValues - Values of how much each object reflects the sound.
			@param boxPositions - The positions of the objects reflecting sound.
			*/
		void Soundplayer::playWithEchoIn(int index, float volume, float delay, vector<float> boxValues, vector<vector<float>> boxPositions);

	private:
		/** An method for loading data into OpenAL.
			@param index - The index of the sound to load.
			@param volume - The volume to set into OpenAL.
			@param loop - The boolean to set into OpenAL which determines if the sound should loop or not.
			*/
		void loadData(int index, float volume, ALboolean loop);
		
		float distanceScale;/**< A distance scale which can be calibrated to fit the distance system used in the game. */
		float reflectionScale;/**< A reflection scale which can be calibrated to fit the environment used in the game. */

		vector<ALfloat> sourcePosX;/**< The sound sources x-positions vector. */
		vector<ALfloat> sourcePosY;/**< The sound sources y-positions vector. */
		vector<ALfloat> sourcePosZ;/**< The sound sources z-positions vector. */
		vector<ALfloat> sourceVelX;/**< The sound sources x-velocity vector. */
		vector<ALfloat> sourceVelY;/**< The sound sources y-velocity vector. */
		vector<ALfloat> sourceVelZ;/**< The sound sources z-velocity vector. */

		vector<ALuint> buffer;/**< The bufferers for the sound sources. */

		vector<bool> iteratedThrough;/**< Booleans in a vector to say if an sound source had already generated an bufferer so that it is not done multiple times for the same sound, as OpenAL can only handle a certain amount of bufferers which depends on the computer. */
		vector<int> sIV;/**< The sound iteration value. Every sound source has 10 bufferers each to be able to play 10 of the same kind of sound at the same time. This is the iteration value. */
		vector<vector<ALuint>> source;/**< The buffer sound sources (There is 10 for each sound). */

		vector<ALbyte*> fileNames;/**< The sound sources' filenames. */

		ALfloat listenerPos[3];/**< The listener's x, y and z positions. */
		ALfloat listenerVel[3];/**< The listener's x, y and z velocities. */
		ALfloat listenerOri[6];/**< The listener's orientation. */
};
