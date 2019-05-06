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
    add("svg", new SvgAppender("svgEV3.svg"));
    add("svgia", new SvgAppender("svgIA.svg"));
    //add("memory", new MemoryAppender());
    add("fileAsserv", new FileAppender("logAsserv.csv"));
    //add("fileRegul", new FileAppender("logRegul.csv"));

    add(logs::Level::ERROR, "", "console");

    add(logs::Level::INFO, "Robot", "console");
    add(logs::Level::INFO, "LegoEV3RobotExtended", "console");
    add(logs::Level::INFO, "LegoEV3AsservExtended", "console");
    add(logs::Level::INFO, "LegoEV3ActionsExtended", "console");


    //COMMON
    add(logs::Level::INFO, "Tirette", "console");
    add(logs::Level::INFO, "Wait90SecAction", "console");
    add(logs::Level::INFO, "SensorsTimer", "console");
    add(logs::Level::INFO, "Chronometer", "console");


    //STATE
    add(logs::Level::INFO, "L_State_Init", "console");
    add(logs::Level::INFO, "L_State_DecisionMakerIA", "console");
    add(logs::Level::INFO, "L_State_WaitEndOfMatch", "console");

    //SVG
    add(logs::Level::INFO, "Svg4LegoEV3Robot", "svg");
    add(logs::Level::INFO, "IAbyPath4LegoEV3Robot", "svgia");

    //File DEBUG
    //add(logs::Level::DEBUG, "logFileAsservInsa", "file");
    add(logs::Level::DEBUG, "logFileAsservEsialR", "fileAsserv");
    //add(logs::Level::DEBUG, "logFileAsservRegul", "fileRegul");

    //TESTS
    add(logs::Level::INFO, "L_LedBarTest", "console");
    add(logs::Level::INFO, "L_ServoUsingMotorTest", "console");
    add(logs::Level::INFO, "L_ButtonBarTest", "console");
    add(logs::Level::INFO, "L_SoundBarTest", "console");
    add(logs::Level::INFO, "L_TiretteTest", "console");
    add(logs::Level::INFO, "L_SensorsTest", "console");
    add(logs::Level::INFO, "L_ServoStepTest", "console");
    add(logs::Level::INFO, "L_MovingBaseTest", "console");
    add(logs::Level::INFO, "L_ActionManagerTimerTest", "console");
    add(logs::Level::INFO, "L_AsservLineRotateTest", "console");
    add(logs::Level::INFO, "TestAction", "console");
    add(logs::Level::INFO, "TestTimer", "console");

    add(logs::Level::INFO, "L_Asserv_FindPIDTest", "console");
    add(logs::Level::INFO, "L_Asserv_SetResolutionTest", "console");
    add(logs::Level::INFO, "L_Asserv_SquareTest", "console");
    add(logs::Level::INFO, "L_AsservEsialTest", "console");
    add(logs::Level::INFO, "L_AsservRunTest", "console");
    add(logs::Level::INFO, "L_IATest", "console");
    add(logs::Level::INFO, "L_ServoObjectTest", "console");

    //DRIVERS SIMU
    add(logs::Level::INFO, "LedDriver.SIMU", "console");
    add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
    add(logs::Level::INFO, "SoundDriver.SIMU", "console");
    add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
    add(logs::Level::INFO, "AsservDriver.SIMU", "console");
    //add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");
    //add(logs::Level::INFO, "AsservDriverMemory.SIMU", "console");

    //DRIVERS EV3
    add(logs::Level::INFO, "LedDriver.EV3", "console");
    add(logs::Level::INFO, "ButtonDriver.EV3", "console");
    add(logs::Level::INFO, "SoundDriver.EV3", "console");
    add(logs::Level::INFO, "SwitchDriver.EV3", "console");
    add(logs::Level::INFO, "AsservDriver.EV3", "console");
    add(logs::Level::INFO, "ServoDriver.EV3", "console");
    add(logs::Level::INFO, "SensorsDriver.EV3", "console");
    add(logs::Level::INFO, "LegoIRSensor.EV3", "console");
    add(logs::Level::INFO, "LegoAngleSensor.EV3", "console");
    add(logs::Level::INFO, "LegoMotor.EV3", "console");
    add(logs::Level::INFO, "LcdShieldDriver.EV3", "console");


    //DEBUG
    add(logs::Level::INFO, "ActionManagerTimer", "console");
    add(logs::Level::INFO, "MovingBase", "console");

    add(logs::Level::INFO, "Asserv", "console");
    //add(logs::Level::INFO, "AsservInsa", "console");
    add(logs::Level::DEBUG, "AsservEsialR", "console");




}
