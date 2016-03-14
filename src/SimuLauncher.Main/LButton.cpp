#include "LButton.hpp"


#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

#include "../Log/Logger.hpp"
#include "IButtonListener.hpp"
#include "LTexture.hpp"

LButton::LButton(Uint32 windowID, int bWidth, int bHeight)
{
	mPosition.x = 0;
	mPosition.y = 0;
	width_ = bWidth;
	height_ = bHeight;
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	binside = 0;
	bplay = 0;
	downup = 0;

	listener = NULL;
	windowID_ = windowID;
}

LButton::~LButton()
{
}

void LButton::addListener(IButtonListener * l)
{
	listener = l;

}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	int x, y;
	//Check if mouse is in button
	bool inside = true;

	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN
			|| e->type == SDL_MOUSEBUTTONUP)
	{

		//Get mouse position
		SDL_GetMouseState(&x, &y);

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + width_)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + height_)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
			binside = 0;
			bplay = 0;
		}
		//Mouse is inside button
		else
		{

			//std::cout << "inside " << e->window.windowID << " windowID_=" << windowID_<< std::endl;
			//std::cout << "e->window.windowID=" << e->window.windowID << std::endl;
			if (e->window.windowID == windowID_)
			{
				binside = 1;
				//Set mouse over sprite
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;

				case SDL_MOUSEBUTTONDOWN:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					break;
				}

				if (e->type == SDL_MOUSEBUTTONDOWN)
				{
					if (e->button.button == SDL_BUTTON_LEFT)
					{
						downup = 1;
					}
				}
				if ((e->type == SDL_MOUSEBUTTONUP) && downup)
				{
					//std::cout << "e->type == SDL_MOUSEBUTTONUP & downup)" << downup << std::endl;
					if (e->button.button == SDL_BUTTON_LEFT)
					{
						bplay = 1;
						if (listener != NULL)
						{
							listener->leftButtonPressed();
						}
						else
						{
							logger().error() << "error listener NULL" << logs::end;
						}
						downup = 0;

					}
				}
			}
		}
	}
}

void LButton::render(SDL_Renderer * renderer, LTexture * texture, SDL_Rect * rect)
{
	//Show current button sprite
	texture->render(renderer, mPosition.x, mPosition.y, &rect[mCurrentSprite]);
}
