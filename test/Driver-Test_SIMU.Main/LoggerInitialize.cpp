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
	this->add("console", new ConsoleAppender());
	this->add("unittestconsole", new UnitTestAppender());
	this->add("memory", new MemoryAppender());

	add(logs::Level::INFO, "", "unittestconsole");


	add(logs::Level::DEBUG, "AsservDriver.SIMU", "console");
	add(logs::Level::DEBUG, "AsservDriverTest.SIMU", "console");
	add(logs::Level::DEBUG, "LedDriverTest.SIMU", "console");
	add(logs::Level::DEBUG, "AsservDriverMemory.SIMU", "memory");


}
