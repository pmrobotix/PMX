/*
 * Panel.cpp
 *
 *  Created on: 8 févr. 2016
 *      Author: pmx
 */

#include "Panel.hpp"

union SDL_Event;

Panel::Panel(int x, int y, int w, int h)
{
	//window_ = NULL;
	x_=x;
	y_=y;
	w_=w;
	h_=h;
}
