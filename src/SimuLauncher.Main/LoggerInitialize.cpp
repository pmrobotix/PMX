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
	add("APF9328svg", new SvgAppender("./resources/svg/APF9328Robot.svg"));
		//add("memory", new MemoryAppender());

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "BotManager", "console");
	add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
	add(logs::Level::INFO, "LegoEV3LedBarTest", "console");
	add(logs::Level::INFO, "LegoEV3State1", "console");
	add(logs::Level::INFO, "APF9328RobotExtended", "console");
	add(logs::Level::INFO, "APF9328LedBarTest", "console");
	add(logs::Level::INFO, "APF9328State1", "console");

	add(logs::Level::INFO, "Svg4LegoEV3Robot", "legoEV3svg");
	add(logs::Level::INFO, "Svg4APF9328Robot", "APF9328svg");



}
