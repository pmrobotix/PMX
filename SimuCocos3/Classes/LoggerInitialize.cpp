/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "Log/Appender/ConsoleAppender.hpp"
#include "Log/Level.hpp"
#include "Log/LoggerFactory.hpp"

void logs::LoggerFactory::initialize()
{
	add("console", new ConsoleAppender());

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::DEBUG, "HelloWorld", "console");

}
