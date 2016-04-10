#include "LedBar.hpp"

#include <cmath>
#include <iostream>

#include "../../Log/Logger.hpp"
#include "Actions.hpp"

using namespace std;

LedBar::LedBar(std::string botId, Actions & actions, int nbLed)
		: AActionsElement(actions), nbLed_(nbLed), actionStopped_(false), actionRunning_(false), position_(0), color_(LED_OFF), nb_(0), timeus_(0), hex_(0), hexNext_(0), botId_(botId)
{
	leddriver = ALedDriver::create(botId, nbLed);
}

LedBar::~LedBar()
{
	logger().debug() << "~LedBar()" << logs::end;
	delete leddriver;
}

LedBarAction::LedBarAction(LedBar & ledBar, LedBarActionName action)
		: ledBar_(ledBar), action_(action), lastTime_(0), i_(1), j_(1), k_(0), inc_(true)
{
	chrono_.start();
}

void LedBar::set(int pos, LedColor color)
{
	leddriver->setBit(pos, color);
}

void LedBar::rainbow(uint nb, uint timeus)
{
	for (uint i = 0; i < nb; i++)
	{
		for (int c = LED_OFF; c < LED_END; c++)
		{
			for (int k = 0; k <= nbLed_ - 1; k++)
			{
				set(k, (LedColor) c);
				set(k, (LedColor) c);
			}
			usleep(timeus);
		}
	}
}

void LedBar::blink(uint nb, uint timeus, LedColor color)
{
	for (uint i = 0; i < nb; i++)
	{
		resetAll();
		usleep(timeus);
		if (i >= nb - 1)
			break;

		flashAll(color);
		usleep(timeus);
	}
}

void LedBar::flash(uint hexPosition, LedColor color)
{
	leddriver->setBytes(hexPosition, color);
}

void LedBar::resetAll()
{
	flash(pow(2, nbLed_) - 1, LED_OFF);
}

void LedBar::flashAll(LedColor color)
{
	flash(pow(2, nbLed_) - 1, color);
}

void LedBar::blinkPin(uint nb, uint timeus, int position, LedColor color)
{
	logger().debug() << "blinkPin" << logs::end;
	for (uint i = 1; i <= nb; i++)
	{
		this->set(position, color);
		usleep(timeus);
		this->set(position, LED_OFF);
		i++;
		if (i > nb)
			break;
		usleep(timeus);
	}
}

void LedBar::alternate(uint nb, uint timeus, uint beginVal, uint endVal, LedColor beginColor)
{
	logger().debug() << "alternate" << logs::end;
	for (uint i = 1; i <= nb; i++)
	{
		flash(beginVal, beginColor);
		usleep(timeus);
		flash(endVal, LED_OFF);
		i++;
		if (i > nb)
			break;
		usleep(timeus);
	}
}

void LedBar::k2mil(uint nb, uint timeus, LedColor color)
{
	logger().debug() << "k2mil nbLed_=" << nbLed_ << logs::end;
	int j = -1;

	for (uint i = 1; i <= nb; i++)
	{
		//inc
		for (int k = 0; k <= nbLed_ - 1; k++)
		{
			set(k, color);
			if (j >= 0)
				if (j != k)
					set(j, LED_OFF);
			j = k;
			usleep(timeus);
		}
		//dec
		for (int k = nbLed_ - 1; k >= 0; k--)
		{
			set(k, color);
			if (j != k)
				set(j, LED_OFF);
			j = k;
			usleep(timeus);
		}
	}
	//off
	set(0, LED_OFF);
}

void LedBar::startSet(ushort index, LedColor color)
{
	logger().debug() << "startSet" << logs::end;
	this->actionStopped_ = false;
	this->position_ = index;
	this->color_ = color;
	//ajout de l'action de set dans la pile d'action
	this->actions().addAction(new LedBarAction(*this, LEDBARSET));

}

