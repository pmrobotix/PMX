#include "O_AsservLineRotateTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservLineRotateTest::configureConsoleArgs(int argc, char **argv) //surcharge
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	robot.getArgs().addArgument("d", "distance mm");
	robot.getArgs().addArgument("a", "angle degres", "0");
	robot.getArgs().addArgument("back", "backwards[0,1]", "0");

	robot.getArgs().addArgument("d2", "distance mm", "0");
	robot.getArgs().addArgument("a2", "angle degres", "0");
	robot.getArgs().addArgument("back2", "backwards[0,1]", "0");

	robot.getArgs().addArgument("d3", "distance mm", "0");
	robot.getArgs().addArgument("a3", "angle degres", "0");
	robot.getArgs().addArgument("back3", "backwards[0,1]", "0");

	robot.getArgs().addArgument("d4", "distance mm", "0");
	robot.getArgs().addArgument("a4", "angle degres", "0");
	robot.getArgs().addArgument("back4", "backwards[0,1]", "0");

	//mode de drive
	Arguments::Option cOptMode('m', "mode used for test");
	cOptMode.addArgument("mode", "mode number", "0");
	robot.getArgs().addOption(cOptMode);

	//speed
	Arguments::Option cOptSpeed('s', "speed en %");
	cOptSpeed.addArgument("speed", "speed en %", "40");
	robot.getArgs().addOption(cOptSpeed);

	//detection adv
	Arguments::Option cOptdetect('B', "Detection Balise");
	cOptdetect.addArgument("detection", "[0-1]", "1");
	robot.getArgs().addOption(cOptdetect);

	Arguments::Option cOpt('+', "Coordinates x,y,a");
	cOpt.addArgument("coordx", "coord x mm", "300.0");
	cOpt.addArgument("coordy", "coord y mm", "300.0");
	cOpt.addArgument("coorda", "coord teta deg", "0.0"); //TODO ATTENTION ERROR SI different de ZERO !!!
	robot.getArgs().addOption(cOpt);

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void O_AsservLineRotateTest::run(int argc, char **argv)
{
	logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	float d = 0.0;
	float a = 0.0;
	bool back = false;

	float d2 = 0.0;
	float a2 = 0.0;
	bool back2 = false;

	float d3 = 0.0;
	float a3 = 0.0;
	bool back3 = false;

	float d4 = 0.0;
	float a4 = 0.0;
	bool back4 = false;

	int B = 0;
	int m = 0;
	int s = 0;
	float coordx = 0.0;
	float coordy = 0.0;
	float coorda_deg = 0.0;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	Arguments args = robot.getArgs();

	if (args["d"] != "0")
	{
		d = atof(args["d"].c_str());
		logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
	}
	if (args["a"] != "0")
	{
		a = atof(args["a"].c_str());
		logger().info() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
	}
	if (args["back"] != "0")
	{
		back = atoi(args["back"].c_str());
		logger().info() << "Arg back set " << args["back"] << ", back = " << back << logs::end;
	}
	if (args["d2"] != "0")
	{
		d2 = atof(args["d2"].c_str());
		logger().info() << "Arg d2 set " << args["d2"] << ", d2 = " << d2 << logs::end;
	}
	if (args["a2"] != "0")
	{
		a2 = atof(args["a2"].c_str());
		logger().info() << "Arg a2 set " << args["a2"] << ", a2 = " << a2 << logs::end;
	}
	if (args["back2"] != "0")
	{
		back2 = atoi(args["back2"].c_str());
		logger().info() << "Arg back2 set " << args["back2"] << ", back2 = " << back2 << logs::end;
	}

	if (args["d3"] != "0")
	{
		d3 = atof(args["d3"].c_str());
		logger().info() << "Arg d3 set " << args["d3"] << ", d3 = " << d3 << logs::end;
	}
	if (args["a3"] != "0")
	{
		a3 = atof(args["a3"].c_str());
		logger().info() << "Arg a3 set " << args["a3"] << ", a3 = " << a3 << logs::end;
	}
	if (args["back3"] != "0")
	{
		back3 = atoi(args["back3"].c_str());
		logger().info() << "Arg back3 set " << args["back3"] << ", back3 = " << back3 << logs::end;
	}

	if (args["d4"] != "0")
	{
		d4 = atof(args["d4"].c_str());
		logger().info() << "Arg d4 set " << args["d4"] << ", d4 = " << d4 << logs::end;
	}
	if (args["a4"] != "0")
	{
		a4 = atof(args["a4"].c_str());
		logger().info() << "Arg a4 set " << args["a4"] << ", a4 = " << a4 << logs::end;
	}
	if (args["back4"] != "0")
	{
		back4 = atoi(args["back4"].c_str());
		logger().info() << "Arg back4 set " << args["back4"] << ", back4 = " << back4 << logs::end;
	}

	B = atoi(args['B']["detection"].c_str());
	logger().info() << "Arg B set " << args['B']["detection"] << ", B = " << B << logs::end;

	s = atoi(args['s']["speed"].c_str());
	logger().info() << "Arg s set " << args['s']["speed"] << ", s = " << s << logs::end;

	m = atoi(args['m']["mode"].c_str());
	logger().info() << "Arg m set " << args['m']["mode"] << ", m = " << m << logs::end;

	coordx = atof(args['+']["coordx"].c_str());
	coordy = atof(args['+']["coordy"].c_str());
	coorda_deg = atof(args['+']["coorda"].c_str());
	logger().info() << "COORD avec cx=" << coordx << " cy=" << coordy << " coorda=" << coorda_deg << logs::end;

	robot.asserv().startMotionTimerAndOdo(false);
	robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (bool) (robot.getMyColor() != PMXYELLOW));
	robot.asserv().assistedHandling();

	ROBOTPOSITION p = robot.asserv().pos_getPosition();
	logger().info() << "time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; " << " px=" << p.x << " py="
			<< p.y << " pa_deg=" << p.theta * 180.0 / M_PI << logs::end;

	robot.svgPrintPosition();

	robot.actions().start();
	if (B == 1)
	{
		//detection adverse
		robot.actions().sensors().addTimerSensors(62);

		robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
		robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
	} else
	{
		robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
		robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
	}
	robot.chrono().start();

	//vitesse reduite
