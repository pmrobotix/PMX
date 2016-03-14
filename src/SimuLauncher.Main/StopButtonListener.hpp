
#ifndef SIMULAUNCHER_MAIN_STOPBUTTONLISTENER_HPP_
#define SIMULAUNCHER_MAIN_STOPBUTTONLISTENER_HPP_

#include "IButtonListener.hpp"

class BotManager;

class StopButtonListener: public IButtonListener
{

public:

	StopButtonListener(BotManager * botm);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~StopButtonListener()
	{
	}

	virtual void leftButtonPressed();
	virtual void rightButtonPressed();
	virtual void leftButtonPressedAndRelease();
	virtual void rightButtonPressedAndRelease();

private:

	BotManager * botm_;

};

#endif
