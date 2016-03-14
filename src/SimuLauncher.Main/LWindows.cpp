#include "LWindows.hpp"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <list>

#include "../Log/Logger.hpp"
#include "BotManager.hpp"
#include "Button.hpp"
#include "DemoButtons.hpp"
#include "PlayButtonListener.hpp"
#include "PlayLedBarButtonListener.hpp"
#include "ResetButtonListener.hpp"
#include "SDLTool.hpp"
#include "StopButtonListener.hpp"
#include "Text.hpp"

bool MainWindow::loadContent(BotManager *botm)
{
	bool success = true;

	if (!gBackgroundTextureMain.load(mRenderer, "./resources/ground_2016_1400x1027px.svg"))
	{
		logger().error() << "Error: Failed to load background texture image ground_2016_1400x1027px.svg!" << logs::end;
		success = false;
	}

	//text
	Text * text = new Text(50, 0, 0, 0);
	SDL_Color textColor =
	{ 100, 100, 100, 0 };
	text->loadMedia(mRenderer, "./resources/ttf/Verdana/verdanab.ttf", "DEMO", 28, textColor);
	this->addPanel(text);

	//reset button
	Button * resetButton = new Button(0, 80, 64, 64);
	resetButton->loadMediaButton(mRenderer, "./resources/IHM/orientation73_64_blue.png",
			"./resources/IHM/orientation73_64_blue.png",
			"./resources/IHM/orientation73_64_blue.png",
			"./resources/IHM/orientation73_64_blue.png");
	this->addPanel(resetButton);
	ResetButtonListener * resetListener = new ResetButtonListener(botm);
	resetButton->setButtonListener(resetListener);

	//Play button
	Button * playButton = new Button(70, 80, 64, 64);
	playButton->loadMediaButton(mRenderer, "./resources/IHM/play-button4_64_green.svg",
			"./resources/IHM/play-button4_64_green_over.svg",
			"./resources/IHM/play-button4_64_green.svg",
			"./resources/IHM/play-button4_64_green.svg");
	this->addPanel(playButton);
	PlayButtonListener * playListener = new PlayButtonListener(botm);
	playButton->setButtonListener(playListener);

	//Stop button
	Button * stopButton = new Button(140, 80, 64, 64);
	stopButton->loadMediaButton(mRenderer, "./resources/IHM/stop48_64_red.png",
			"./resources/IHM/stop48_64_red.png", "./resources/IHM/stop48_64_red.png",
			"./resources/IHM/stop48_64_red.png");
	this->addPanel(stopButton);
	StopButtonListener * stopListener = new StopButtonListener(botm);
	stopButton->setButtonListener(stopListener);

	return success;
}

void MainWindow::render()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	if (!isMinimized())
	{
		//Initialize renderer color
		SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(mRenderer);
		SDL_Rect stretchRect;
		stretchRect =
		//{	0, 0, getWidth(), getHeight()};
		{	0, 40, 1400, 1027}; //rect dans l'image
		//gBackgroundTextureMain.setAlpha(100);
		gBackgroundTextureMain.render(mRenderer, 150, 0, &stretchRect);

		for (std::list<Panel*>::iterator iter = pList_->begin(); iter != pList_->end(); iter++)
		{
			Panel *p = (*iter);
			p->render(mRenderer);
		}
	}
	//Update screen
	SDL_RenderPresent(mRenderer);
}

bool SecondWindow::loadContent(BotManager *)
{
	bool success = true;

	dbuttons = new DemoButtons(this->mWindowID);

	if (!gBackgroundTextureMainWin1.loadFromFile(mRenderer, "./resources/background.png"))
	{
		logger().error() << "Error: Failed to load background texture image!" << logs::end;
		success = false;
	}
	if (!dbuttons->loadMediaButton(mRenderer, "./resources/button100.png"))
	{
		logger().error() << "Error: Failed to load media!" << logs::end;
		success = false;
	}
	return success;
}

void SecondWindow::render()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	if (dbuttons != NULL) dbuttons->render(mRenderer);

	for (std::list<Panel*>::iterator iter = pList_->begin(); iter != pList_->end(); iter++)
	{
		Panel *p = (*iter);
		p->render(mRenderer);
	}

	//Update screen
	SDL_RenderPresent(mRenderer);
}

void SecondWindow::buttonHandleEvent(SDL_Event& e)
{
	dbuttons->handleEvent(&e);
}

bool ThirdWindow::loadContent(BotManager *botm)
{
	bool success = true;

	//Play button for LedBartest
	Button * playButton = new Button(70, 00, 64, 64);
	playButton->loadMediaButton(mRenderer, "./resources/IHM/play-button4_64_green.svg",
			"./resources/IHM/play-button4_64_green_over.svg",
			"./resources/IHM/play-button4_64_green.svg",
			"./resources/IHM/play-button4_64_green.svg");
	this->addPanel(playButton);
	PlayLedBarButtonListener * playListener = new PlayLedBarButtonListener(botm);
	playButton->setButtonListener(playListener);

	return success;
}

void ThirdWindow::render()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	for (std::list<Panel*>::iterator iter = pList_->begin(); iter != pList_->end(); iter++)
	{
		Panel *p = (*iter);
		p->render(mRenderer);
	}

	//Update screen
	SDL_RenderPresent(mRenderer);
}
