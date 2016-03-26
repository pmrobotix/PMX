#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../Bot-LegoEV3/LegoEV3AsservInsaTest.hpp"
#include "../Bot-LegoEV3/LegoEV3ButtonBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3LedBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3MovingBaseTest.hpp"
#include "../Bot-LegoEV3/LegoEV3RobotExtended.hpp"
#include "../Bot-LegoEV3/LegoEV3SoundBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3TiretteTest.hpp"
#include "../Common/ConsoleKeyInput.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"

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

	robot.parseConsoleArgs(argc, argv);

	//pause s'il n'y a pas tous les elements pour garder le log d'erreur
	Arguments &args = robot.getArgs();
	if (args["type"] == "0")
	{
		//------------- Pour debug
		char cInput;
		Robot::logger().info() << "Press Enter key to continue ..." << logs::end;
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
		//---------------fin Pour debug
	}

	//start the Robot (functional tests or match)
	robot.begin();

	sleep(1); //TODO wait and verify end of logger ?
	//cout << "End LegoEV3 robot." << endl;
	return 0;
}
