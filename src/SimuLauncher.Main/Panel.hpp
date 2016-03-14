/*
 * Panel.hpp
 *
 *  Created on: 8 févr. 2016
 *      Author: pmx
 */

#ifndef SIMULAUNCHER_MAIN_PANEL_HPP_
#define SIMULAUNCHER_MAIN_PANEL_HPP_
struct SDL_Renderer;

union SDL_Event;

class IWindow;

//something clickable
class Panel
{
public:

	virtual inline ~Panel()
	{
	}

	virtual void handleEvent(SDL_Event& e, int xMouse, int yMouse) = 0;

	virtual void render(SDL_Renderer * renderer) = 0;

	int getX()
	{
		return x_;
	}
	int getY()
	{
		return y_;
	}
	int getW()
	{
		return w_;
	}
	int getH()
	{
		return h_;
	}

	Panel(int x, int y, int w, int h);

protected:
	//IWindow * window_;
	int x_;
	int y_;
	int w_;
	int h_;
};

#endif /* SIMULAUNCHER_MAIN_PANEL_HPP_ */
