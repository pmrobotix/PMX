#include "../Bot-OPOS6UL/O_Asserv_CalageTest.hpp"
#include "../Bot-OPOS6UL/O_Asserv_SquareTest.hpp"
#include "../Bot-OPOS6UL/O_AsservEsialTest.hpp"
#include "../Bot-OPOS6UL/O_AsservLineRotateTest.hpp"
#include "../Bot-OPOS6UL/O_AsservTest.hpp"
#include "../Bot-OPOS6UL/O_ButtonBarTest.hpp"
#include "../Bot-OPOS6UL/O_GroveColorTest.hpp"
#include "../Bot-OPOS6UL/O_IAbyPathTest.hpp"
#include "../Bot-OPOS6UL/O_LcdBoardTest.hpp"
#include "../Bot-OPOS6UL/O_LedBarTest.hpp"
#include "../Bot-OPOS6UL/O_SensorsTest.hpp"
#include "../Bot-OPOS6UL/O_ServoObjectsTest.hpp"
#include "../Bot-OPOS6UL/O_ServoStepTest.hpp"
#include "../Bot-OPOS6UL/OPOS6UL_RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
    utils::set_realtime_priority(); //set priority MAX 99

    //Specific Robot BigPMX
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //add specific tests for this robot
    robot.getConsoleManager().add(new O_LedBarTest());
    robot.getConsoleManager().add(new O_ButtonBarTest());
    robot.getConsoleManager().add(new O_LcdBoardTest());
    robot.getConsoleManager().add(new O_GroveColorTest());
    robot.getConsoleManager().add(new O_AsservEsialTest());
    robot.getConsoleManager().add(new O_AsservLineRotateTest());
    robot.getConsoleManager().add(new O_AsservTest());
    robot.getConsoleManager().add(new O_Asserv_SquareTest());
    robot.getConsoleManager().add(new O_Asserv_CalageTest());
    robot.getConsoleManager().add(new O_IAByPathTest());
    robot.getConsoleManager().add(new O_ServoStepTest);
    robot.getConsoleManager().add(new O_ServoObjectsTest);
    robot.getConsoleManager().add(new O_SensorsTest);

    robot.parseConsoleArgs(argc, argv, false);

    //start the Robot (functional tests or match)
    robot.begin(argc, argv);



    return 0;
}
