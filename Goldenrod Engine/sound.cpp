#include "sound.h"
#include "time.h"

using namespace std;

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

int speedTier(float speed)
{
	if (speed > 0.07) return 0;
	else if (speed > 0.06) return 1;
	else if (speed > 0.04) return 2;
	else return 3;
}

void puttSFX(float power, SoundEngine * s)
{
	s->getEngine()->play2D("sfx/retro_putt.wav");
}

void bounceSFX(float speed, SoundEngine * s)
{
	switch(speedTier(speed))
	{
	case 0 : s->getEngine()->play2D("sfx/retro_bounce_max.wav");
		break;
	case 1 : s->getEngine()->play2D("sfx/retro_bounce_high.wav");
		break;
	case 2 : s->getEngine()->play2D("sfx/retro_bounce_med.wav");
		break;
	case 3 : s->getEngine()->play2D("sfx/retro_bounce_low.wav");
		break;
	}
}

clock_t delay; //How many ticks to delay between plays of the rolling sound
clock_t elapsed;
Timer timer;

void rollSFX(float speed, SoundEngine * s)
{
	//delay = 10000;

	//if (elapsed >= delay)
	//{
	//	s->getEngine()->play2D("sfx/retro_roll.wav");
	//	elapsed = 0;
	//}

	//elapsed += timer.delta();
}
