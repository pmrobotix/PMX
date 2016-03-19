#ifndef EV3_SOUNDDRIVER_HPP_
#define EV3_SOUNDDRIVER_HPP_

#include <string>

#include "../Common/Action.Driver/ASoundDriver.hpp"

class SoundDriver: public ASoundDriver
{

public:

	virtual void beep();


	virtual void tone(unsigned frequency, unsigned ms);

	virtual void play(const std::string &soundfile, bool bSynchronous);

	virtual void speak(const std::string &text, bool bSynchronous);

	virtual unsigned int volume();

	virtual void set_volume(unsigned);

	/*!
	 * \brief Constructor.
	 */
	SoundDriver();

	/*!
	 * \brief Destructor.
	 */
	virtual ~SoundDriver();

};

#endif
