#include "DemoButtons.hpp"

#include <SDL2/SDL_stdinc.h>
#include <cstdio>

//#include "LWindows.hpp"

DemoButtons::DemoButtons(Uint32 windowID)
{
	gButtons[0] = new LButton(windowID, DBUTTON_WIDTH, DBUTTON_HEIGHT);
	gButtons[1] = new LButton(windowID, DBUTTON_WIDTH, DBUTTON_HEIGHT);
	gButtons[2] = new LButton(windowID, DBUTTON_WIDTH, DBUTTON_HEIGHT);
	gButtons[3] = new LButton(windowID, DBUTTON_WIDTH, DBUTTON_HEIGHT);
}

DemoButtons::~DemoButtons()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();
}

bool DemoButtons::loadMediaButton(SDL_Renderer * renderer, std::string path)
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gButtonSpriteSheetTexture.loadFromFile(renderer, path))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * DBUTTON_HEIGHT;
			gSpriteClips[i].w = DBUTTON_WIDTH;
			gSpriteClips[i].h = DBUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[0]->setPosition(0, 0);
		gButtons[1]->setPosition(200 - DBUTTON_WIDTH, 0);
		gButtons[2]->setPosition(0, 200 - DBUTTON_HEIGHT);
		gButtons[3]->setPosition(200 - DBUTTON_WIDTH, 200 - DBUTTON_HEIGHT);

	}

	return success;
}

void DemoButtons::render(SDL_Renderer * renderer)
{
	//Render buttons
	for (int i = 0; i < DTOTAL_BUTTONS; ++i)
	{
		gButtons[i]->render(renderer, &gButtonSpriteSheetTexture, gSpriteClips);
	}
}

void DemoButtons::handleEvent(SDL_Event* e)
{
	for (int i = 0; i < DTOTAL_BUTTONS; ++i)
	{
		gButtons[i]->handleEvent(e);
	}
}

void DemoButtons::addListener(int i, IButtonListener *l)
{
	gButtons[i]->addListener(l);
}

