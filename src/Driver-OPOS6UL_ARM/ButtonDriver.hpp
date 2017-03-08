#ifndef OPOS_BUTTONDRIVER_HPP_
#define OPOS_BUTTONDRIVER_HPP_

#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "Adafruit_RGBLCDShield.hpp"

class ButtonDriver: public AButtonDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ButtonDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ButtonDriver.SIMU");
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
	virtual ~ButtonDriver();

	virtual bool pressed(ButtonTouch button);

};

#endif
