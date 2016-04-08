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
	add("legoEV3svg", new SvgAppender("svgEV3.svg"));
	add("memory", new MemoryAppender());
	add("file", new FileAppender("logEV3.csv"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "Robot", "console");
	add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
	add(logs::Level::INFO, "LegoEV3State1", "console");

	//SVG
	add(logs::Level::INFO, "Svg4LegoEV3Robot", "legoEV3svg");
	add(logs::Level::INFO, "Svg4AsservInsa", "legoEV3svg");

	//File
	add(logs::Level::DEBUG, "logFileAsservInsa", "file");

	//TESTS
	add(logs::Level::INFO, "LegoEV3LedBarTest", "console");
	add(logs::Level::INFO, "LegoEV3ButtonBarTest", "console");
	add(logs::Level::INFO, "LegoEV3SoundBarTest", "console");
	add(logs::Level::INFO, "LegoEV3TiretteTest", "console");
	add(logs::Level::INFO, "LegoEV3MovingBaseTest", "console");

	add(logs::Level::DEBUG, "LegoEV3FindPIDTest", "console");
	add(logs::Level::DEBUG, "LegoEV3AsservSetResolutionTest", "console");
	add(logs::Level::DEBUG, "LegoEV3AsservInsaTest", "console");
	add(logs::Level::DEBUG, "LegoEV3AsservRunTest", "console");

	//DRIVERS SIMU
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");

	//DRIVERS EV3
	add(logs::Level::INFO, "LedDriver.EV3", "console");
	add(logs::Level::INFO, "ButtonDriver.EV3", "console");
	add(logs::Level::INFO, "SoundDriver.EV3", "console");
	add(logs::Level::INFO, "SwitchDriver.EV3", "console");
	add(logs::Level::INFO, "AsservDriver.EV3", "console");

	//DEBUG
	add(logs::Level::INFO, "MovingBase", "console");
	add(logs::Level::INFO, "AsservInsa", "console");
	add(logs::Level::DEBUG, "Asserv", "console");

}
