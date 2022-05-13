#include <unistd.h>

#include "../Bot-LegoEV3/l_ServoStepTest.hpp"
#include "../Bot-LegoEV3/L_ActionManagerTimerTest.hpp"
#include "../Bot-LegoEV3/L_Asserv_CalageTest.hpp"
#include "../Bot-LegoEV3/L_Asserv_SquareTest.hpp"
#include "../Bot-LegoEV3/L_AsservEsialTest.hpp"
#include "../Bot-LegoEV3/L_AsservLineRotateTest.hpp"
#include "../Bot-LegoEV3/L_AsservRunTest.hpp"
#include "../Bot-LegoEV3/L_ButtonBarTest.hpp"
#include "../Bot-LegoEV3/L_IAbyPathTest.hpp"
#include "../Bot-LegoEV3/L_IATest.hpp"
#include "../Bot-LegoEV3/L_LcdTest.hpp"
#include "../Bot-LegoEV3/L_LedBarTest.hpp"
#include "../Bot-LegoEV3/L_MovingBaseTest.hpp"
#include "../Bot-LegoEV3/L_SensorsTest.hpp"
#include "../Bot-LegoEV3/L_ServoObjectTest.hpp"
#include "../Bot-LegoEV3/L_ServoUsingMotorTest.hpp"
#include "../Bot-LegoEV3/L_SoundBarTest.hpp"
#include "../Bot-LegoEV3/L_TiretteTest.hpp"
#include "../Bot-LegoEV3/LegoEV3RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
    utils::set_realtime_priority(5, "Main"); //set priority MAX 99

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();



    //add specific tests for this robot
    robot.getConsoleManager().add(new L_LedBarTest());
    robot.getConsoleManager().add(new L_ButtonBarTest());
    robot.getConsoleManager().add(new L_SoundBarTest());
    robot.getConsoleManager().add(new L_TiretteTest());
    robot.getConsoleManager().add(new L_LcdTest());
    robot.getConsoleManager().add(new L_SensorsTest());
    robot.getConsoleManager().add(new L_ServoObjectTest());
    robot.getConsoleManager().add(new L_ServoStepTest());
    robot.getConsoleManager().add(new L_ServoUsingMotorTest());
    robot.getConsoleManager().add(new L_MovingBaseTest());
    robot.getConsoleManager().add(new L_AsservLineRotateTest());
    robot.getConsoleManager().add(new L_Asserv_SquareTest());
    robot.getConsoleManager().add(new L_AsservEsialTest());
    robot.getConsoleManager().add(new L_AsservRunTest());
    robot.getConsoleManager().add(new L_IATest());
    robot.getConsoleManager().add(new L_IAByPathTest());
    robot.getConsoleManager().add(new L_ActionManagerTimerTest());
    robot.getConsoleManager().add(new L_Asserv_CalageTest());

    //parse default to retrieve default parameter (t or m)
    robot.parseConsoleArgs(argc, argv, false);

    //start the Robot (functional tests or match)
    robot.begin(argc, argv);

    //cout << "End LegoEV3 robot." << endl;
    return 0;
}
