#include "../Bot-OPOS6UL/O_Asserv_SquareTest.hpp"
#include "../Bot-OPOS6UL/O_AsservTest.hpp"
#include "../Bot-OPOS6UL/O_ButtonBarTest.hpp"
#include "../Bot-OPOS6UL/O_GroveColorTest.hpp"
#include "../Bot-OPOS6UL/O_IAbyPathTest.hpp"
#include "../Bot-OPOS6UL/O_LcdBoardTest.hpp"
#include "../Bot-OPOS6UL/O_LedBarTest.hpp"
#include "../Bot-OPOS6UL/O_ServoStepTest.hpp"
#include "../Bot-OPOS6UL/OPOS6UL_RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//Specific Robot BigPMX
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new O_LedBarTest());
	robot.getConsoleManager().add(new O_ButtonBarTest());
	robot.getConsoleManager().add(new O_LcdBoardTest());
	robot.getConsoleManager().add(new O_GroveColorTest());
	robot.getConsoleManager().add(new O_AsservTest());
	robot.getConsoleManager().add(new O_Asserv_SquareTest());
	robot.getConsoleManager().add(new O_IAByPathTest());
	robot.getConsoleManager().add(new O_ServoStepTest);

	robot.parseConsoleArgs(argc, argv);


	//start the Robot (functional tests or match)
	robot.begin(argc, argv);

	return 0;
}
