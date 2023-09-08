#include "SoundBar.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Interface.Driver/ASoundDriver.hpp"
#include "Actions.hpp"

using namespace std;

SoundBar::SoundBar(Actions & actions) :
        AActionsElement(actions), actionStopped_(false)
{
    sounddriver_ = ASoundDriver::create();
}

SoundBar::~SoundBar()
{
    delete sounddriver_;
}

SoundBarAction::SoundBarAction(SoundBar & soundBar, SoundBarActionName action) :
        soundBar_(soundBar), action_(action)
{
}

void SoundBar::toneImperialMarchDarthVader()
{
    sounddriver_->tone(
	{
	{ 392, 350, 100 },
	{ 392, 350, 100 },
	{ 392, 350, 100 },
	{ 311.1, 250, 100 },
	{ 466.2, 25, 100 },
	{ 392, 350, 100 },
	{ 311.1, 250, 100 },
	{ 466.2, 25, 100 },
	{ 392, 700, 100 },
	{ 587.32, 350, 100 },
	{ 587.32, 350, 100 },
	{ 587.32, 350, 100 },
	{ 622.26, 250, 100 },
	{ 466.2, 25, 100 },
	{ 369.99, 350, 100 },
	{ 311.1, 250, 100 },
	{ 466.2, 25, 100 },
	{ 392, 700, 100 },
	{ 784, 350, 100 },
	{ 392, 250, 100 },
	{ 392, 25, 100 },
	{ 784, 350, 100 },
	{ 739.98, 250, 100 },
	{ 698.46, 25, 100 },
	{ 659.26, 25, 100 },
	{ 622.26, 25, 100 },
	{ 659.26, 50, 400 },
	{ 415.3, 25, 200 },
	{ 554.36, 350, 100 },
	{ 523.25, 250, 100 },
	{ 493.88, 25, 100 },
	{ 466.16, 25, 100 },
	{ 440, 25, 100 },
	{ 466.16, 50, 400 },
	{ 311.13, 25, 200 },
	{ 369.99, 350, 100 },
	{ 311.13, 250, 100 },
	{ 392, 25, 100 },
	{ 466.16, 350, 100 },
	{ 392, 250, 100 },
	{ 466.16, 25, 100 },
	{ 587.32, 700, 100 },
	{ 784, 350, 100 },
	{ 392, 250, 100 },
	{ 392, 25, 100 },
	{ 784, 350, 100 },
	{ 739.98, 250, 100 },
	{ 698.46, 25, 100 },
	{ 659.26, 25, 100 },
	{ 622.26, 25, 100 },
	{ 659.26, 50, 400 },
	{ 415.3, 25, 200 },
	{ 554.36, 350, 100 },
	{ 523.25, 250, 100 },
	{ 493.88, 25, 100 },
	{ 466.16, 25, 100 },
	{ 440, 25, 100 },
	{ 466.16, 50, 400 },
	{ 311.13, 25, 200 },
	{ 392, 350, 100 },
	{ 311.13, 250, 100 },
	{ 466.16, 25, 100 },
	{ 392.00, 300, 150 },
	{ 311.13, 250, 100 },
	{ 466.16, 25, 100 },
	{ 392, 700 } }, true);
}

void SoundBar::speakPMX()
{
    sounddriver_->speak("Hello PMX", true);
    sleep(1);
    sounddriver_->speak("How are you today ?", true);
}

void SoundBar::beep()
{
    sounddriver_->beep("", true);
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

    switch (this->action_) {
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
