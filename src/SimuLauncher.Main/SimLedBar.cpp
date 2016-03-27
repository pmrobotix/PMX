/*
 * SimLedBar.cpp
 *
 *  Created on: 2 mars 2016
 *      Author: pmx
 */

#include "SimLedBar.hpp"

#include <cstdio>

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "LTexture.hpp"

struct SDL_Renderer;

SimLedBar::SimLedBar(int x, int y, int w, int h, int nb)
		: Panel(x, y, w, h) //on appelle le constructeur pere
{
	nb_ = nb;
	currentTexture_ = new LTexture[nb_];
}

void SimLedBar::setBit(int i, LedColor color)
{
	if (i < 0 || i >= nb_)
	{
		logger().error() << "Error SimLedBar::setBytes no color " << i << logs::end;
		//printf("Error SimLedBar::setBit invalid i= %d", i);
		exit(-1);
	}

	switch (color)
	{
	case LED_OFF:
	case LED_END:
		currentTexture_[i] = offTexture_;
		break;
	case LED_GREEN:
		currentTexture_[i] = greenTexture_;
		break;
	case LED_ORANGE:
		currentTexture_[i] = orangeTexture_;
		break;
	case LED_RED:
		currentTexture_[i] = redTexture_;
		break;
	case LED_AMBER:
		currentTexture_[i] = orangeTexture_;
		break;
	case LED_YELLOW:
		currentTexture_[i] = orangeTexture_;
		break;
	default:
		logger().error() << "Error SimLedBar::setBytes no color " << i << logs::end;
		//printf("Error SimLedBar::setBit no color %d", i);
		exit(-1);
	}
}

void SimLedBar::setBytes(uint hex, LedColor color)
{

	for (int i = 0; i < nb_; i++)
	{
		if (((hex >> i) & 0x01) == 1)
		{
			switch (color)
			{
			case LED_OFF:
			case LED_END:
				currentTexture_[i] = offTexture_;
				break;
			case LED_GREEN:
				currentTexture_[i] = greenTexture_;
				break;
			case LED_ORANGE:
				currentTexture_[i] = orangeTexture_;
				break;
			case LED_RED:
				currentTexture_[i] = redTexture_;
				break;
			case LED_AMBER:
				currentTexture_[i] = orangeTexture_;
				break;
			case LED_YELLOW:
				currentTexture_[i] = orangeTexture_;
				break;
			default:
				logger().error() << "Error SimLedBar::setBytes no color " << i << logs::end;
				//printf("Error SimLedBar::setBytes no color %d\n", i);
				exit(-1);
			}
		}
		else
		{
			currentTexture_[i] = offTexture_;
		}
	}
}

void SimLedBar::handleEvent(SDL_Event&, int, int)
{
}

void SimLedBar::render(SDL_Renderer * renderer)
{
	for (int i = 0; i < nb_; ++i)
	{
		currentTexture_[i].render(renderer, x_ + (i * 20), y_, &spriteClip_);
	}
}

bool SimLedBar::loadMedia(SDL_Renderer * renderer,
		std::string pathOff,
		std::string pathGreen,
		std::string pathRed,
		std::string pathOrange)
{
	//Loading success flag
	bool success = true;

	if (!offTexture_.load(renderer, pathOff))
	{
		logger().error() << "Error : Failed to load offTexture_ !" << logs::end;
		success = false;
	}
	if (!greenTexture_.load(renderer, pathGreen))
	{
		logger().error() << "Error : Failed to load greenTexture_ !" << logs::end;
		success = false;
	}
	if (!orangeTexture_.load(renderer, pathOrange))
	{
		logger().error() << "Error : Failed to load orangeTexture_ !" << logs::end;
		success = false;
	}
	if (!redTexture_.load(renderer, pathRed))
	{
		logger().error() << "Error : Failed to load redTexture_ !" << logs::end;
		success = false;
	}

	//Load sprites
	for (int i = 0; i < nb_; ++i)
	{
		currentTexture_[i] = offTexture_;
	}

	//Set sprites
	spriteClip_.x = 0;
	spriteClip_.y = 0;
	spriteClip_.w = w_;
	spriteClip_.h = h_;

	return success;
}
