#ifndef EV3_LEDDRIVER_HPP_
#define EV3_LEDDRIVER_HPP_

#include <sys/types.h>

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/LoggerFactory.hpp"

class LedDriver: public ALedDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriver.EV3");
		return instance;
	}

	int nb_;

public:

	void setBit(int index, LedColor color);

	void setBytes(uint hex, LedColor color);

	/*!
	 * \brief Constructor.
	 */
	LedDriver(int nb);

	/*!
	 * \brief Destructor.
	 */
	~LedDriver();

};

#endif
