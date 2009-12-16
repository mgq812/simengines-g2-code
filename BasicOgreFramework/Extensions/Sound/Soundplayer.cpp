#include "Soundplayer.h"
#include "windows.h"

//Soundplayer constructor for setting default listener, orientation values and scales

Soundplayer::Soundplayer()
{
	//Default scales
	distanceScale = 2.0f;
	reflectionScale = 1000000.0f;

	//Default listener position and orientation
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

	//Create 10 default sound sources for each sound (meaning that 10 sounds of the same kind can only be playing at a time)
	vector<ALuint> inS;
	inS.push_back(-1);inS.push_back(-1);
	inS.push_back(-1);inS.push_back(-1);
	inS.push_back(-1);inS.push_back(-1);
	inS.push_back(-1);inS.push_back(-1);
	inS.push_back(-1);inS.push_back(-1);
	
	//Create the starting iteration value for the 10 sources and a boolean that says if you have generated sources for all 10 sources yet.
	sIV.push_back(-1);
	iteratedThrough.push_back(false);
	source.push_back(inS);

	//Save positions and velocities
	sourcePosX.push_back(soundPositionX);
	sourcePosY.push_back(soundPositionY);
	sourcePosZ.push_back(soundPositionZ);
	sourceVelX.push_back(soundVelocityX);
	sourceVelY.push_back(soundVelocityY);
	sourceVelZ.push_back(soundVelocityZ);
}

//This sets game specific scales for echos. The distance scale should be modifyed towards the games position system and the reflection scale to the reflection values sent in
void Soundplayer::setScales(float dS, float rS)
{
	distanceScale = dS;
	reflectionScale = rS;
}

//A method for updating the listeners position. The input is three coordinates for the room. 
void Soundplayer::setListenerPosition(float x, float y, float z)
{
	//Change the listener position
	listenerPos[0] = x;
	listenerPos[1] = y;
	listenerPos[2] = z;

	//Update the listener position
	alListenerfv(AL_POSITION, listenerPos);
}
//A method for updating the listeners velocity. The input is three coordinates for a velocity vector. 
void Soundplayer::setListenerVelocity(float x, float y, float z)
{
	//Change the listener velocity
	listenerVel[0] = x;
	listenerVel[1] = y;
	listenerVel[2] = z;

	//Update the listener velocity
	alListenerfv(AL_VELOCITY, listenerVel);
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

//A method for moving a sound source's position in the room. The input is the index of the sound, and it's new room coordinates.
void Soundplayer::setSourcePosition(int index, float x, float y, float z)
{
	sourcePosX[index] = x;
	sourcePosY[index] = y;
	sourcePosZ[index] = z;
}

//A method for loading sound data before playing it. The input is an index for what sound to load.
void Soundplayer::loadData(int index, float volume)
{
    //Data holders
	ALvoid* data;
    ALsizei freq;
	ALsizei size;
    ALenum format;
	ALboolean loop;

	//Increment the source iteration value, reset it to 0 if it reaches the max
	sIV[index]++;
	if(sIV[index] == 10)
	{
		sIV[index] = 0;
		iteratedThrough[index] = true;
	}


	//Generate buffer and load wav data into it
    alGenBuffers(1, &buffer[index]);
	alutLoadWAVFile(fileNames[index], &format, &data, &size, &freq, &loop);
    alBufferData(buffer[index], format, data, size, freq);

	//Unload wav data
    alutUnloadWAV(format, data, size, freq);

	//Generate a source
	if(!iteratedThrough[index])
		alGenSources(1, &source[index][sIV[index]]);
	//Indata
	ALfloat sourcePosIn[] = { sourcePosX[index], sourcePosY[index], sourcePosZ[index] };
	ALfloat sourceVelIn[] = { sourceVelX[index], sourceVelY[index], sourceVelZ[index] };

	//Setting sound data and binding buffer to source
	alSourcei (source[index][sIV[index]], AL_BUFFER, buffer[index]);
    alSourcef (source[index][sIV[index]], AL_PITCH, 1.0f);
	alSourcefv(source[index][sIV[index]], AL_POSITION, sourcePosIn);
    alSourcefv(source[index][sIV[index]], AL_VELOCITY, sourceVelIn);
    alSourcei (source[index][sIV[index]], AL_LOOPING, loop);
    alSourcef (source[index][sIV[index]], AL_GAIN, volume);
	alSourcef(source[index][sIV[index]], AL_REFERENCE_DISTANCE, 20.0f);
}
//A method that plays a sound that is decided by the index. Also sets the volume after the volume input.
void Soundplayer::playSound(int index, float volume)
{
	//Initialize alut
    alutInit(NULL, NULL);

	// Load the wav data
	loadData(index, volume);

	//Set listener values
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	//Play the sound
    alSourcePlay(source[index][sIV[index]]);
}

//A struct for passing parameters into the ThreadPlayEcho method through a new thread created by the CreateThread() method
struct ThreadParameterPassingStruct
{
	int i;
	float v;
	int d;
	Soundplayer* s;
};

//The method to be run by the new thread in order to play the echo. The parameter is cast into the right type(ThreadParameterPassingStruct)
DWORD WINAPI ThreadPlay(LPVOID lparam)
{
	//Cast parameter into struct
	ThreadParameterPassingStruct param = *(ThreadParameterPassingStruct*)lparam;
	
	//Sleep for the delay time
	Sleep((DWORD)param.d);
	
	//Play the echo sound and return
	param.s->playSound(param.i, param.v);
	return 0;
}

//A method that plays a sound with an echo that is decided by the input, which is an integer for what sound to play, an volume for the volume, values of other objects in the room and positions of those objects
void Soundplayer::playSoundWithEcho(int index, float volume, vector<int> boxValues, vector<vector<float>> boxPositions)
{
	//Initialize alut
    alutInit(NULL, NULL);

	// Load the wav data
    loadData(index, volume);

	//Set listener values
	alListenerfv(AL_POSITION,    listenerPos);
	alListenerfv(AL_VELOCITY,    listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	//Play the sound
    alSourcePlay(source[index][sIV[index]]);

	//Calculate echo
	float in[3] = { sourcePosX[index], sourcePosY[index], sourcePosZ[index] };
	EchoProperties echoProperties = Echo::calculateEcho(volume, boxValues, boxPositions, in, distanceScale, reflectionScale);
	
	//Change volume and waiting time
	float newVolume = echoProperties.getVolume();
	int delay = echoProperties.getDelay();
	
	playIn(index, newVolume, delay);

}

//A struct needed for sending many parameters into the playIn thread
ThreadParameterPassingStruct pIT;

//A method for playing a sound after a certain time.
void Soundplayer::playIn(int index, float volume, float delay)
{
	//Create the thread handle
	HANDLE thread;

	//Put parameters in the struct made for passing parameters into the thread
	pIT.d = delay;
	pIT.i = index;
	pIT.v = volume;
	pIT.s = this;

	//Create and start a new thread
	thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadPlay, &pIT,0,0);
	
	//Release the thread
	CloseHandle(thread);
}
