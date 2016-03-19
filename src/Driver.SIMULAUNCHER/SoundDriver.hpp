#ifndef SIMU_SOUNDDRIVER_HPP_
#define SIMU_SOUNDDRIVER_HPP_

#include <string>

#include "../Common/Action.Driver/ASoundDriver.hpp"
#include "../Log/LoggerFactory.hpp"

class SoundDriver: public ASoundDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SoundDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SoundDriver.SIMU");
		return instance;
	}

	int vol_;

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
