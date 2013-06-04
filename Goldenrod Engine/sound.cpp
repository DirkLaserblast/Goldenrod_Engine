#include "sound.h"

SoundEngine::SoundEngine()
{
	this->engine = createIrrKlangDevice();
	this->engine->setSoundVolume(1.0f);
}

SoundEngine::~SoundEngine()
{
	this->engine->drop();
}

ISoundEngine* SoundEngine::getEngine()
{
	return this->engine;
}

void SoundEngine::updateListenerPosition(vec3 listenPos, vec3 listenDirection, vec3 up)
{
	engine->setListenerPosition(vec3df(listenPos.x, listenPos.y, listenPos.z), vec3df(listenDirection.x, listenDirection.y, listenDirection.z), vec3df(up.x, up.y, up.z));
}

void SoundEngine::playSound3D(const char * sound, vec3 position)
{
	engine->play3D(sound, vec3df(position.x, position.y, position.z));
}