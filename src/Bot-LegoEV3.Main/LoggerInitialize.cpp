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
	//add("file", new FileAppender("log.txt"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "LedBarTest", "console");
	add(logs::Level::INFO, "ButtonBarTest", "console");

	add(logs::Level::INFO, "Robot", "console");

	add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
	add(logs::Level::INFO, "LegoEV3State1", "console");
	add(logs::Level::INFO, "LegoEV3LedBarTest", "console");


	//DEBUG

	//add(logs::Level::DEBUG, "MotorControl", "console");
	//add(logs::Level::DEBUG, "ActionManager", "console");
	//add(logs::Level::DEBUG, "Actions", "console");
	//add(logs::Level::INFO, "Asserv", "console");

	//DRIVER
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	//add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	//add(logs::Level::INFO, "MotorDeviceDriver.EV3", "console");
	//add(logs::Level::INFO, "AsservDriver.EV3", "console");

}
