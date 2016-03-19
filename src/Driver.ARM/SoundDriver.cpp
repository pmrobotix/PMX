//drivers...ARM

#include "SoundDriver.hpp"

#include <string>

using namespace std;

ASoundDriver * ASoundDriver::create()
{
	return new SoundDriver();
}

SoundDriver::SoundDriver()
{
	vol_ = 5;
}

SoundDriver::~SoundDriver()
{
}

void SoundDriver::beep()
{
	logger().info() << "BEEP" << logs::end;
}

void SoundDriver::tone(unsigned frequency, unsigned ms)
{
	logger().info() << "TONE f=" << frequency << " ms=" << ms << logs::end;
}

void SoundDriver::play(const std::string &soundfile, bool bSynchronous = false)
{
	logger().info() << "PLAY " << soundfile << logs::end;
}

void SoundDriver::speak(const std::string &text, bool bSynchronous = false)
{
	logger().info() << "SPEAK: " << text << logs::end;
}

unsigned int SoundDriver::volume()
{
	return vol_;
}

void SoundDriver::set_volume(unsigned int volume)
{
	vol_ = volume;
	logger().info() << "VOLUME=" << vol_ << logs::end;
}
