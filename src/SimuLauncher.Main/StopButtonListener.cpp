#include "StopButtonListener.hpp"

#include <iostream>

#include "BotManager.hpp"

StopButtonListener::StopButtonListener(BotManager * botm)
{
	botm_ = botm;
}

void StopButtonListener::leftButtonPressed()
{
	//std::cout << "StopButtonListener leftButtonPressed"  << std::endl;
}

void StopButtonListener::rightButtonPressed()
{
	//std::cout << "StopButtonListener rightButtonPressed" << std::endl;
}

void StopButtonListener::leftButtonPressedAndRelease()
{
	//std::cout << "StopButtonListener leftButtonPressedAndRelease"<< std::endl;
	botm_->stop(true);
}

void StopButtonListener::rightButtonPressedAndRelease()
{
	//std::cout << "StopButtonListener rightButtonPressedAndRelease" << std::endl;
}
