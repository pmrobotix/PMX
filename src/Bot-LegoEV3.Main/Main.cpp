#include <unistd.h>

#include "../Bot-LegoEV3/LegoEV3AsservInsaTest.hpp"
#include "../Bot-LegoEV3/LegoEV3ButtonBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3FindPIDTest.hpp"
#include "../Bot-LegoEV3/LegoEV3LedBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3MovingBaseTest.hpp"
#include "../Bot-LegoEV3/LegoEV3RobotExtended.hpp"
#include "../Bot-LegoEV3/LegoEV3SoundBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3TiretteTest.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new LegoEV3LedBarTest());
	robot.getConsoleManager().add(new LegoEV3ButtonBarTest());
	robot.getConsoleManager().add(new LegoEV3SoundBarTest());
	robot.getConsoleManager().add(new LegoEV3TiretteTest());
	robot.getConsoleManager().add(new LegoEV3MovingBaseTest());
	robot.getConsoleManager().add(new LegoEV3AsservInsaTest());
	robot.getConsoleManager().add(new LegoEV3FindPIDTest());

	robot.parseConsoleArgs(argc, argv);

	//start the Robot (functional tests or match)
	robot.begin(argc, argv);

	sleep(1); //TODO wait and verify end of logger ?
	//cout << "End LegoEV3 robot." << endl;
	return 0;
}
