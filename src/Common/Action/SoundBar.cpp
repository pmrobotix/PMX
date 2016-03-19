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

SoundBarAction::SoundBarAction(SoundBar & soundBar, SoundBarActionName action, int volume) :
		soundBar_(soundBar), action_(action), volume_(volume)
{
}

void SoundBar::speakPMX(int volume)
{
	sounddriver->set_volume(volume);
	sounddriver->speak("Hello PMX");
	sleep(1);
	sounddriver->speak("How are you today ?", true);
}

void SoundBar::beep(int volume)
{
	sounddriver->set_volume(volume);
	sounddriver->beep();
	usleep(300000); //TODO ajout un parametre wait pour supprimer ce usleep si besoin
}

void SoundBar::startBeep(int volume)
{
	logger().debug() << "startBeep" << logs::end;
	this->actionStopped_ = false;
	//ajout de l'action de set dans la pile d'action
	this->actions().addAction(new SoundBarAction(*this, SOUNDBARBEEP, volume));
}

bool SoundBarAction::execute()
{

	switch (this->action_)
	{
	case SOUNDBARBEEP:
		logger().debug() << "SOUNDBARBEEP" << logs::end;
		soundBar_.beep(this->volume());
		return false; //suppression de l'action
		break;

	default:
		logger().error() << "Bad execute command !!" << logs::end;
		return false;
		break;
	}
	return false;
}
