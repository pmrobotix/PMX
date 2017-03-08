/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../../src/Log/Level.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTestAppender.hpp"

void logs::LoggerFactory::initialize()
{
	this->add("console", new UnitTestAppender());

	add(logs::Level::ERROR, "", "console");
	add(logs::Level::INFO, "", "console");
}
