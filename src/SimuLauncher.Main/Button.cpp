/*
 * Button.cpp
 *
 *  Created on: 8 févr. 2016
 *      Author: pmx
 */

#include "Button.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "IButtonListener.hpp"

Button::Button(int x, int y, int w, int h)
		: Panel(x, y, w, h) //on appelle le constructeur pere
{
	listener_ = NULL;
	pressleft_ = false;
	pressright_ = false;
	currentSprite_ = BUTTON_SPRITE_MOUSE_OUT_;
}

void Button::handleEvent(SDL_Event& e, int xMouse, int yMouse) //dans le repere du button
{
	if (xMouse >= 0 && xMouse <= (this->getW()) && yMouse >= 0 && yMouse <= (this->getH()))
	{
		//Set mouse over sprite
		switch (e.type)
		{
			case SDL_MOUSEMOTION:
				currentSprite_ = BUTTON_SPRITE_MOUSE_OVER_MOTION_;
				break;

			case SDL_MOUSEBUTTONDOWN:
				currentSprite_ = BUTTON_SPRITE_MOUSE_DOWN_;
				break;

			case SDL_MOUSEBUTTONUP:
				currentSprite_ = BUTTON_SPRITE_MOUSE_UP_;
				break;

			default:
				break;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				if (listener_ != NULL)
					listener_->leftButtonPressed();
				pressleft_ = true;
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				if (listener_ != NULL)
					listener_->rightButtonPressed();
				pressright_ = true;
			}
		}
		if (e.type == SDL_MOUSEBUTTONUP)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				if (pressleft_)
				{
					if (listener_ != NULL)
						listener_->leftButtonPressedAndRelease();
					pressleft_ = false;
				}
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				if (pressright_)
				{
					if (listener_ != NULL)
						listener_->rightButtonPressedAndRelease();
					pressright_ = false;
				}
			}
		}
	}
	else
	{
		currentSprite_ = BUTTON_SPRITE_MOUSE_OUT_;
	}
}

void Button::render(SDL_Renderer * renderer)
{

	switch (currentSprite_)
	{
		case BUTTON_SPRITE_MOUSE_OUT_:
			mouseOutSpriteSheetTexture_.render(renderer, x_, y_, &spriteClip_);
			break;
		case BUTTON_SPRITE_MOUSE_OVER_MOTION_:
			mouseOverSpriteSheetTexture_.render(renderer, x_, y_, &spriteClip_);
			break;
		case BUTTON_SPRITE_MOUSE_DOWN_:
			mouseDownSpriteSheetTexture_.render(renderer, x_, y_, &spriteClip_);
			break;
		case BUTTON_SPRITE_MOUSE_UP_:
			mouseUpSpriteSheetTexture_.render(renderer, x_, y_, &spriteClip_);
			break;
		default:
			break;
	}
}

bool Button::loadMediaButton(SDL_Renderer * renderer,
		std::string pathMouseOut,
		std::string pathMouseOver,
		std::string pathMouseDown,
		std::string pathMouseUp)
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!mouseOutSpriteSheetTexture_.load(renderer, pathMouseOut))
	{
		logger().error() << "Failed to load mouseOutSpriteSheetTexture_ !" << logs::end;
		success = false;
	}

	if (!mouseOverSpriteSheetTexture_.load(renderer, pathMouseOver))
	{
		logger().error() << "Failed to load mouseOverSpriteSheetTexture_ !" << logs::end;
		success = false;
	}

	if (!mouseDownSpriteSheetTexture_.load(renderer, pathMouseDown))
	{
		logger().error() << "Failed to load mouseDownSpriteSheetTexture_ !" << logs::end;
		success = false;
	}

	if (!mouseUpSpriteSheetTexture_.load(renderer, pathMouseUp))
	{
		logger().error() << "Failed to load mouseUpSpriteSheetTexture_ !" << logs::end;
		success = false;
	}

	//Set sprites
	spriteClip_.x = 0;
	spriteClip_.y = 0;
	spriteClip_.w = w_; //mouseOutSpriteSheetTexture_.getWidth();
	spriteClip_.h = h_; //mouseOutSpriteSheetTexture_.getHeight();

	return success;
}

