//drivers...EV3

#include "SoundDriver.hpp"

#include "ev3dev.h"
#include <vector>
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

void SoundDriver::beep(const std::string &args, bool bSynchronous = true)
{
    ev3dev::sound::beep(args, bSynchronous);
}

void SoundDriver::tone(unsigned frequency, unsigned ms, bool bSynchronous = true)
{
    ev3dev::sound::tone(frequency, ms, bSynchronous);
}

void SoundDriver::tone(const std::vector<std::vector<float> > &sequence, bool bSynchronous = true)
{
    ev3dev::sound::tone(sequence, bSynchronous);
}

void SoundDriver::play(const std::string &soundfile, bool bSynchronous = true)
{
    ev3dev::sound::play(soundfile, bSynchronous);
}

void SoundDriver::speak(const std::string &text, bool bSynchronous = true)
{
    ev3dev::sound::speak(text, bSynchronous);
}

