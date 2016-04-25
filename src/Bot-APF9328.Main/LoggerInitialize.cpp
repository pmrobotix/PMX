/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../Log/Appender/ConsoleAppender.hpp"
#include "../Log/Appender/FileAppender.hpp"
#include "../Log/Appender/SvgAppender.hpp"
#include "../Log/Level.hpp"
#include "../Log/LoggerFactory.hpp"

void logs::LoggerFactory::initialize()
{
	add("console", new ConsoleAppender());
	add("svg", new SvgAppender("svgAPF.svg"));
	add("memory", new MemoryAppender());
	add("file", new FileAppender("logAPF.csv"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "Robot", "console");
	add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
	add(logs::Level::INFO, "LegoEV3State1", "console");

	//SVG
	add(logs::Level::INFO, "Svg4APF9328Robot", "svg");

	//File
	add(logs::Level::INFO, "logFileAsservInsa", "file");

	//TESTS
	add(logs::Level::INFO, "A_LedBarTest", "console");
	add(logs::Level::INFO, "A_MovingBaseTest", "console");
	add(logs::Level::INFO, "A_LcdBoardTest", "console");
	add(logs::Level::INFO, "A_Asserv_SetResolutionTest", "console");
	add(logs::Level::INFO, "A_Asserv_FindPIDTest", "console");
	add(logs::Level::INFO, "A_Asserv_SquareTest", "console");
		/*
	add(logs::Level::INFO, "APF9328ButtonBarTest", "console");
	add(logs::Level::INFO, "APF9328SoundBarTest", "console");
	add(logs::Level::INFO, "APF9328TiretteTest", "console");
	add(logs::Level::INFO, "APF9328AsservInsaTest", "console");
*/
	//DRIVERS SIMU
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");

	//DRIVERS EV3
	add(logs::Level::INFO, "LedDriver.ARM", "console");
	add(logs::Level::INFO, "ButtonDriver.ARM", "console");
	add(logs::Level::INFO, "SoundDriver.ARM", "console");
	add(logs::Level::INFO, "SwitchDriver.ARM", "console");
	add(logs::Level::INFO, "AsservDriver.ARM", "console");

}
