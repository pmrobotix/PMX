//drivers...ARM

#include "SoundDriver.hpp"

#include <vector>

#include "../Log/Logger.hpp"

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
	logger().info() << "BEEP" << logs::end;
}

void SoundDriver::tone(unsigned frequency, unsigned ms, bool bSynchronous = true)
{
	logger().info() << "TONE f=" << frequency << " ms=" << ms << logs::end;
}

void SoundDriver::tone(const std::vector<std::vector<float> > &sequence, bool bSynchronous = true)
{
	logger().info() << "TONE vector size=" << sequence.size() << logs::end;
}

void SoundDriver::play(const std::string &soundfile, bool bSynchronous = true)
{
	logger().info() << "PLAY " << soundfile << logs::end;
}

void SoundDriver::speak(const std::string &text, bool bSynchronous = true)
{
	logger().info() << "SPEAK: " << text << logs::end;
}

