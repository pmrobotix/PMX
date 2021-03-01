/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../Log/Appender/ConsoleAppender.hpp"
#include "../Log/Appender/SvgAppender.hpp"
#include "../Log/Appender/TelemetryAppender.hpp"
#include "../Log/Level.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"

void logs::LoggerFactory::initialize()
{
    //set minimum thread priority FIFO
    utils::set_realtime_priority(1);

    add("net", new ConsoleAppender());
    add("svg", new SvgAppender("svgEV3.svg"));
    add("svgia", new SvgAppender("svgIA.svg"));
    //add("memory", new MemoryAppender());
    //add("fileAsserv", new FileAppender("logAsserv.csv"));
    //add("fileRegul", new FileAppender("logRegul.csv"));

    add("nettel", new TelemetryAppender("MSG|SML"));

    add(logs::Level::ERROR, "", "net");

    add(logs::Level::INFO, "Robot", "net");
    add(logs::Level::INFO, "LegoEV3RobotExtended", "net");
    add(logs::Level::INFO, "LegoEV3AsservExtended", "net");
    add(logs::Level::INFO, "LegoEV3ActionsExtended", "net");

    //COMMON
    add(logs::Level::INFO, "Tirette", "net");
    add(logs::Level::INFO, "Wait90SecAction", "net");
    add(logs::Level::INFO, "Sensors", "net");
    add(logs::Level::INFO, "SensorsTimer", "net");
    add(logs::Level::INFO, "Chronometer", "net");
    add(logs::Level::INFO, "IAbyPath", "net");

    //STATE
    add(logs::Level::INFO, "L_State_Init", "net");
    add(logs::Level::INFO, "L_State_DecisionMakerIA", "net");
    add(logs::Level::INFO, "L_State_WaitEndOfMatch", "net");

    //SVG
    add(logs::Level::INFO, "Svg4LegoEV3Robot", "svg");
    add(logs::Level::INFO, "IAbyPath4LegoEV3Robot", "svgia");

    //File DEBUG
    //add(logs::Level::DEBUG, "logFileAsservInsa", "file");
    //add(logs::Level::DEBUG, "logFileAsservEsialR", "fileAsserv");
    //add(logs::Level::DEBUG, "logFileAsservRegul", "fileRegul");

    //TESTS
    add(logs::Level::INFO, "L_LedBarTest", "net");
    add(logs::Level::INFO, "L_LcdTest", "net");
    add(logs::Level::INFO, "L_ServoUsingMotorTest", "net");
    add(logs::Level::INFO, "L_ButtonBarTest", "net");
    add(logs::Level::INFO, "L_SoundBarTest", "net");
    add(logs::Level::INFO, "L_TiretteTest", "net");
    add(logs::Level::INFO, "L_SensorsTest", "net");
    add(logs::Level::INFO, "L_ServoStepTest", "net");
    add(logs::Level::INFO, "L_MovingBaseTest", "net");
    add(logs::Level::INFO, "L_ActionManagerTimerTest", "net");
    add(logs::Level::INFO, "L_AsservLineRotateTest", "net");
    add(logs::Level::INFO, "TestAction", "net");
    add(logs::Level::INFO, "TestTimer", "net");
    add(logs::Level::INFO, "TestActionRun", "net");

    add(logs::Level::INFO, "L_Asserv_FindPIDTest", "net");
    add(logs::Level::INFO, "L_Asserv_SetResolutionTest", "net");
    add(logs::Level::INFO, "L_Asserv_SquareTest", "net");
    add(logs::Level::INFO, "L_AsservEsialTest", "net");
    add(logs::Level::INFO, "L_AsservRunTest", "net");
    add(logs::Level::INFO, "L_IATest", "net");
    add(logs::Level::INFO, "L_ServoObjectTest", "net");
    add(logs::Level::INFO, "L_Asserv_CalageTest", "net");

    //DRIVERS SIMU
    add(logs::Level::INFO, "LedDriver.SIMU", "net");
    add(logs::Level::INFO, "ButtonDriver.SIMU", "net");
    add(logs::Level::INFO, "SoundDriver.SIMU", "net");
    add(logs::Level::INFO, "SwitchDriver.SIMU", "net");
    add(logs::Level::INFO, "AsservDriver.SIMU", "net");
    add(logs::Level::INFO, "LcdShieldDriver.SIMU", "net");

    //add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");
    //add(logs::Level::INFO, "AsservDriverMemory.SIMU", "console");

    //DRIVERS EV3
    add(logs::Level::INFO, "LedDriver.EV3", "net");
    add(logs::Level::INFO, "ButtonDriver.EV3", "net");
    add(logs::Level::INFO, "SoundDriver.EV3", "net");
    add(logs::Level::INFO, "SwitchDriver.EV3", "net");
    add(logs::Level::INFO, "AsservDriver.EV3", "net");
    add(logs::Level::INFO, "ServoDriver.EV3", "net");
    add(logs::Level::INFO, "ServoMotorExtend.EV3", "net");
    add(logs::Level::INFO, "SensorsDriver.EV3", "net");
    add(logs::Level::INFO, "LegoIRSensor.EV3", "net");
    add(logs::Level::INFO, "LegoAngleSensor.EV3", "net");
    add(logs::Level::INFO, "LegoMotor.EV3", "net");
    add(logs::Level::INFO, "LcdShieldDriver.EV3", "net");

    add(logs::Level::INFO, "ActionManagerTimer", "net");
    add(logs::Level::INFO, "MovingBase", "net");

    add(logs::Level::INFO, "Asserv", "net");
    //add(logs::Level::INFO, "AsservInsa", "console");
    add(logs::Level::INFO, "AsservEsialR", "net");

    //DEBUG

}
