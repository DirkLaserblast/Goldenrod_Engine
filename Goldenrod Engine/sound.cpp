#include "sound.h"

SoundEngine::SoundEngine()
{
	this->engine = createIrrKlangDevice();
}

SoundEngine::~SoundEngine()
{
	this->engine->drop();
}

ISoundEngine* SoundEngine::getEngine()
{
	return this->engine;
}