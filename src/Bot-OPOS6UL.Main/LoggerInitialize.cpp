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
	//add("file", new FileAppender("logAPF.csv"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "Robot", "console");
	add(logs::Level::INFO, "OPOS6UL_RobotExtended", "console");

	//COMMON
	//add(logs::Level::INFO, "Tirette", "console");
	add(logs::Level::INFO, "Asserv", "console");


	//SVG
	add(logs::Level::INFO, "Svg4OPOS6UL_Robot", "svg");
	add(logs::Level::INFO, "AsservDriver.OPO.SVG", "svg");
	add(logs::Level::INFO, "AsservDriver.SIMU.SVG", "svg");


	//File
	//add(logs::Level::DEBUG, "logFileAsservInsa", "file");


	//State
	add(logs::Level::INFO, "O_State1", "console");
	add(logs::Level::INFO, "O_State_decisionMaker", "console");
	add(logs::Level::INFO, "O_State_Wait90SecAction", "console");


	//TESTS
	add(logs::Level::INFO, "O_LedBarTest", "console");
	add(logs::Level::INFO, "O_ButtonBarTest", "console");
	add(logs::Level::INFO, "O_LcdBoardTest", "console");
	add(logs::Level::INFO, "O_GroveColorTest", "console");
	add(logs::Level::INFO, "O_AsservTest", "console");
	add(logs::Level::INFO, "O_Asserv_SquareTest", "console");



	/*
	add(logs::Level::INFO, "A_TiretteTest", "console");
	add(logs::Level::INFO, "A_SensorsTest", "console");
	add(logs::Level::INFO, "A_ServoTest", "console");
	add(logs::Level::INFO, "A_ServoStepTest", "console");
	add(logs::Level::INFO, "A_Asserv_SetResolutionTest", "console");
	add(logs::Level::INFO, "A_Asserv_FindPIDTest", "console");
	add(logs::Level::INFO, "A_AsservInsaTest", "console");
	add(logs::Level::INFO, "A_AsservRunTest", "console");
	add(logs::Level::INFO, "A_Asserv_SquareTest", "console");
	add(logs::Level::INFO, "A_IATest", "console");
	add(logs::Level::INFO, "O_MovingBaseTest", "console");
*/

	//DRIVERS SIMU
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");

	//DRIVERS OPOS
	add(logs::Level::INFO, "LedDriver.OPO", "console");
	add(logs::Level::INFO, "ButtonDriver.OPO", "console");
	add(logs::Level::INFO, "SoundDriver.OPO", "console");
	add(logs::Level::INFO, "SwitchDriver.OPO", "console");
	add(logs::Level::INFO, "SensorsDriver.OPO", "console");
	add(logs::Level::INFO, "AsservDriver.OPO", "console");
	//add(logs::Level::INFO, "GpioPCA9555", "console");
	//add(logs::Level::INFO, "ExtEncoder", "console");

	//add(logs::Level::INFO, "IrSensor", "console");
	//add(logs::Level::INFO, "as_adc", "console");


}