//    robot.asserv().setLowSpeedForward(true, s);
//    robot.asserv().setLowSpeedBackward(true, s);

	robot.asserv().setMaxSpeed(true, s, s);

	//Definition du path
	//bool byPathfinding = false;

	TRAJ_STATE ts = TRAJ_OK;
	if (B == 1)
	{
		robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
		robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
	}

	if (m == 0)
	{
		if (back == 0)
		{
			if (d != 0)
			{
				ts = robot.asserv().doLine(d);
				robot.svgPrintPosition();
			}
		} else //En arrière (la demande d'angle relatif ne change pas)
		{
			if (d != 0)
			{
				ts = robot.asserv().doLine(-d);
				robot.svgPrintPosition();
			}
		}
		if (a != 0)
		{
			ts = robot.asserv().doRelativeRotateDeg(a);
			robot.svgPrintPosition();
		}

		if (back2 == 0)
		{
			if (d2 != 0)
			{
				ts = robot.asserv().doLine(d2);
				robot.svgPrintPosition();
			}

		} else //En arrière (la demande d'angle relatif ne change pas)
		{
			if (d2 != 0)
			{
				ts = robot.asserv().doLine(-d2);
				robot.svgPrintPosition();
			}
		}
		if (a2 != 0)
		{
			ts = robot.asserv().doRelativeRotateDeg(a2);
			robot.svgPrintPosition();
		}

		if (back3 == 0)
		{
			if (d3 != 0)
			{
				ts = robot.asserv().doLine(d3);
				robot.svgPrintPosition();
			}

		} else //En arrière (la demande d'angle relatif ne change pas)
		{
			if (d3 != 0)
			{
				ts = robot.asserv().doLine(-d3);
				robot.svgPrintPosition();
			}
		}
		if (a3 != 0)
		{
			ts = robot.asserv().doRelativeRotateDeg(a3);
			robot.svgPrintPosition();
		}

		if (back4 == 0)
		{
			if (d4 != 0)
			{
				ts = robot.asserv().doLine(d4);
				robot.svgPrintPosition();
			}

		} else //En arrière (la demande d'angle relatif ne change pas)
		{
			if (d4 != 0)
			{
				ts = robot.asserv().doLine(-d4);
				robot.svgPrintPosition();
			}
		}
		if (a4 != 0)
		{
			ts = robot.asserv().doRelativeRotateDeg(a4);
			robot.svgPrintPosition();
		}

	} else if (m == 1)
	{

	}

	/*
	 for (int num = 1; num <= nb; num++)
	 {
	 logger().info() << "Go essai num= " << num << " / " << nb << logs::end;
	 ts = TRAJ_OK;
	 if (d != 0)
	 {
	 logger().info() << "go ...d=" << d << "mm" << logs::end;

	 //calcul de coord
	 float x_dest = robot.asserv().pos_getX_mm() + cos(robot.asserv().pos_getTheta()) * d;
	 float y_dest = robot.asserv().pos_getY_mm() + sin(robot.asserv().pos_getTheta()) * d;
	 if (back == 0)
	 {
	 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	 bool frontcenter = robot.actions().sensors().getAvailableFrontCenter();
	 logger().info() << "frontcenter=" << frontcenter << " " << logs::end;

	 ts = robot.ia().iAbyPath().whileMoveForwardTo(x_dest, y_dest, false, 1000000, 5, 10, false);
	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().info() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
	 //robot.asserv().stopMotors();


	 logger().info() << "Attente 3 sec!" << logs::end;
	 utils::sleep_for_secs(3);
	 robot.asserv().resetEmergencyOnTraj("===== TRAJ_NEAR_OBSTACLE CONFIRMED");

	 }
	 if (ts == TRAJ_COLLISION)
	 {
	 logger().info() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
	 robot.asserv().stopMotors();


	 logger().info() << "Attente 3 sec!" << logs::end;
	 utils::sleep_for_secs(3);
	 robot.asserv().resetEmergencyOnTraj("===== COLLISION ASSERV CONFIRMED");
	 }
	 if ((ts != TRAJ_INTERRUPTED) && (ts != TRAJ_COLLISION) && (ts != TRAJ_FINISHED))
	 {
	 logger().info() << "=====> cas pas normal ts=" << ts << logs::end;
	 //robot.asserv().stopMotors();
	 robot.asserv().resetEmergencyOnTraj("===== OTHER CASE CONFIRMED");

	 }

	 } else
	 {

	 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
	 robot.actions().sensors().setIgnoreBackNearObstacle(true, false, true);

	 ts = robot.ia().iAbyPath().whileMoveBackwardTo(robot.asserv().pos_getX_mm() + d,
	 robot.asserv().pos_getY_mm(), false, 1000000, 3, 2, false);
	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().info() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj("==== TRAJ_NEAR_OBSTACLE CONFIRMED");
	 //robot.asserv().stopMotors();
	 }
	 if (ts == TRAJ_COLLISION)
	 {
	 logger().info() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj("===== COLLISION ASSERV CONFIRMED");
	 //robot.asserv().stopMotors();
	 }

	 }
	 }
	 ts = TRAJ_OK;
	 if (a != 0.0)
	 {
	 logger().info() << "go Rotate..." << a << " deg" << logs::end;
	 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
	 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	 ts = robot.ia().iAbyPath().whileMoveRotateTo(a, 1000000, 2);

	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().error() << "===== TRAJ_NEAR_OBSTACLE FINAL" << logs::end;
	 robot.asserv().resetEmergencyOnTraj("rotate ===== TRAJ_NEAR_OBSTACLE FINAL");
	 //robot.asserv().stopMotors();
	 }
	 if (ts == TRAJ_COLLISION)
	 {
	 logger().error() << "===== COLLISION ASSERV FINAL" << logs::end;
	 robot.asserv().resetEmergencyOnTraj("rotate ===== COLLISION ASSERV FINAL");
	 //robot.asserv().stopMotors();
	 }

	 robot.svgPrintPosition();
	 }
	 ts = TRAJ_OK;
	 if (!(x == 0.0 && y == 0.0))
	 {
	 if (!back)
	 {

	 logger().info() << "go Forward... x=" << x << ", y=" << y << logs::end;
	 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	 ts = robot.ia().iAbyPath().whileMoveForwardTo(x, y, true, 1000000, 20, 2);

	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().error() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj(
	 "robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_NEAR_OBSTACLE");
	 //robot.asserv().stopMotors();
	 }
	 if (ts == TRAJ_COLLISION)
	 {
	 logger().error() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj(
	 "robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_COLLISION");
	 //robot.asserv().stopMotors();
	 }

	 robot.svgPrintPosition();
	 } else
	 {

	 logger().info() << "go Backward... x=" << x << ", y=" << y << logs::end;
	 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
	 robot.actions().sensors().setIgnoreBackNearObstacle(true, false, true);

	 ts = robot.ia().iAbyPath().whileMoveBackwardTo(x, y, true, 1000000, 20, 3);

	 //TODO TRAJ_NEAR_OBSTACLE or TRAJ_NEAR_OBSTACLE_REAR ???
	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().error() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj(
	 "robot.ia().iAbyPath().whileMoveBackwardTo FINAL TRAJ_NEAR_OBSTACLE");
	 //robot.asserv().stopMotors();
	 }
	 if (ts == TRAJ_COLLISION)
	 {
	 logger().error() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj(
	 "robot.ia().iAbyPath().whileMoveBackwardTo FINAL TRAJ_COLLISION");
	 //robot.asserv().stopMotors();
	 }

	 robot.svgPrintPosition();
	 }
	 }
	 ts = TRAJ_OK;
	 if (a != 0)
	 {
	 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
	 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	 ts = robot.ia().iAbyPath().whileMoveRotateTo(a, 1000000, 2);

	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().error() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj();
	 }
	 if (ts == TRAJ_COLLISION)
	 {
	 logger().error() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
	 robot.asserv().resetEmergencyOnTraj();
	 }

	 robot.svgPrintPosition();
	 }
	 //pour la repetition
	 //d += d;
	 }
	 */
	robot.asserv().stopMotors();
	robot.svgPrintPosition();

	robot.asserv().freeMotion();
	robot.asserv().setMaxSpeed(false);

	p = robot.asserv().pos_getPosition();
	logger().info() << "time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; " << " x=" << p.x << " y=" << p.y
			<< " deg=" << p.theta * 180.0 / M_PI << logs::end;

	robot.svgPrintPosition();
	robot.svgPrintEndOfFile();
	logger().info() << "Happy End." << logs::end;
}

