#ifndef LCDSHIELD_HPP_
#define LCDSHIELD_HPP_

#include <stddef.h>
#include <cstdint>
#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Print.hpp"
#include "AActionsElement.hpp"

class ALcdShieldDriver;

class LcdShield: public AActionsElement, public Print
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LcdShield.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LcdShield");
		return instance;
	}

public:

	/*!
	 * \brief ID du robot.
	 */
	std::string botId_;

	ALcdShieldDriver* lcdshielddriver;

	/*!
	 * \brief Constructor.
	 *
	 * \param nb Number of leds in the LedBar.
	 */
	LcdShield(std::string botId, Actions & actions);

	void init();
	void reset();

	void clear();
	void home();
	void setBacklightOn();
	void setBacklightOff();

	void setCursor(uint8_t col, uint8_t row);

	/*
	void noDisplay();
	void display();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void leftToRight();
	void rightToLeft();
	void autoscroll();
	void noAutoscroll();
	 */

	virtual size_t write(uint8_t);


	/*!
	 * \brief Destructor.
	 */
	~LcdShield();

};

#endif
