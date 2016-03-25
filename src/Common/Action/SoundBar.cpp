#include "SoundBar.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Action.Driver/ASoundDriver.hpp"
#include "Actions.hpp"

using namespace std;

SoundBar::SoundBar(Actions & actions) :
		AActionsElement(actions), actionStopped_(false)
{
	sounddriver = ASoundDriver::create();
}

SoundBar::~SoundBar()
{
	delete sounddriver;
}

SoundBarAction::SoundBarAction(SoundBar & soundBar, SoundBarActionName action) :
		soundBar_(soundBar), action_(action)
{
}

void SoundBar::speakPMX()
{
	sounddriver->speak("Hello PMX", true);
	sleep(1);
	sounddriver->speak("How are you today ?", true);
}

void SoundBar::beep()
{

	sounddriver->beep("", true);

}

void SoundBar::startBeep()
{
	logger().debug() << "startBeep" << logs::end;
	this->actionStopped_ = false;
	//ajout de l'action de set dans la pile d'action
	this->actions().addAction(new SoundBarAction(*this, SOUNDBARBEEP));
}

bool SoundBarAction::execute()
{

	switch (this->action_)
	{
	case SOUNDBARBEEP:
		logger().debug() << "SOUNDBARBEEP" << logs::end;
		soundBar_.beep();
		return false; //suppression de l'action
		break;

	default:
		logger().error() << "Bad execute command !!" << logs::end;
		return false;
		break;
	}
	return false;
}
