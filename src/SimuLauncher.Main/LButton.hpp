#ifndef SIMULAUNCHER_MAIN_LBUTTON_HPP_
#define SIMULAUNCHER_MAIN_LBUTTON_HPP_

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>

#include "../Log/LoggerFactory.hpp"

class IButtonListener;



class LTexture;
struct SDL_Renderer;

union SDL_Event;

//Button constants
//const int BUTTON_WIDTH = 300;
//const int BUTTON_HEIGHT = 200;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//The mouse button
class LButton
{

private:
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LButton");
		return instance;
	}

	Uint32 windowID_;
	int width_;

	int height_;

	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

public:
	//Initializes internal variables
	LButton(Uint32 id, int bWidth, int bHeight);

	~LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render(SDL_Renderer * renderer, LTexture * texture, SDL_Rect * rect);

	void addListener(IButtonListener * l);

	IButtonListener *listener;

	int binside;
	int bplay;
	int downup;

};

#endif /* SIMULAUNCHER_MAIN_LBUTTON_HPP_ */
