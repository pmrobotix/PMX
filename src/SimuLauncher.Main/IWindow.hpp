/*!
 * \file
 * \brief Définition de l'interface IWindow.
 */

#ifndef SIMULAUNCHER_MAIN_IWINDOW_HPP_
#define SIMULAUNCHER_MAIN_IWINDOW_HPP_

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <list>
#include <sstream>
#include <string>

#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LTexture.hpp"
#include "Panel.hpp"

struct SDL_Renderer;

class IWindow
{
private:
	LTexture gBackgroundTexture;

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("IWindow");
		return instance;
	}

public:

	//Focuses on window
	void focus()
	{
		//Restore window if needed
		if (!mShown)
		{
			SDL_ShowWindow(mWindow);
		}

		//Move window forward
		SDL_RaiseWindow(mWindow);
	}

	std::string getTitle()
	{
		return mTitle;
	}

	int getWidth()
	{
		return mWidth;
	}

	int getHeight()
	{
		return mHeight;
	}

	bool hasMouseFocus()
	{
		return mMouseFocus;
	}

	bool hasKeyboardFocus()
	{
		return mKeyboardFocus;
	}

	bool isMinimized()
	{
		return mMinimized;
	}

	bool isShown()
	{
		return mShown;
	}

	bool isClosed()
	{
		return mClosed;
	}

	//functions which can be surcharged

	void free()
	{
		SDL_DestroyRenderer(mRenderer);

		if (mWindow != NULL)
		{
			SDL_DestroyWindow(mWindow);
		}

		mMouseFocus = false;
		mKeyboardFocus = false;
		mWidth = 0;
		mHeight = 0;
	}

	virtual bool initSDL(int x, int y, int w, int h, std::string wtitle, Uint32 flags)
	{
		bool success = true;

		mTitle = wtitle;

		//Set the scaling quality to nearest-pixel
		if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") < 0)
		{
			success = false;
			logger().error() << "Error: Failed to set Render Scale Quality" << logs::end;
			return success;
		}

		//Create window
		mWindow = SDL_CreateWindow(mTitle.c_str(), x, y, w, h, flags);
		if (mWindow != NULL)
		{
			mMouseFocus = true;
			mKeyboardFocus = true;
			mWidth = w;
			mHeight = h;

			//Create renderer for window
			mRenderer = SDL_CreateRenderer(mWindow, -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mRenderer == NULL)
			{
				logger().error() << "Error: Renderer could not be created! SDL Error: "
						<< SDL_GetError() << logs::end;
				SDL_DestroyWindow(mWindow);
				mWindow = NULL;
				success = false;
				return success;
			}
			else
			{
				//Grab window identifier
				mWindowID = SDL_GetWindowID(mWindow);

				//Flag as opened
				mShown = true;
			}
		}
		else
		{
			success = false;
			logger().error() << "Error: Window could not be created! SDL Error: " << SDL_GetError()
					<< logs::end;
			return success;
		}
		logger().debug() << "Window initSDL => ID= " << mWindowID << logs::end;
		return success;
	}

	virtual void handleEvent(SDL_Event& e)
	{
		//If an event was detected for this window
		if (e.window.windowID != mWindowID)
			return;

		if (e.type == SDL_WINDOWEVENT)
		{
			//Caption update flag
			bool updateCaption = false;

			switch (e.window.event)
			{
				//Window appeared
				case SDL_WINDOWEVENT_SHOWN:
					mShown = true;
					break;

					//Window disappeared
				case SDL_WINDOWEVENT_HIDDEN:
					mShown = false;
					break;

					//Get new dimensions and repaint
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					mWidth = e.window.data1;
					mHeight = e.window.data2;
					SDL_RenderPresent(mRenderer);
					break;

					//Repaint on expose
				case SDL_WINDOWEVENT_EXPOSED:
					SDL_RenderPresent(mRenderer);
					break;

					//Mouse enter
				case SDL_WINDOWEVENT_ENTER:
					mMouseFocus = true;
					updateCaption = true;
					break;

					//Mouse exit
				case SDL_WINDOWEVENT_LEAVE:
					mMouseFocus = false;
					updateCaption = true;
					break;

					//Keyboard focus gained
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					mKeyboardFocus = true;
					updateCaption = true;
					break;

					//Keyboard focus lost
				case SDL_WINDOWEVENT_FOCUS_LOST:
					mKeyboardFocus = false;
					updateCaption = true;
					break;

					//Window minimized
				case SDL_WINDOWEVENT_MINIMIZED:
					mMinimized = true;
					break;

					//Window maxized
				case SDL_WINDOWEVENT_MAXIMIZED:
					mMinimized = false;
					break;

					//Window restored
				case SDL_WINDOWEVENT_RESTORED:
					mMinimized = false;
					break;

					//Hide and quit on close
				case SDL_WINDOWEVENT_CLOSE:
					SDL_HideWindow(mWindow);
					mClosed = true;
					break;
			}

			//Demo Update window caption with new data
			if (updateCaption)
			{
				std::stringstream caption;
				caption << mTitle << " - ID: " << mWindowID << " MouseFocus:"
						<< ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:"
						<< ((mKeyboardFocus) ? "On" : "Off");
				SDL_SetWindowTitle(mWindow, caption.str().c_str());
			}
		}

		//Case of Panels
		int x, y;
		//Get mouse position
		SDL_GetMouseState(&x, &y);

		for (std::list<Panel*>::iterator iter = pList_->begin(); iter != pList_->end(); iter++)
		{
			Panel *p = (*iter);
			p->handleEvent(e, x - p->getX(), y - p->getY());
		}
	}

	virtual void render()
	{
		if (!mMinimized)
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(mRenderer);
			SDL_Rect stretchRect;
			stretchRect =
			{	0, 0, mWidth, mHeight};
			gBackgroundTexture.render(mRenderer, 0, 0, &stretchRect);

			for (std::list<Panel*>::iterator iter = pList_->begin(); iter != pList_->end(); iter++)
			{
				Panel *p = (*iter);
				p->render(mRenderer);
			}
		}

		//Update screen
		SDL_RenderPresent(mRenderer);
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ IWindow()
	{
	}

	void addPanel(Panel* p)
	{
		pList_->push_back(p);
	}

	//TODO add get()
	SDL_Renderer* mRenderer;

protected:

	//SDL data
	SDL_Window* mWindow;

	Uint32 mWindowID;
	bool mShown;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mClosed;

	std::string mTitle;

	std::list<Panel*> * pList_;

	/*!
	 * \brief Constructeur de la classe.
	 */
	IWindow()
	{
		mWindow = NULL;
		mRenderer = NULL;

		mShown = false;
		mWindowID = -1;

		//Initialize non-existant window
		mMinimized = NULL;

		mMouseFocus = false;
		mKeyboardFocus = false;
		mFullScreen = false;

		mClosed = false;
		mWindowID = -1;

		mWidth = 0;
		mHeight = 0;

		mTitle = "Default";

		pList_ = new std::list<Panel*>();
	}
};

#endif /* SIMULAUNCHER_MAIN_IWINDOW_HPP_ */
