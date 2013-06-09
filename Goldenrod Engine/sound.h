#ifndef SOUND_H
#define SOUND_H

//irrKlang Sound Library
#include "irrKlang/irrKlang.h"
#pragma comment(lib, "lib/irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;


class SoundEngine{
public:
	SoundEngine();
	~SoundEngine();
	ISoundEngine* getEngine();
private:
	ISoundEngine* engine;
	int tickTimer;
};

void puttSFX(float power, SoundEngine * s);
void bounceSFX(float speed, SoundEngine * s);
void rollSFX(float speed, SoundEngine * s);

#endif