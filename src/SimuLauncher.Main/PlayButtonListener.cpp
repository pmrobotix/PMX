#include "PlayButtonListener.hpp"

#include "BotManager.hpp"

PlayButtonListener::PlayButtonListener(BotManager * botm)
{
	botm_ = botm;
}

void PlayButtonListener::leftButtonPressed()
{
	//std::cout << "PlayButtonListener leftButtonPressed"  << std::endl;
}

void PlayButtonListener::rightButtonPressed()
{
	//std::cout << "PlayButtonListener rightButtonPressed" << std::endl;
}

void PlayButtonListener::leftButtonPressedAndRelease()
{
	//std::cout << "PlayButtonListener leftButtonPressedAndRelease"<< std::endl;


	botm_->start(true);
}

void PlayButtonListener::rightButtonPressedAndRelease()
{
	//std::cout << "PlayButtonListener rightButtonPressedAndRelease" << std::endl;
}
