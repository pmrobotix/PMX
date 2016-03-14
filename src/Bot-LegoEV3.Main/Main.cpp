#include <unistd.h>

#include "../Bot-LegoEV3/LegoEV3ButtonBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3LedBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"

using namespace std;

int main(int argc, char** argv)
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//add specific tests for this robot
	robot.getConsoleManager().add(new LegoEV3LedBarTest());
	robot.getConsoleManager().add(new LegoEV3ButtonBarTest());

	/*
	 //Pour debug
	 char cInput;
	 cout << "Press Enter key to continue ...\n" << endl;
	 do
	 {
	 cInput = ConsoleKeyInput::mygetch();
	 switch (cInput)
	 {

	 case 10:
	 //printf("Enter key!\n");
	 break;
	 case 127:
	 cout << "Exit !\n" << endl;
	 //cout << default_console << endl;
	 exit(0);
	 break;
	 }
	 usleep(1000);
	 } while (cInput != 10);
	 */


	robot.parseConsoleArgs(argc, argv);

	//start the Robot (functional tests or match)
	robot.begin();

	sleep(1); //TODO wait and verify end of logger ?
	//cout << "End LegoEV3 robot." << endl;
	return 0;
}
