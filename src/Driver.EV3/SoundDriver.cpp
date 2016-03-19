//drivers...EV3

#include "SoundDriver.hpp"

#include "ev3dev.h"

#include <string>

using namespace std;

ASoundDriver * ASoundDriver::create()
{
	return new SoundDriver();
}

SoundDriver::SoundDriver()
{
}

SoundDriver::~SoundDriver()
{
}

void SoundDriver::beep()
{
	ev3dev::sound::beep();
}

void SoundDriver::tone(unsigned frequency, unsigned ms)
{
	ev3dev::sound::tone(frequency, ms);
}

void SoundDriver::play(const std::string &soundfile, bool bSynchronous = false)
{
	ev3dev::sound::play(soundfile, bSynchronous);
}

void SoundDriver::speak(const std::string &text, bool bSynchronous = false)
{
	ev3dev::sound::speak(text, bSynchronous);
}

unsigned int SoundDriver::volume()
{
	return ev3dev::sound::volume();
}

void SoundDriver::set_volume(unsigned int vol)
{
	ev3dev::sound::set_volume(vol);
}
