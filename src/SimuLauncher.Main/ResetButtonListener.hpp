
#ifndef SIMULAUNCHER_MAIN_RESETBUTTONLISTENER_HPP_
#define SIMULAUNCHER_MAIN_RESETBUTTONLISTENER_HPP_

#include "IButtonListener.hpp"

class BotManager;

class ResetButtonListener: public IButtonListener
{

public:

	ResetButtonListener(BotManager * botm);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ResetButtonListener()
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
