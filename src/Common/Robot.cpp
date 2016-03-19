/*
 * Robot.cpp
 * test
 *
 */

#include "Robot.hpp"

#include <cstdlib>

#include "../Log/Logger.hpp"
#include "Action/Actions.hpp"
#include "Asserv/Asserv.hpp"

using namespace std;

Robot::Robot() :
		myColor_(PMXNOCOLOR), cArgs_("", "(c) PM-ROBOTIX 2016", "-/")
{
	actions_default = new Actions();

	asserv_default = new Asserv();

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
		exit(0);
	}
}

void Robot::begin()
{
	int num = -1;
	string select = "-";
	string color = "-";

	logger().debug() << "type = " << cArgs_["type"] << logs::end;
	logger().debug() << "Option c set " << (int) cArgs_['c'] << ", color = " << " " << cArgs_['c']["color"] << logs::end;

	if (cArgs_['c'])
	{
		color = cArgs_['c']["color"];
		if (color == "green") this->setMyColor(PMXGREEN);
		else if (color == "red") this->setMyColor(PMXRED);
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
		logger().debug() << "Option n set " << (int)cArgs_['n'] << ", num = " << num << logs::end;
	}

	//skip state
	if (cArgs_['s'])
	{
		logger().debug() << "skip = " << (int) cArgs_['s'] << logs::end;
	}

	if (cArgs_["type"] != "m" && cArgs_["type"] != "t")
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
	if (cArgs_["type"] == "t")
	{
		if (num > 0)
		{
			//execute defined test
			cmanager_.run(num, &cArgs_);
		}
		else
		{
			//ask param
			cmanager_.displayMenuFunctionalTestsAndRun(&cArgs_);
		}
	}
}

void Robot::stop()
{
 //TODO stop robot
}

