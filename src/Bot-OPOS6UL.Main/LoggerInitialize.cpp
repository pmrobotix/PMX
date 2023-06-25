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
    //on ecrase la priorité du loggerFactory
    setPriority(1); //set priority for log per robot !! //99 is high priority

    add("console", new ConsoleAppender());
    add("svg", new SvgAppender("svgAPF.svg"));
    add("svgia", new SvgAppender("svgIA.svg"));
    add("memory", new MemoryAppender());
    //add("file", new FileAppender("logAPF.csv"));


    //order=ERROR>TELEM>WARN>INFO>DEBUG
    //net =TELEMETRY+CONSOLE
    //add("net", new TelemetryAppender("OPOS6UL", "pmx-vm"));
    add(logs::Level::ERROR, "", "console"); //net = TELEMETRY + CONSOLE

    //INFO
    add(logs::Level::INFO, "Robot", "console");
    add(logs::Level::INFO, "OPOS6UL_RobotExtended", "console");
    add(logs::Level::INFO, "OPOS6UL_AsservExtended", "console");
    add(logs::Level::INFO, "OPOS6UL_ActionsExtended", "console");

    //COMMON
    add(logs::Level::INFO, "Asserv", "console");
    add(logs::Level::INFO, "IAbyPath", "console");
    add(logs::Level::INFO, "Sensors", "console");
    add(logs::Level::INFO, "ServoObjectsSystem", "console");
    add(logs::Level::INFO, "ActionManagerTimer", "console");
    add(logs::Level::INFO, "LedBar", "console");

    //SVG
    add(logs::Level::INFO, "Svg4OPOS6UL_Robot", "svg");
    add(logs::Level::INFO, "IAbyPath4OPOS6UL_Robot", "svgia");
    //SVG specific en parallele //TODO ca pose pb de lock
    add(logs::Level::INFO, "AsservDriver.OPO.SVG", "svg");
    add(logs::Level::INFO, "AsservDriver.SIMU.SVG", "svg");


    //State
    add(logs::Level::INFO, "O_State_Init", "console");
    add(logs::Level::INFO, "O_State_DecisionMakerIA", "console");
    add(logs::Level::INFO, "O_State_WaitEndOfMatch", "console");

    //TESTS
    add(logs::Level::INFO, "O_ActionManagerTimerTest", "console");
    add(logs::Level::INFO, "O_ActionManagerTimerTest-Action", "console");
    add(logs::Level::INFO, "O_ActionManagerTimerTest-Timer", "console");
    add(logs::Level::INFO, "O_TiretteTest", "console");
    add(logs::Level::INFO, "O_Asserv_CalageTest", "console");
    add(logs::Level::INFO, "O_Asserv_SquareTest", "console");
    add(logs::Level::INFO, "O_LedBarTest", "console");
    add(logs::Level::INFO, "O_AsservEsialTest", "console");
    add(logs::Level::INFO, "O_AsservLineRotateTest", "console");
    add(logs::Level::INFO, "O_AsservTest", "console");
    add(logs::Level::INFO, "O_ButtonBarTest", "console");
    add(logs::Level::INFO, "O_GroveColorTest", "console");
    add(logs::Level::INFO, "O_IAByPathTest", "console");
    add(logs::Level::INFO, "O_LcdBoardTest", "console");
    add(logs::Level::INFO, "O_SensorsTest", "console");
    add(logs::Level::INFO, "O_ServoObjectsTest", "console");
    add(logs::Level::INFO, "O_ServoStepTest", "console");

    //DRIVERS SIMU
    add(logs::Level::INFO, "LedDriver.SIMU", "console");
    add(logs::Level::INFO, "ButtonDriver.SIMU", "console");
    add(logs::Level::INFO, "SoundDriver.SIMU", "console");
    add(logs::Level::INFO, "SwitchDriver.SIMU", "console");
    add(logs::Level::INFO, "AsservDriver.SIMU", "console");
    add(logs::Level::INFO, "AsservDriverMemory.SIMU", "memory");
    add(logs::Level::INFO, "LcdShieldDriver.SIMU", "memory");
    add(logs::Level::INFO, "SensorsDriver.SIMU", "console");

    //DRIVERS OPOS
    add(logs::Level::INFO, "LedDriver.OPO", "console");
    add(logs::Level::INFO, "ButtonDriver.OPO", "console");
    add(logs::Level::INFO, "SoundDriver.OPO", "console");
    add(logs::Level::INFO, "SwitchDriver.OPO", "console");
    add(logs::Level::INFO, "SensorsDriver.OPO", "console");
    add(logs::Level::INFO, "AsservDriver.OPO", "console");
    add(logs::Level::INFO, "CCAx12Adc.OPO", "console");
    add(logs::Level::INFO, "SensorsDriver.OPO", "console");
    add(logs::Level::INFO, "IrSensor.OPO", "console");
    add(logs::Level::INFO, "BeaconSensors.OPO", "console");


}
