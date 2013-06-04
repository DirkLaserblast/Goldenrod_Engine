#ifndef SOUND_H
#define SOUND_H

//irrKlang Sound Library
#include "irrKlang/irrKlang.h"
#include "glm/glm.hpp"
#pragma comment(lib, "lib/irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;
using namespace glm;


class SoundEngine{
public:
	SoundEngine();
	~SoundEngine();
	void updateListenerPosition(vec3 listenPos, vec3 listenDirection, vec3 up); //Updates 3D audio position in relation to camera
	void playSound3D(const char * sound, vec3 position);
	ISoundEngine* getEngine();
private:
	ISoundEngine* engine;
};


#endif