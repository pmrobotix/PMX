/*
 * SimLedBar.hpp
 *
 *  Created on: 2 mars 2016
 *      Author: pmx
 */

#ifndef SIMULAUNCHER_MAIN_SIMLEDBAR_HPP_
#define SIMULAUNCHER_MAIN_SIMLEDBAR_HPP_

#include <sys/types.h>
#include <SDL2/SDL_rect.h>
#include <string>

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LTexture.hpp"
#include "Panel.hpp"

class LTexture;

class SimLedBar: public Panel
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SimLedBar.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SimLedBar");
		return instance;
	}

	LTexture *currentTexture_;
	LTexture offTexture_;
	LTexture greenTexture_;
	LTexture orangeTexture_;
	LTexture redTexture_;
	SDL_Rect spriteClip_;
	int nb_;

public:

	SimLedBar(int x, int y, int w, int h, int nb);

	~SimLedBar()
	{
		//Free loaded images
		offTexture_.free();
		greenTexture_.free();
		orangeTexture_.free();
		redTexture_.free();
	}

	void handleEvent(SDL_Event& e, int xMouse, int yMouse);

	void render(SDL_Renderer * renderer);

	bool loadMedia(SDL_Renderer * renderer, std::string pathOff, std::string pathGreen,
			std::string pathRed, std::string pathOrange);

	void setBit(int index, LedColor color);
	void setBytes(uint hex, LedColor color);

};

#endif
