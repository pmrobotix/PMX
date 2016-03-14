#include <unistd.h>

#include "../Bot-SmallPMX/SButtonBarTest.hpp"
#include "../Bot-SmallPMX/SLedBarTest.hpp"
#include "../Bot-SmallPMX/SRobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//cout << "!!!Hello SmallPMX EV3!!!" << endl;

	//SRobotExtended robot;
	SRobotExtended &robot = SRobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new SLedBarTest());
	robot.getConsoleManager().add(new SButtonBarTest());

	robot.parseConsoleArgs(argc, argv);

	//start the Robot (functional tests or match)
	robot.begin();

	sleep(1); //TODO wait and verify end of logger ?
	//cout << "End SmallPMX EV3." << endl;
	return 0;
}
