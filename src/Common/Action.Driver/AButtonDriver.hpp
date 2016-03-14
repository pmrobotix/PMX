#ifndef ABUTTONDRIVER_HPP_
#define ABUTTONDRIVER_HPP_

#include "../Utils/Macro.hpp"

/*!
 * \brief Enumération des touches.
 */
enum ButtonTouch
{
	BUTTON_ENTER_KEY,
	BUTTON_BACK_KEY,
	BUTTON_UP_KEY,
	BUTTON_DOWN_KEY,
	BUTTON_LEFT_KEY,
	BUTTON_RIGHT_KEY
};

class AButtonDriver
{

public:
	/*!
	 * \brief ButtonDriver instance creation.
	 */
	static AButtonDriver * create();

	/*
	 *\brief pressed.
	 */
	virtual bool pressed(ButtonTouch button) = 0;

	/*!
	 * \brief Destructor.
	 */
	virtual ~AButtonDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	AButtonDriver()
	{
	}

};

#endif