void LedBar::startReset()
{
	logger().debug() << "startReset" << logs::end;
	this->actionStopped_ = false;
	this->actions().addAction(new LedBarAction(*this, LEDBARRESET));
}

void LedBar::startFlash()
{
	logger().debug() << "startFlash" << logs::end;
	this->actionStopped_ = false;
	this->actions().addAction(new LedBarAction(*this, LEDBARFLASH));
}

void LedBar::startFlashValue(uint hexValue)
{
	logger().debug() << "startFlashValue" << logs::end;
	this->actionStopped_ = false;
	this->hex_ = hexValue;
	this->actions().addAction(new LedBarAction(*this, LEDBARFLASHVALUE));
}

void LedBar::startAlternate(uint nb, uint timeus, uint hexValue, uint hexValueNext, LedColor color, bool wait)
{
	logger().debug() << "startAlternate" << logs::end;
	while (this->actionRunning_)
	{
		usleep(1000);
	}
	this->actionStopped_ = false;
	this->nb_ = nb;
	this->timeus_ = timeus;
	this->hex_ = hexValue;
	this->hexNext_ = hexValueNext;
	this->color_ = color;
	this->actions().addAction(new LedBarAction(*this, LEDBARALTERNATE));
	if (wait)
	{
		while (!this->actionRunning_)
		{
			usleep(1000);
		}
		while (this->actionRunning_)
		{
			usleep(1000);
		}
	}
}

void LedBar::startBlink(uint nb, uint timeus, LedColor color, bool wait)
{
	logger().debug() << "startBlink before while" << logs::end;
	while (this->actionRunning_)
	{
		usleep(1000);
	}
	logger().debug() << "startBlink after while" << logs::end;
	this->actionStopped_ = false;
	this->nb_ = nb;
	this->timeus_ = timeus;
	this->color_ = color;
	this->actions().addAction(new LedBarAction(*this, LEDBARBLINK));
	if (wait)
	{
		logger().debug() << "while !this->actionRunning_..." << logs::end;
		while (!this->actionRunning_)
		{
			usleep(1000);

		}
		logger().debug() << "while this->actionRunning_..." << logs::end;
		while (this->actionRunning_)
		{
			usleep(1000);
		}
	}
}

void LedBar::startBlinkPin(uint nb, uint timeus, int position, LedColor color, bool wait)
{
	while (this->actionRunning_)
	{
		usleep(1000);
	}
	this->position_ = position;
	this->actionStopped_ = false;
	this->nb_ = nb;
	this->timeus_ = timeus;
	this->color_ = color;
	this->actions().addAction(new LedBarAction(*this, LEDBARBLINKPIN));
	if (wait)
	{
		while (!this->actionRunning_)
		{
			usleep(1000);
		}
		while (this->actionRunning_)
		{
			usleep(1000);
		}
	}
}

void LedBar::startK2mil(uint nb, uint timeus, LedColor color, bool wait)
{
	while (this->actionRunning_)
	{
		usleep(1000);
	}
	this->actionStopped_ = false;
	this->timeus_ = timeus;
	this->nb_ = nb;
	this->color_ = color;
	this->actions().addAction(new LedBarAction(*this, LEDBARK2MIL));

	if (wait)
	{
		while (!this->actionRunning_)
		{
			usleep(1000);
		}
		while (this->actionRunning_)
		{
			usleep(1000);
		}
	}
}

