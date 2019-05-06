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
	add("svgia", new SvgAppender("svgIA.svg"));
	add("memory", new MemoryAppender());
	//add("file", new FileAppender("logAPF.csv"));

	add(logs::Level::ERROR, "", "console");

	add(logs::Level::INFO, "Robot", "console");
	add(logs::Level::INFO, "OPOS6UL_RobotExtended", "console");

	//COMMON
	add(logs::Level::INFO, "Asserv", "console");
	add(logs::Level::INFO, "IAbyPath", "console");
	add(logs::Level::INFO, "SensorsTimer", "console");
	add(logs::Level::INFO, "ServoObjectsSystem", "console");


	//SVG
	add(logs::Level::INFO, "Svg4OPOS6UL_Robot", "svg");
	add(logs::Level::INFO, "AsservDriver.OPO.SVG", "svg");
	add(logs::Level::INFO, "AsservDriver.SIMU.SVG", "svg");
	add(logs::Level::INFO, "IAbyPath4OPOS6UL_Robot", "svgia");


	//File
	//add(logs::Level::DEBUG, "logFileAsservInsa", "file");


	//State
	add(logs::Level::INFO, "O_State_Init", "console");
	add(logs::Level::INFO, "O_State_DecisionMakerIA", "console");
	add(logs::Level::INFO, "O_State_WaitEndOfMatch", "console");


	//TESTS
	add(logs::Level::INFO, "O_LedBarTest", "console");
	add(logs::Level::INFO, "O_ButtonBarTest", "console");
	add(logs::Level::INFO, "O_LcdBoardTest", "console");
	add(logs::Level::INFO, "O_GroveColorTest", "console");
	add(logs::Level::INFO, "O_AsservTest", "console");
	add(logs::Level::INFO, "O_Asserv_SquareTest", "console");
	add(logs::Level::INFO, "O_Asserv_CalageTest", "console");
	add(logs::Level::INFO, "O_ServoStepTest", "console");
	add(logs::Level::INFO, "O_ServoObjectsTest", "console");
	add(logs::Level::INFO, "O_SensorsTest", "console");
	add(logs::Level::INFO, "O_IAByPathTest", "console");




	//DRIVERS SIMU
	add(logs::Level::INFO, "LedDriver.SIMU", "console");
	add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
	add(logs::Level::INFO, "SoundDriver.SIMU", "console");
	add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriver.SIMU", "console");
	add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");
	add(logs::Level::INFO, "LcdShieldDriver.SIMU", "memory");

	//DRIVERS OPOS
	add(logs::Level::INFO, "LedDriver.OPO", "console");
	add(logs::Level::INFO, "ButtonDriver.OPO", "console");
	add(logs::Level::INFO, "SoundDriver.OPO", "console");
	add(logs::Level::INFO, "SwitchDriver.OPO", "console");
	add(logs::Level::DEBUG, "SensorsDriver.OPO", "console");
	add(logs::Level::INFO, "AsservDriver.OPO", "console");
	add(logs::Level::INFO, "CCAx12Adc.OPO", "console");
	add(logs::Level::INFO, "SensorsDriver.OPO", "console");
	add(logs::Level::INFO, "IrSensor.OPO", "console");




}
