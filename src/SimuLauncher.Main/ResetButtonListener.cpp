#include <iostream>

#include "BotManager.hpp"
#include "ResetButtonListener.hpp"

ResetButtonListener::ResetButtonListener(BotManager * botm)
{
	botm_ = botm;
}

void ResetButtonListener::leftButtonPressed()
{
	//std::cout << "ResetButtonListener leftButtonPressed"  << std::endl;
}

void ResetButtonListener::rightButtonPressed()
{
	//std::cout << "ResetButtonListener rightButtonPressed" << std::endl;
}

void ResetButtonListener::leftButtonPressedAndRelease()
{
	//std::cout << "ResetButtonListener leftButtonPressedAndRelease"<< std::endl;
	botm_->reset();
}

void ResetButtonListener::rightButtonPressedAndRelease()
{
	//std::cout << "ResetButtonListener rightButtonPressedAndRelease" << std::endl;
}
