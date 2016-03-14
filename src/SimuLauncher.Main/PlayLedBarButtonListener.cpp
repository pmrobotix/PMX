#include <iostream>

#include "BotManager.hpp"
#include "PlayLedBarButtonListener.hpp"

PlayLedBarButtonListener::PlayLedBarButtonListener(BotManager * botm)
{
	botm_ = botm;
}

void PlayLedBarButtonListener::leftButtonPressed()
{
	//std::cout << "PlayButtonListener leftButtonPressed"  << std::endl;
}

void PlayLedBarButtonListener::rightButtonPressed()
{
	//std::cout << "PlayButtonListener rightButtonPressed" << std::endl;
}

void PlayLedBarButtonListener::leftButtonPressedAndRelease()
{
	//std::cout << "PlayButtonListener leftButtonPressedAndRelease"<< std::endl;
	botm_->launchLedBarTest();
}

void PlayLedBarButtonListener::rightButtonPressedAndRelease()
{
	//std::cout << "PlayButtonListener rightButtonPressedAndRelease" << std::endl;
}
