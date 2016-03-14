#include "Simulator.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "BotManager.hpp"
#include "IWindow.hpp"
#include "LWindows.hpp"
#include "SDLTool.hpp"



using namespace std;

Simulator::Simulator()
{
	mainWindow = NULL;
	secondWindow = NULL;
	thirdWindow = NULL;
	botm = new BotManager();

	SDLTool::initSDLThread();
}


Simulator::~Simulator()
{
}

void Simulator::execute()
{

	botm->launchRobotThreads();


	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	bool allWindowsClosed = true;

	if (!initSDL())
	{
		logger().error() << "Error: Failed to initialize SDL!" << logs::end;
		//printf("Error: Failed to initialize SDL!\n");
		exit(-1);
	}

	if (!createWindows())
	{
		logger().error() << "Error: Failed to initialize windows!" << logs::end;
		//printf("Error: Failed to initialize windows!\n");
		exit(-1);
	}

	//While application is running
	while (!quit)
	{

		//execute les taches des drivers
		SDLTool::processSDLTasks();


		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			mainWindow->handleEvent(e);
			secondWindow->handleEvent(e);
			thirdWindow->handleEvent(e);

			//DemoButtons
			secondWindow->buttonHandleEvent(e);

			//Pull up window
			if (e.type == SDL_KEYDOWN)
			{
				//log key
				cout << "key pressed: " << e.key.keysym.sym << endl;

				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;

				case SDLK_KP_1: //touche 1 du pavé numérique
					mainWindow->focus();
					break;

				case SDLK_KP_2: //touche 2 du pavé numérique
					secondWindow->focus();
					break;

				case SDLK_KP_3: //touche 3 du pavé numérique
					thirdWindow->focus();
					break;
				}
			}
		}

		//Update all windows
		mainWindow->render();
		secondWindow->render();
		thirdWindow->render();

		if (mainWindow == NULL)
		{
			logger().error() << "Window mainWindow is NULL!" << logs::end;
			exit(-1);
		}
		else
		{
			if (mainWindow->isClosed())
			{
				quit = true;
				break;
			}
		}
		if (secondWindow == NULL)
		{
			logger().error() << "Window secondWindow is NULL!" << logs::end;
			exit(-1);
		}
		else
		{
			if (secondWindow->isClosed())
			{
				quit = true;
				break;
			}
		}

		if (thirdWindow == NULL)
		{
			logger().error() << "Window thirdWindow is NULL!" << logs::end;
			exit(-1);
		}
		else
		{
			if (thirdWindow->isClosed())
			{
				quit = true;
				break;
			}
		}

		//checked if all windows are minimized, if yes, close and quit

		if (mainWindow->isShown())
		{
			allWindowsClosed = false;
		}
		if (secondWindow->isShown())
		{
			allWindowsClosed = false;
		}
		if (thirdWindow->isShown())
		{
			allWindowsClosed = false;
		}

		if (allWindowsClosed)
		{
			quit = true;
		}
		usleep(2000); //todo framerate ?
	}

}

bool Simulator::initSDL()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);


	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		printf("Error: SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//SDL2_image : Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("Error: SDL_image could not initialize! SDL_image Error: %s\n",
			IMG_GetError());
			success = false;
		}

		//Initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("Error: SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	return success;
}

bool Simulator::createWindows()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);

	//Initialization flag
	bool success = true;

	//BotManager *botm = new BotManager();

	mainWindow = new MainWindow();
	mainWindow->focus();

	if (!mainWindow->initSDL(200, 0, 1000, 768, "Main Window",
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))
	{
		printf("Error: Window mainWindow could not be created!\n");
		success = false;
	}
	else
	{

		if (!mainWindow->loadContent(botm))
		{
			success = false;
			printf("Error: Window mainWindow could not load content!\n");
		}
	}

	secondWindow = new SecondWindow();
	if (!secondWindow->initSDL(0, 0, 200, 200, "Second Window",
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))
	{
		printf("Error: Window secondWindow could not be created!\n");
		success = false;
	}
	else
	{
		if (!secondWindow->loadContent(botm))
		{
			success = false;
			printf("Error: Window secondWindow could not load content!\n");
		}
	}

	thirdWindow = new ThirdWindow();
	if (!thirdWindow->initSDL(0, 250, 200, 200, "Third Window",
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))
	{
		printf("Error: Window thirdWindow could not be created!\n");
		success = false;
	}
	else
	{
		if (!thirdWindow->loadContent(botm))
		{
			success = false;
			printf("Error: Window thirdWindow could not load content!\n");
		}
	}

	return success;
}

void Simulator::closeSDL()
{
	mainWindow->free();
	secondWindow->free();
	thirdWindow->free();

//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
