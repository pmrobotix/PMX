/*
 * DemoButtons.hpp
 *
 *  Created on: 21 janv. 2016
 *      Author: pmx
 */

#ifndef SIMULAUNCHER_MAIN_DEMOBUTTONS_HPP_
#define SIMULAUNCHER_MAIN_DEMOBUTTONS_HPP_

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <string>

#include "LButton.hpp"
#include "LTexture.hpp"

const int DTOTAL_BUTTONS = 4;
const int DBUTTON_WIDTH = 100;
const int DBUTTON_HEIGHT = 66;

//The mouse button
class DemoButtons
{
public:
	//Initializes internal variables
	DemoButtons(Uint32 windowID);

	~DemoButtons();

	bool loadMediaButton(SDL_Renderer * renderer, std::string path);

	void render(SDL_Renderer * renderer);

	void handleEvent(SDL_Event * e);

	void addListener(int i, IButtonListener * l);

private:

	//Buttons objects
	LButton *gButtons[DTOTAL_BUTTONS];

	LTexture gButtonSpriteSheetTexture;

	//Mouse button sprites
	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

};

#endif /* SIMULAUNCHER_MAIN_DEMOBUTTONS_HPP_ */
