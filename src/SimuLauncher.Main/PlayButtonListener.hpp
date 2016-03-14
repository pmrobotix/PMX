
#ifndef SIMULAUNCHER_MAIN_PLAYBUTTONLISTENER_HPP_
#define SIMULAUNCHER_MAIN_PLAYBUTTONLISTENER_HPP_

#include "IButtonListener.hpp"

class BotManager;

class PlayButtonListener: public IButtonListener
{

public:

	PlayButtonListener(BotManager * botm);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~PlayButtonListener()
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
