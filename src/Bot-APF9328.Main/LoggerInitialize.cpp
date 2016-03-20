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
	add("APF9328svg", new SvgAppender("./resources/svg/APF9328Robot.svg"));

	add(logs::Level::ERROR, "", "console");

	//SVG
	add(logs::Level::INFO, "Svg4APF9328Robot", "APF9328svg");

	//TESTS
	add(logs::Level::INFO, "LedBarTest", "console");

	//DRIVER
	add(logs::Level::DEBUG, "LedDriver.SIMU", "console");

}
