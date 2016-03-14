#include <unistd.h>
#include <iostream>

#include "../Bot-BigPMX/BLedBarTest.hpp"
#include "../Bot-BigPMX/BRobotExtended.hpp"

#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	cout << "!!! Hello BigPMX !!!" << endl;

	//Specific Robot BigPMX
	BRobotExtended &robot = BRobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new BLedBarTest());

	robot.parseConsoleArgs(argc, argv);

	//start the Robot (functional tests or match)
	robot.begin();

	sleep(1); //TODO wait and verify end of logger ?
	//cout << "End BigPMX." << endl;
	return 0;
}
