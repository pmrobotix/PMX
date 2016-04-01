/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../Log/Appender/FileAppender.hpp"
#include "../Log/Appender/SvgAppender.hpp"
#include "../Log/Appender/ConsoleAppender.hpp"
#include "../Log/Level.hpp"
#include "../Log/LoggerFactory.hpp"

void logs::LoggerFactory::initialize()
{
	add("console", new ConsoleAppender());
	//add("memory", new MemoryAppender());
	add("APF9328svg", new SvgAppender("./resources/svg/APF9328Robot.svg"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "Robot", "console");
	add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
	add(logs::Level::INFO, "LegoEV3State1", "console");

	//SVG
	add(logs::Level::INFO, "Svg4APF9328Robot", "APF9328svg");

	//TESTS
	add(logs::Level::INFO, "APF9328LedBarTest", "console");
	/*
	add(logs::Level::INFO, "APF9328ButtonBarTest", "console");
	add(logs::Level::INFO, "APF9328SoundBarTest", "console");
	add(logs::Level::INFO, "APF9328TiretteTest", "console");
	add(logs::Level::INFO, "APF9328MovingBaseTest", "console");
	add(logs::Level::INFO, "APF9328AsservInsaTest", "console");
*/
	//DRIVERS SIMU
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriver.SIMU", "console");

	//DRIVERS EV3
	add(logs::Level::INFO, "LedDriver.ARM", "console");
	add(logs::Level::INFO, "ButtonDriver.ARM", "console");
	add(logs::Level::INFO, "SoundDriver.ARM", "console");
	add(logs::Level::INFO, "SwitchDriver.ARM", "console");
	add(logs::Level::INFO, "AsservDriver.ARM", "console");

}
