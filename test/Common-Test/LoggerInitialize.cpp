/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../../src/Log/Appender/FileAppender.hpp"
#include "../../src/Log/Appender/SvgAppender.hpp"
#include "../../src/Log/Appender/ConsoleAppender.hpp"
#include "../../src/Log/Level.hpp"
#include "../../src/Log/LoggerFactory.hpp"

void logs::LoggerFactory::initialize()
{
	add("console", new ConsoleAppender());
	//add("memory", new MemoryAppender());

	add("test-svg", new SvgAppender("./svg/file.svg"));

	add(logs::Level::ERROR, "", "console");
	add(logs::Level::INFO, "", "console");



	//LoggerTest
	add(logs::Level::DEBUG, "test::LoggerTest", "console");
	add(logs::Level::INFO, "test::LoggerTestSvg", "test-svg");
	add(logs::Level::INFO, "SvgWriter::svg", "test-svg");
}

