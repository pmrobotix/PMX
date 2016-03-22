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


	//DRIVERS
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");


	//DEBUG
	add(logs::Level::DEBUG, "MovingBase", "console");


}
