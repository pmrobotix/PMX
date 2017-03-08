/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../../src/Log/Appender/SvgAppender.hpp"
#include "../../src/Log/Level.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTestAppender.hpp"

void logs::LoggerFactory::initialize()
{
	this->add("console", new UnitTestAppender());

	//add("test-svg", new SvgAppender("./svg/file.svg"));

	//add(logs::Level::ERROR, "", "console");
	//add(logs::Level::INFO, "", "console");
	add(logs::Level::DEBUG, "", "console");

	//add(logs::Level::INFO, "test::LoggerTestSvg", "test-svg");
	//add(logs::Level::INFO, "SvgWriter::svg", "test-svg");

	//LoggerTest
	//add(logs::Level::DEBUG, "test::LoggerTest", "console");

}

