#ifndef OPOS6UL_BUTTONDRIVER_HPP_
#define OPOS6UL_BUTTONDRIVER_HPP_

#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "Adafruit_RGBLCDShield.hpp"

class ButtonDriver: public AButtonDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ButtonDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ButtonDriver.OPO");
		return instance;
	}


public:

	/*!
	 * \brief Constructor.
	 */
	ButtonDriver();

	/*!
	 * \brief Destructor.
	 */
	~ButtonDriver();

	bool pressed(ButtonTouch button);

};

#endif
