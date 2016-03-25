#ifndef ALEDDRIVER_HPP_
#define ALEDDRIVER_HPP_

#include <string>

#include "../Utils/Macro.hpp"

/*!
 * \brief Enumeration of the different color of leds.
 */
enum LedColor
{
	/*!
	 * \brief color LED OFF.
	 */
	LED_OFF,
	/*!
	 * \brief led color green.
	 */
	LED_GREEN,
	/*!
	 * \brief led color red.
	 */
	LED_RED,
	/*!
	 * \brief led color orange.
	 */
	LED_ORANGE,

	/*!
	 * \brief led color amber.
	 */
	LED_AMBER,

	/*!
	 * \brief led color yellow.
	 */
	LED_YELLOW,

	LED_END

};

class ALedDriver
{

public:
	/*!
	 * \brief LedDriver instance creation.
	 * \param botName
	 * 			Name of the robot (for simulator).
	 * \param nb
	 * 			number of Leds in the LedBar.
	 *
	 */
	static ALedDriver * create(std::string botName, int nb);

	/*!
	 * \brief Set led value.
	 *
	 * \param index
	 * 				The position of the led in the ledBar (between 0 to nb).
	 * \param color
	 * 				The color of the led.
	 */
	virtual void setBit(int index, LedColor color) = 0;

	/*!
	 * \brief Set color where hex=1. Set 0 where hex=0.
	 *
	 * \param hexPosition
	 * 				The values of the whole ledbar.
	 * \param color
	 * 				The color of the led setted to 1 in hex.
	 */
	virtual void setBytes(uint hex, LedColor color) = 0;

	/*!
	 * \brief Destructor.
	 */
	virtual ~ALedDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	ALedDriver()
	{
	}

};

#endif
