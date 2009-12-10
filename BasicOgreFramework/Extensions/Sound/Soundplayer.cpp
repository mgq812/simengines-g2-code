//|||||||||||||||||||||||||||||||||||||||||||||||

#include "Soundplayer.h"

//Soundplayer constructor for setting default listener values
Soundplayer::Soundplayer()
{
	listenerPos[0] = 0.0;
	listenerPos[1] = 0.0;
	listenerPos[2] = 0.0;
	listenerVel[0] = 0.0;
	listenerVel[1] = 0.0;
	listenerVel[2] = 0.0;
	listenerOri[0] = 0.0;
	listenerOri[1] = 0.0;
	listenerOri[2] = -1.0;
	listenerOri[3] = 0.0;
	listenerOri[4] = 1.0;
	listenerOri[5] = 0.0;
}

//A method for adding a sound to the soundplayers list of sound it can play. The input is the file path to the sound, the coordinates for the sound, and the sounds velocity in each direction
void Soundplayer::addSound(ALbyte* filePath, float soundPositionX, float soundPositionY, float soundPositionZ, float soundVelocityX, float soundVelocityY, float soundVelocityZ)
{
	//Save file name
	fileNames.push_back(filePath);

	//Create a default buffer for the vector
	buffer.push_back(0);

	//Create a default source for the vector
	source.push_back(0);

	//Save positions and velocities
	sourcePosX.push_back(soundPositionX);
	sourcePosY.push_back(soundPositionY);
	sourcePosZ.push_back(soundPositionZ);
	sourceVelX.push_back(soundVelocityX);
	sourceVelY.push_back(soundVelocityY);
	sourceVelZ.push_back(soundVelocityZ);
}

//A method for updating the listeners position. The input is three coordinates for the room. 
void Soundplayer::setListenerPosition(float x, float y, float z)
{
	//Change the listener position
	listenerPos[0] = x;
	listenerPos[1] = y;
	listenerPos[2] = z;

	//Update the listener position
	alListenerfv(AL_POSITION,    listenerPos);
}
//A method for updating the listeners velocity. The input is three coordinates for a velocity vector. 
void Soundplayer::setListenerVelocity(float x, float y, float z)
{
	//Change the listener velocity
	listenerVel[0] = x;
	listenerVel[1] = y;
	listenerVel[2] = z;

	//Update the listener velocity
	alListenerfv(AL_VELOCITY,    listenerVel);
}
//A method for updating the listeners orientation. The input is six floats for the orientation. 
void Soundplayer::setListenerOrientation(float a, float b, float c, float d, float e, float f)
{
	//Change the listener orientation
	listenerOri[0] = a;
	listenerOri[1] = b;
	listenerOri[2] = c;
	listenerOri[3] = d;
	listenerOri[4] = e;
	listenerOri[5] = f;

	//Update the listener orientation
	alListenerfv(AL_ORIENTATION, listenerOri);
}

//A method for moving a sound source's position in the room. The input is the index of the sound, and it's new room coordinates. (The first added sound is of index 0, the second of index 2 and so on)
void Soundplayer::setSourcePosition(int index, float x, float y, float z)
{
	sourcePosX[index] = x;
	sourcePosY[index] = y;
	sourcePosZ[index] = z;
}

//A method for loading sound data before playing it. The input is an index for what sound to load.
ALboolean Soundplayer::loadData(int index, float volume)
{
    //Data holders
	ALvoid* data;
    ALsizei freq;
	ALsizei size;
    ALenum format;
	ALboolean loop;

	//Load wav data into a buffer
    alGenBuffers(1, &buffer[index]);
    if (alGetError() != AL_NO_ERROR)
        return AL_FALSE;
	alutLoadWAVFile(fileNames[index], &format, &data, &size, &freq, &loop);
    alBufferData(buffer[index], format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);

	//Bind buffer to a source
	alGenSources(1, &source[index]);

    if (alGetError() != AL_NO_ERROR)
        return AL_FALSE;

	//Indata
	ALfloat sourcePosIn[] = { sourcePosX[index], sourcePosY[index], sourcePosZ[index] };
	ALfloat sourceVelIn[] = { sourceVelX[index], sourceVelY[index], sourceVelZ[index] };

	//Setting sound data
	alSourcei (source[index], AL_BUFFER,   buffer[index]);
    alSourcef (source[index], AL_PITCH,    1.0f);
	alSourcefv(source[index], AL_POSITION, sourcePosIn);
    alSourcefv(source[index], AL_VELOCITY, sourceVelIn);
    alSourcei (source[index], AL_LOOPING,  loop);
    alSourcef (source[index], AL_GAIN,     volume);
	alSourcef(source[index], AL_REFERENCE_DISTANCE, 20.0f);

	 // Make an error check
    if (alGetError() == AL_NO_ERROR)
        return AL_TRUE;

    return AL_FALSE;
}
//A method that plays a sound that is decided by the input, which is an integer for what sound to play
int Soundplayer::playSound(int index, float volume)
{

    alutInit(NULL, NULL);
    alGetError();

	  // Load the wav data.
    if (loadData(index, volume) == AL_FALSE)
        return -1;

	//Set listener values
	alListenerfv(AL_POSITION,    listenerPos);
	alListenerfv(AL_VELOCITY,    listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);


	//Play the sound
    alSourcePlay(source[index]);
	
	return 1;
}

//A method that plays a sound with an echo that is decided by the input, which is an integer for what sound to play
int Soundplayer::playSoundWithEcho(int index, float volume, vector<int> boxValues, vector<vector<int>> boxPositions)
{
    alutInit(NULL, NULL);
    alGetError();

	  // Load the wav data.
    if (loadData(index, volume) == AL_FALSE)
        return -1;

	//Set listener values
	alListenerfv(AL_POSITION,    listenerPos);
	alListenerfv(AL_VELOCITY,    listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	//Play the sound
    alSourcePlay(source[index]);

	//Calculate echo
	float in[3] = { sourcePosX[index], sourcePosY[index], sourcePosZ[index] };
	EchoProperties echoProperties = Echo::calculateEcho(volume, boxValues, boxPositions, in);
	
	//Change volume and wait
	float newVolume = echoProperties.getVolume();
	float delay = echoProperties.getDelay();

	//Start an echo thread
	boost::thread echoThread(EchoThread(index, newVolume, delay, this));
	echoThread.join();

	return 1;
}

//The constructor of the EchoThread
EchoThread::EchoThread(int index, float volume, float delay, Soundplayer* sP)
{
		this->index = index;
		this->volume = volume;
		this->sP = sP;
		this->delay = delay;
}
void EchoThread::operator ()()
{
		boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
		sP->playSound(index, volume);
}