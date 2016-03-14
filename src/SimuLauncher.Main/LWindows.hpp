#ifndef SIMULAUNCHER_MAIN_LWINDOWS_HPP_
#define SIMULAUNCHER_MAIN_LWINDOWS_HPP_

#include "../Log/LoggerFactory.hpp"
#include "IWindow.hpp"
#include "LTexture.hpp"

class BotManager;
class DemoButtons;

class MainWindow: public IWindow
{
private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref MainWindow.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("MainWindow");
		return instance;
	}

	LTexture gBackgroundTextureSvg;
	LTexture gBackgroundTexturePng;
	LTexture gBackgroundTextureMain;

public:

	bool loadContent(BotManager *botm);
	virtual void render();

};

class SecondWindow: public IWindow
{

private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SecondWindow.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SecondWindow");
		return instance;
	}

	LTexture gBackgroundTextureMainWin1;

public:

	DemoButtons * dbuttons;

	void buttonHandleEvent(SDL_Event& e);
	bool loadContent(BotManager *botm);

	virtual void render();

};

class ThirdWindow: public IWindow
{
private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ThirdWindow.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ThirdWindow");
		return instance;
	}

public:

	bool loadContent(BotManager *botm);
	virtual void render();

};

#endif
