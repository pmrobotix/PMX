/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../../src/Log/Appender/ConsoleAppender.hpp"
#include "../../src/Log/Level.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTestAppender.hpp"

void logs::LoggerFactory::initialize()
{
	add("log", new ConsoleAppender());
	//add("memory", new MemoryAppender());

	this->add("console", new UnitTestAppender());

	add(logs::Level::ERROR, "", "console");
	//add(logs::Level::INFO, "", "console");

	//DEBUG
	add(logs::Level::DEBUG, "AsservDriverTest.EV3", "log");
	add(logs::Level::DEBUG, "SensorDriverTest.EV3", "log");
	add(logs::Level::DEBUG, "ServoDriverTest.EV3", "log");

	add(logs::Level::DEBUG, "SensorsDriver.EV3", "log");
	add(logs::Level::DEBUG, "AsservDriver.EV3", "log");
	add(logs::Level::DEBUG, "LegoIRSensor.EV3", "log");
	add(logs::Level::DEBUG, "ServoDriver.EV3", "log");


}
