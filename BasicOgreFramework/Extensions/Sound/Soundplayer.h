//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_SOUND_H
#define OGRE_SOUND_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreAxisAlignedBox.h>
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
#include <boost/thread/thread.hpp>

using namespace std;

//A class to be instancified in order to add and play sounds from

class Soundplayer : public Ogre::AxisAlignedBox
{
	public:	
		Soundplayer();

		void addSound(ALbyte* fN, float soundPositionX, float soudnPositionY, float soundPositionZ, float soundVelocityX, float soundVelocityY, float soundVelocityZ);
		void setListenerPosition(float x, float y, float z);
		void setListenerVelocity(float x, float y, float z);
		void setListenerOrientation(float a, float b, float c, float d, float e, float f);	
		void setSourcePosition(int index, float x, float y, float z);

		int playSound(int index, float volume);
		int playSoundWithEcho(int index, float volume, vector<int> boxValues, vector<vector<int>>);

	private:
		ALboolean loadData(int index, float volume);
		
		// Sound sources positions and velocities
		vector<ALfloat> sourcePosX;
		vector<ALfloat> sourcePosY;
		vector<ALfloat> sourcePosZ;
		vector<ALfloat> sourceVelX;
		vector<ALfloat> sourceVelY;
		vector<ALfloat> sourceVelZ;

		// Buffer for sound data
		vector<ALuint> buffer;

		//Source of sound
		vector<ALuint> source;

		//A list of filenames for all added sounds
		vector<ALbyte*> fileNames;

		//Create listener position and velocity and orientation
		ALfloat listenerPos[3];
		ALfloat listenerVel[3];
		ALfloat listenerOri[6];
};

//Echo thread
class EchoThread 
{
public:
	EchoThread(int index, float volume, float delay, Soundplayer* sP);

	// The entry point for a thread
	void operator()();
	
private:
	float delay;
	float volume;
	int index;
	Soundplayer* sP;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||