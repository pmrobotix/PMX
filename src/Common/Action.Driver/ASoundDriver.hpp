#ifndef ASOUNDDRIVER_HPP_
#define ASOUNDDRIVER_HPP_

#include <string>

class ASoundDriver
{

public:
	/*!
	 * \brief ASoundDriver instance creation.
	 */
	static ASoundDriver * create();

	/*!
	 * \brief Beep.
	 */
	virtual void beep()=0;

	virtual void tone(unsigned frequency, unsigned ms) = 0;

	virtual void play(const std::string &soundfile, bool bSynchronous = false) = 0;

	virtual void speak(const std::string &text, bool bSynchronous = false) = 0;

	virtual unsigned volume() = 0;

	virtual void set_volume(unsigned) = 0;

	/*!
	 * \brief Destructor.
	 */
	virtual ~ASoundDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	ASoundDriver()
	{
	}

};

#endif
