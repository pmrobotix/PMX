#ifndef OPOS6UL_SOUNDDRIVER_HPP_
#define OPOS6UL_SOUNDDRIVER_HPP_

#include <string>
#include <vector>

#include "../Common/Action.Driver/ASoundDriver.hpp"
#include "../Log/LoggerFactory.hpp"

class SoundDriver: public ASoundDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SoundDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SoundDriver.OPO");
		return instance;
	}

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
