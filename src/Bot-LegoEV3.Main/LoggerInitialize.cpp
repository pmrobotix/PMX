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
	add("legoEV3svg", new SvgAppender("./resources/svg/LegoEV3Robot.svg"));
	//add("memory", new MemoryAppender());
	//add("file", new FileAppender("log.txt"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "Robot", "console");
	add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
	add(logs::Level::INFO, "LegoEV3State1", "console");

	//SVG
	add(logs::Level::INFO, "Svg4LegoEV3Robot", "legoEV3svg");

	//TESTS
	add(logs::Level::INFO, "LegoEV3LedBarTest", "console");
	add(logs::Level::INFO, "LegoEV3ButtonBarTest", "console");
	add(logs::Level::INFO, "LegoEV3SoundBarTest", "console");
	add(logs::Level::INFO, "LegoEV3TiretteTest", "console");
	add(logs::Level::INFO, "LegoEV3MovingBaseTest", "console");
	add(logs::Level::INFO, "LegoEV3AsservInsaTest", "console");

	//DRIVERS SIMU
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriver.SIMU", "console");

	//DRIVERS EV3
	add(logs::Level::INFO, "LedDriver.EV3", "console");
	add(logs::Level::INFO, "ButtonDriver.EV3", "console");
	add(logs::Level::INFO, "SoundDriver.EV3", "console");
	add(logs::Level::INFO, "SwitchDriver.EV3", "console");
	add(logs::Level::INFO, "AsservDriver.EV3", "console");

	//DEBUG
	add(logs::Level::DEBUG, "MovingBase", "console");

}
