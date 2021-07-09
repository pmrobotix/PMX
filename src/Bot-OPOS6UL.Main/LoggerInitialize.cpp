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


void logs::LoggerFactory::setPriority()
{
    //set minimum thread priority FIFO for Logs
    utils::set_realtime_priority(1);
}
void logs::LoggerFactory::initialize()
{

    add("console", new ConsoleAppender());
    add("svg", new SvgAppender("svgAPF.svg"));
    add("svgia", new SvgAppender("svgIA.svg"));
    add("memory", new MemoryAppender());
    //add("file", new FileAppender("logAPF.csv"));
    add("net", new TelemetryAppender("MSG|BIG"));

    add(logs::Level::ERROR, "", "net");

    add(logs::Level::INFO, "Robot", "net");
    add(logs::Level::INFO, "OPOS6UL_RobotExtended", "net");
    add(logs::Level::INFO, "OPOS6UL_AsservExtended", "net");

    //COMMON
    add(logs::Level::INFO, "Asserv", "net");
    add(logs::Level::INFO, "IAbyPath", "net");
    add(logs::Level::INFO, "SensorsTimer", "net");
    add(logs::Level::INFO, "Sensors", "net");
    add(logs::Level::INFO, "ServoObjectsSystem", "net");
    add(logs::Level::INFO, "ActionManagerTimer", "net");

    //SVG
    add(logs::Level::INFO, "Svg4OPOS6UL_Robot", "svg");
    add(logs::Level::INFO, "AsservDriver.OPO.SVG", "svg");
    add(logs::Level::INFO, "AsservDriver.SIMU.SVG", "svg");
    add(logs::Level::INFO, "IAbyPath4OPOS6UL_Robot", "svgia");

    //File
    //add(logs::Level::DEBUG, "logFileAsservInsa", "file");

    //State
    add(logs::Level::INFO, "O_State_Init", "net");
    add(logs::Level::INFO, "O_State_DecisionMakerIA", "net");
    add(logs::Level::INFO, "O_State_WaitEndOfMatch", "net");

    //TESTS
    add(logs::Level::INFO, "O_ActionManagerTimerTest", "net");
    add(logs::Level::INFO, "O_TiretteTest", "net");
    add(logs::Level::INFO, "O_Asserv_CalageTest", "net");
    add(logs::Level::INFO, "O_Asserv_SquareTest", "net");
    add(logs::Level::INFO, "O_LedBarTest", "net");
    add(logs::Level::INFO, "O_AsservEsialTest", "net");
    add(logs::Level::INFO, "O_AsservLineRotateTest", "net");
    add(logs::Level::INFO, "O_AsservTest", "net");
    add(logs::Level::INFO, "O_ButtonBarTest", "net");
    add(logs::Level::INFO, "O_GroveColorTest", "net");
    add(logs::Level::INFO, "O_IAByPathTest", "net");
    add(logs::Level::INFO, "O_LcdBoardTest", "net");
    add(logs::Level::INFO, "O_SensorsTest", "net");
    add(logs::Level::INFO, "O_ServoObjectsTest", "net");
    add(logs::Level::INFO, "O_ServoStepTest", "net");

    //DRIVERS SIMU
    add(logs::Level::INFO, "LedDriver.SIMU", "net");
    add(logs::Level::INFO, "ButtonDriver.SIMU", "net");
    add(logs::Level::INFO, "SoundDriver.SIMU", "net");
    add(logs::Level::INFO, "SwitchDriver.SIMU", "net");
    add(logs::Level::INFO, "AsservDriver.SIMU", "net");
    add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");
    add(logs::Level::INFO, "LcdShieldDriver.SIMU", "memory");

    //DRIVERS OPOS
    add(logs::Level::INFO, "LedDriver.OPO", "net");
    add(logs::Level::INFO, "ButtonDriver.OPO", "net");
    add(logs::Level::INFO, "SoundDriver.OPO", "net");
    add(logs::Level::INFO, "SwitchDriver.OPO", "net");
    add(logs::Level::INFO, "SensorsDriver.OPO", "net");
    add(logs::Level::INFO, "AsservDriver.OPO", "net");
    add(logs::Level::INFO, "CCAx12Adc.OPO", "net");
    add(logs::Level::INFO, "SensorsDriver.OPO", "net");
    add(logs::Level::INFO, "IrSensor.OPO", "net");
    add(logs::Level::INFO, "BeaconSensors.OPO", "net");


}
