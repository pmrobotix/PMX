#ifndef EV3_SOUNDDRIVER_HPP_
#define EV3_SOUNDDRIVER_HPP_

#include <string>
#include <vector>

#include "../Common/Action.Driver/ASoundDriver.hpp"

class SoundDriver: public ASoundDriver
{

public:

	virtual void beep(const std::string &args, bool bSynchronous);

	virtual void tone(unsigned frequency, unsigned ms, bool bSynchronous);

	virtual void tone(const std::vector<std::vector<float> > &sequence, bool bSynchronous);

	virtual void play(const std::string &soundfile, bool bSynchronous);

	virtual void speak(const std::string &text, bool bSynchronous);

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
