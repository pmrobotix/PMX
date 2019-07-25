//drivers...SIMULAUNCHER

#include "LedDriver.hpp"

#include <SDL2/SDL_pixels.h>
#include <sstream>

#include "../Log/Logger.hpp"
#include "../SimuLauncher.Main/IWindow.hpp"
#include "../SimuLauncher.Main/LWindows.hpp"
#include "../SimuLauncher.Main/SDLTool.hpp"
#include "../SimuLauncher.Main/SimLedBar.hpp"
#include "../SimuLauncher.Main/Simulator.hpp"
#include "../SimuLauncher.Main/Text.hpp"

//init variable statique
int LedDriver::nb_leddriver = 0;

ALedDriver * ALedDriver::create(std::string botName, int nb)
{
	return new LedDriver(botName, nb);
}

LedDriver::LedDriver(std::string botname, int nb)
{
	once_ = 0;
	nb_ = nb;
	ledbar_ = NULL;

	nb_leddriver++; //increment nb of instances of robot

	logger().debug() << "increment nb of robot" << nb_leddriver << logs::end;

	height_display_ = (nb_leddriver * 50) + 30; //dynamic height
	botId_ = botname;

	std::stringstream stext;
	stext << botId_ << " - nb LED = " << nb;
	text_ = stext.str();

	//add task to the simulator to load
	LedDriverSDLTask *task = new LedDriverSDLTask(this);

	SDLTool::addSDLTask(task);
}

LedDriver::~LedDriver()
{
}

void LedDriver::load()
{
	once_++;
	if (once_ == 1)
	{
		logger().debug() << "LedDriver::load()" << logs::end;
		Simulator &sim = Simulator::instance();

		Text * text = new Text(0, height_display_, 0, 0);
		SDL_Color textColor =
		{ 100, 100, 100, 0 };
		text->loadMedia(sim.thirdWindow->mRenderer, "./resources/ttf/Verdana/verdanab.ttf", text_,
				10, textColor);

		sim.thirdWindow->addPanel(text);

		ledbar_ = new SimLedBar(0, height_display_ + 20, 16, 16, nb_);

		ledbar_->loadMedia(sim.thirdWindow->mRenderer, "./resources/IHM/led_16_off.svg",
				"./resources/IHM/led_16_green.svg", "./resources/IHM/led_16_red.svg",
				"./resources/IHM/led_16_orange.svg");

		sim.thirdWindow->addPanel(ledbar_);
	}
}

void LedDriver::setBit(int index, LedColor color)
{
	ledbar_->setBit(index, color);
}

void LedDriver::setBytes(uint hex, LedColor color)
{
	ledbar_->setBytes(hex, color);
}

LedDriverSDLTask::LedDriverSDLTask(LedDriver * leddriver)
{
	leddriver_ = leddriver;
}

void LedDriverSDLTask::run()
{
	leddriver_->load();
}

