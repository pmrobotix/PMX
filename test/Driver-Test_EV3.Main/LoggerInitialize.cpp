/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../../src/Log/Level.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTestAppender.hpp"

void logs::LoggerFactory::initialize()
{
	//add("console", new ConsoleAppender());
	//add("memory", new MemoryAppender());

	this->add("console", new UnitTestAppender());

	add(logs::Level::ERROR, "", "console");
	add(logs::Level::INFO, "", "console");

	//DEBUG
	add(logs::Level::DEBUG, "AsservDriver.EV3", "console");

}
