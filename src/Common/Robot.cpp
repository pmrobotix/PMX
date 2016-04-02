/*
 * Robot.cpp
 * test
 *
 */

#include "Robot.hpp"

#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "../Log/Logger.hpp"
#include "Action/Actions.hpp"
#include "Asserv/Asserv.hpp"
#include "ConsoleKeyInput.hpp"

using namespace std;

Robot::Robot()
		: myColor_(PMXNOCOLOR), cArgs_("", "(c) PM-ROBOTIX 2016", "-/")
{
	actions_default = new Actions();
	asserv_default = new Asserv("RobotDefaultAsserv");

	configureDefaultConsoleArgs();
}

void Robot::configureDefaultConsoleArgs()
{
	// Add option "-h" with explanation...
	cArgs_.addOption('h', "Display usage help");
	cArgs_.addOption('s', "skip setup");

	cArgs_.addArgument("type", "Type of match (t)est/(m)atch", "0");
	{
		Arguments::Option cOpt('n', "");
		cOpt.addArgument("num", "number of the functional test");
		cArgs_.addOption(cOpt);
	}
	{
		Arguments::Option cOpt('c', "");
		cOpt.addArgument("color", "color of robot green/red", "red");
		cArgs_.addOption(cOpt);
	}
}

void Robot::parseConsoleArgs(int argc, char** argv)
{

	if (!cArgs_.parse(argc, argv))
	{
		logger().error() << "Error parsing" << logs::end;
		exit(-1);
	}

	// request option "h" and print out manual if set...
	if (cArgs_['h'])
	{
		cArgs_.usage();
		std::cout << "Available functional tests: " << std::endl;
		cmanager_.displayAvailableTests("", -1);
		exit(0);
	}
}

void Robot::begin(int argc, char** argv)
{
	int num = -1;
	string select = "-";
	string color = "-";

	if (cArgs_["type"] == "0")
	{
		//display all functional tests
		cmanager_.displayAvailableTests("", -1);

		if (!cArgs_['s'])
		{
			//------------- Pour debug
			//pause s'il n'y a pas tous les elements pour garder le log d'erreur
			char cInput;
			Robot::logger().info() << "Press Enter key to continue ..." << logs::end;
			//cout << "Press Enter key to continue ..." << endl;
			//sleep(1);
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
	}
	logger().debug() << "" << logs::end;
	logger().debug() << "" << logs::end;
	logger().debug() << "type = " << cArgs_["type"] << logs::end;

	logger().debug() << "Option c set "
			<< (int) cArgs_['c']
			<< ", color = "
			<< " "
			<< cArgs_['c']["color"]
			<< logs::end;

	if (cArgs_['c'])
	{
		color = cArgs_['c']["color"];
		if (color == "green")
			this->setMyColor(PMXGREEN);
		else if (color == "red")
			this->setMyColor(PMXVIOLET);
		else
		{
			this->setMyColor(PMXNOCOLOR);
			logger().error() << "setMyColor(NOCOLOR)" << logs::end;
			exit(-1);
		}
	}

	//test number
	if (cArgs_['n'])
	{
		num = atoi(cArgs_['n']["num"].c_str());
		logger().debug() << "Option n set " << (int) cArgs_['n'] << ", num = " << num << logs::end;
	}

	//skip state
	if (cArgs_['s'])
	{
		logger().debug() << "skip = " << (int) cArgs_['s'] << logs::end;
	}

	if (cArgs_["type"] != "m" && cArgs_["type"] != "t" && cArgs_["type"] != "T" && cArgs_["type"] != "M")
	{
		select = cmanager_.displayMenuFirstArgu();
		if (select == "-")
		{
			logger().error() << "displayMenuFirstArgu bad return " << logs::end;
			exit(-1);
		}
		cArgs_["type"] = select;
	}

	//functional test cases
	if (cArgs_["type"] == "t" or cArgs_["type"] == "T")
	{
		if (num > 0)
		{
			//execute defined test
			//cmanager_.run(num, &cArgs_);
			cmanager_.run(num, argc, argv);
		}
		else
		{
			//ask param
			cmanager_.displayMenuFunctionalTestsAndRun(argc, argv);
		}
	}
}

void Robot::stop()
{
	this->asserv_default->stopMotionTimerAndOdo();

}