bool LedBarAction::execute()
{
	switch (this->action_)
	{
	case LEDBARSET:
		ledBar_.set(ledBar_.position(), ledBar_.color());
		return 0; //suppression de l'action
		break;

	case LEDBARRESET:
		ledBar_.resetAll();
		return 0; //suppression de l'action
		break;

	case LEDBARFLASH:
		ledBar_.flashAll();
		return 0; //suppression de l'action
		break;

	case LEDBARFLASHVALUE:
		ledBar_.flash(ledBar_.hexValue(), ledBar_.color());
		return 0; //suppression de l'action
		break;

	case LEDBARALTERNATE:
		ledBar_.actionRunning(true);
		if (chrono_.getElapsedTimeInMicroSec() >= lastTime_ + (long) ledBar_.timeus() || i_ == 1)
		{
			if ((i_ % 2))
			{
				ledBar_.flash(ledBar_.hexValue(), ledBar_.color());
			}
			else
			{
				ledBar_.flash(ledBar_.hexValueNext(), ledBar_.color());
			}
			i_++;
			lastTime_ = chrono_.getElapsedTimeInMicroSec();
		}
		if (i_ >= ledBar_.nb() + 1)
		{
			ledBar_.stop(true);
			ledBar_.nb(0);
		}
		if (ledBar_.stop())
			ledBar_.actionRunning(false);
		return !ledBar_.stop(); //renvoi 0 pour supprimer l'action
		break;

	case LEDBARBLINK:
		ledBar_.actionRunning(true);
		if (chrono_.getElapsedTimeInMicroSec() >= lastTime_ + (long) ledBar_.timeus() || i_ == 1)
		{
			//ledBar_.flash((255 * (i_ % 2)), ledBar_.color());
			ledBar_.flash(((std::pow(2, ledBar_.nbLed()) - 1) * (i_ % 2)), ledBar_.color());

			i_++;
			lastTime_ = chrono_.getElapsedTimeInMicroSec();
		}
		if (i_ >= ledBar_.nb() + 1)
		{
			ledBar_.stop(true);
			ledBar_.nb(0);
		}
		if (ledBar_.stop())
			ledBar_.actionRunning(false);
		return !ledBar_.stop(); //renvoi 0 pour supprimer l'action
		break;

	case LEDBARBLINKPIN:
		ledBar_.actionRunning(true);
		if (chrono_.getElapsedTimeInMicroSec() >= lastTime_ + (long) ledBar_.timeus() || i_ == 1)
		{
			LedColor temp;
			if (i_ % 2)
				temp = ledBar_.color();
			else
				temp = LED_OFF;

			ledBar_.set(ledBar_.position(), temp);
			i_++;

			lastTime_ = chrono_.getElapsedTimeInMicroSec();

		}
		if (i_ >= ledBar_.nb() + 1)
		{
			ledBar_.stop(true);
			ledBar_.nb(0);
		}
		if (ledBar_.stop())
			ledBar_.actionRunning(false);
		return !ledBar_.stop(); //renvoi 0 pour supprimer l'action
		break;

	case LEDBARK2MIL:
		ledBar_.actionRunning(true);
		if (chrono_.getElapsedTimeInMicroSec() >= lastTime_ + (long) ledBar_.timeus() || i_ == 1)
		{
			if (i_ >= ledBar_.nb() + 2)
			{
				ledBar_.stop(true);
				ledBar_.nb(0);
			}
			else
			{
				ledBar_.set(k_, ledBar_.color());
			}

			ledBar_.setOff(j_);

			if ((k_ == ledBar_.nbLed() - 1 && !inc_) || (k_ == 0 && inc_))
				i_++;
			j_ = k_;
			if (inc_)
			{
				k_++;
			}
			else
			{
				k_--;
			}
			if (k_ >= ledBar_.nbLed() - 1)
			{
				k_ = ledBar_.nbLed() - 1;
				inc_ = false;
			}
			if (k_ <= 0)
			{
				k_ = 0;
				inc_ = true;
			}
			lastTime_ = chrono_.getElapsedTimeInMicroSec();
		}

		if (ledBar_.stop())
			ledBar_.actionRunning(false);
		return !ledBar_.stop(); //renvoi 0 pour supprimer l'action
		break;

	default:
		logger().error() << "Bad execute command !!" << logs::end;
		return false;
		break;
	}
	return false;
}

