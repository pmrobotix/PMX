#include <unistd.h>

#include "../Bot-LegoEV3/L_ActionManagerTimerTest.hpp"
#include "../Bot-LegoEV3/L_Asserv_FindPIDTest.hpp"
#include "../Bot-LegoEV3/L_Asserv_SetResolutionTest.hpp"
#include "../Bot-LegoEV3/L_Asserv_SquareTest.hpp"
#include "../Bot-LegoEV3/L_AsservInsaTest.hpp"
#include "../Bot-LegoEV3/L_AsservRunTest.hpp"
#include "../Bot-LegoEV3/L_ButtonBarTest.hpp"
#include "../Bot-LegoEV3/L_IATest.hpp"
#include "../Bot-LegoEV3/L_LedBarTest.hpp"
#include "../Bot-LegoEV3/L_MovingBaseTest.hpp"
#include "../Bot-LegoEV3/L_ServoUsingMotorTest.hpp"
#include "../Bot-LegoEV3/L_SoundBarTest.hpp"
#include "../Bot-LegoEV3/L_TiretteTest.hpp"
#include "../Bot-LegoEV3/LegoEV3RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new L_LedBarTest());
	robot.getConsoleManager().add(new L_ButtonBarTest());
	robot.getConsoleManager().add(new L_SoundBarTest());
	robot.getConsoleManager().add(new L_TiretteTest());
	robot.getConsoleManager().add(new L_MovingBaseTest());
	robot.getConsoleManager().add(new L_Asserv_SetResolutionTest());
	robot.getConsoleManager().add(new L_Asserv_FindPIDTest());
	robot.getConsoleManager().add(new L_Asserv_SquareTest());
	robot.getConsoleManager().add(new L_AsservInsaTest());
	robot.getConsoleManager().add(new L_AsservRunTest());
	robot.getConsoleManager().add(new L_ServoUsingMotorTest());
	robot.getConsoleManager().add(new L_IATest());
	robot.getConsoleManager().add(new L_ActionManagerTimerTest());

	robot.parseConsoleArgs(argc, argv);

	//start the Robot (functional tests or match)
	robot.begin(argc, argv);

	sleep(1); //TODO wait and verify end of logger ?
	//cout << "End LegoEV3 robot." << endl;
	return 0;
}
