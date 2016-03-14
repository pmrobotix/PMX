
#ifndef SIMULAUNCHER_MAIN_PLAYLEDBARBUTTONLISTENER_HPP_
#define SIMULAUNCHER_MAIN_PLAYLEDBARBUTTONLISTENER_HPP_

#include "IButtonListener.hpp"

class BotManager;

class PlayLedBarButtonListener: public IButtonListener
{

public:

	PlayLedBarButtonListener(BotManager * botm);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~PlayLedBarButtonListener()
	{
	}

	virtual void leftButtonPressed();
	virtual void rightButtonPressed();
	virtual void leftButtonPressedAndRelease();
	virtual void rightButtonPressedAndRelease();

private:

	BotManager * botm_;

};

#endif /* SIMULAUNCHER_MAIN_PLAYBUTTONLISTENER_HPP_ */
