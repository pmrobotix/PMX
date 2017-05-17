#include "O_AsservTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.getArgs().addArgument("d", "dist en mm");
	robot.getArgs().addArgument("a", "angle en degrees", "0");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void O_AsservTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	Arguments args = robot.getArgs();

	float d = 0.0;
	if (args["d"] != "0")
	{
		d = atof(args["d"].c_str());
		logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
	}
	float a = 0.0;
	if (args["a"] != "0")
		{
			a = atof(args["a"].c_str());
		logger().info() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
	}

	logger().info() << "Start Asserv " << logs::end;
	robot.asserv().startMotionTimerAndOdo(true);
	robot.setMyColor(PMXBLUE);
	robot.asserv().setPositionAndColor(100.0, 100.0, 0.0, (robot.getMyColor() == PMXBLUE));
	RobotPosition p = robot.asserv().pos_getPosition();
	logger().info() << "p= " << p.x << " " << p.y << " " << p.theta << " " << p.asservStatus
			<< logs::end;
	robot.svgPrintPosition();
/*
	logger().info() << "GO distance mm=" << d << logs::end;
	if (robot.asserv().doLineAbs(d) != TRAJ_OK)
	{
		logger().info() << "Interruption !!" << logs::end;
	}*/

	p = robot.asserv().pos_getPosition();
	logger().info() << "p= " << p.x << " " << p.y << " " << p.theta << " " << p.asservStatus
			<< logs::end;
	robot.svgPrintPosition();

	logger().info() << "GO turn angle=" << a << logs::end;
	if (robot.asserv().doRotateAbs(a) != TRAJ_OK)
	{
		logger().info() << "Interruption !!" << logs::end;
	}

	p = robot.asserv().pos_getPosition();
	logger().info() << "p= " << p.x << " " << p.y << " " << p.theta << " " << p.asservStatus
			<< logs::end;
	robot.svgPrintPosition();

	logger().info() << "Stopping Robot... " << logs::end;
	robot.asserv().stopMotionTimerAndOdo();
	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

