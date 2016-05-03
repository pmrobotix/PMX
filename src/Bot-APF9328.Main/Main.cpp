#include <unistd.h>
#include <iostream>

#include "../Bot-APF9328/A_Asserv_FindPIDTest.hpp"
#include "../Bot-APF9328/A_Asserv_SetResolutionTest.hpp"
#include "../Bot-APF9328/A_Asserv_SquareTest.hpp"
#include "../Bot-APF9328/A_AsservInsaTest.hpp"
#include "../Bot-APF9328/A_AsservRunTest.hpp"
#include "../Bot-APF9328/A_ButtonBarTest.hpp"
#include "../Bot-APF9328/A_IATest.hpp"
#include "../Bot-APF9328/A_LcdBoardTest.hpp"
#include "../Bot-APF9328/A_LedBarTest.hpp"
#include "../Bot-APF9328/A_MovingBaseTest.hpp"
#include "../Bot-APF9328/A_SensorsTest.hpp"
#include "../Bot-APF9328/A_ServoStepTest.hpp"
#include "../Bot-APF9328/A_ServoTest.hpp"
#include "../Bot-APF9328/A_TiretteTest.hpp"
#include "../Bot-APF9328/APF9328RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//Specific Robot BigPMX
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new A_LedBarTest());
	robot.getConsoleManager().add(new A_ButtonBarTest());
	robot.getConsoleManager().add(new A_LcdBoardTest());
	robot.getConsoleManager().add(new A_TiretteTest());
	robot.getConsoleManager().add(new A_SensorsTest());
	robot.getConsoleManager().add(new A_ServoTest());
	robot.getConsoleManager().add(new A_ServoStepTest());
	robot.getConsoleManager().add(new A_MovingBaseTest());
	robot.getConsoleManager().add(new A_Asserv_SetResolutionTest());
	robot.getConsoleManager().add(new A_Asserv_FindPIDTest());
	robot.getConsoleManager().add(new A_AsservInsaTest());
	robot.getConsoleManager().add(new A_AsservRunTest());
	robot.getConsoleManager().add(new A_Asserv_SquareTest());
	robot.getConsoleManager().add(new A_IATest());

	robot.parseConsoleArgs(argc, argv);

	//start the Robot (functional tests or match)
	robot.begin(argc, argv);

	sleep(1); //TODO wait and verify end of logger ?

	cout << "HappyEnd Bot-APF9328." << endl;
	return 0;
}
