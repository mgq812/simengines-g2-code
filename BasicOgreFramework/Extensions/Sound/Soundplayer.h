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

//The soundplayer class that is build to play sounds through openal, using my echo extension
class Soundplayer
{
	//Public methods for the Soundplayer class
	public:	
		Soundplayer();
		
		void addSound(ALbyte* fN, float soundPositionX, float soundPositionY, float soundPositionZ, float soundVelocityX, float soundVelocityY, float soundVelocityZ);
		void setListenerPosition(float x, float y, float z);
		void setListenerVelocity(float x, float y, float z);
		void setListenerOrientation(float a, float b, float c, float d, float e, float f);	
		void setSourcePosition(int index, float x, float y, float z);
		void setScales(float distanceScale, float reflectionScale);
		
		
		void playSound(int index, float volume);
		void playIn(int index, float volume, float delay);
		void playSoundWithEcho(int index, float volume, vector<int> boxValues, vector<vector<float>> boxPosition);

	//Private methods and variables for the Soundplayer class
	private:
		void loadData(int index, float volume);
		
		//Scales for modifying the echoes
		float distanceScale;
		float reflectionScale;

		//Sound sources positions and velocities
		vector<ALfloat> sourcePosX;
		vector<ALfloat> sourcePosY;
		vector<ALfloat> sourcePosZ;
		vector<ALfloat> sourceVelX;
		vector<ALfloat> sourceVelY;
		vector<ALfloat> sourceVelZ;

		// Buffer for sound data
		vector<ALuint> buffer;

		//Sources of sound, their source iteration values and a boolean for each sound telling if it has been iterated through yet
		vector<bool> iteratedThrough;
		vector<int> sIV;
		vector<vector<ALuint>> source;

		//A list of filenames for all added sounds
		vector<ALbyte*> fileNames;

		//Create listener position and velocity and orientation
		ALfloat listenerPos[3];
		ALfloat listenerVel[3];
		ALfloat listenerOri[6];
};
