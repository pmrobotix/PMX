#include "O_AsservTest.hpp"

#include <bits/basic_string.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

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

void O_AsservTest::configureConsoleArgs(int argc, char **argv) //surcharge
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	//robot.getArgs().addArgument("nb", "nbr position", "1");
	robot.getArgs().addArgument("s", "speed en %");
	robot.getArgs().addArgument("x", "x mm");
	robot.getArgs().addArgument("y", "y mm");
	robot.getArgs().addArgument("x2", "x2 mm", "0");
	robot.getArgs().addArgument("y2", "y2 mm", "0");
	robot.getArgs().addArgument("x3", "x3 mm", "0");
	robot.getArgs().addArgument("y3", "y3 mm", "0");
	//TODO mettre plus de points x,y

	Arguments::Option cOpt('+', "Coordinates x,y,a");
	cOpt.addArgument("coordx", "coord x mm", "200.0");
	cOpt.addArgument("coordy", "coord y mm", "200.0");
	cOpt.addArgument("coorda", "coord teta mm", "0.0");
	robot.getArgs().addOption(cOpt);

	//reparse again arguments for the specific test
	robot.parseConsoleArgs(argc, argv);

}

void O_AsservTest::run(int argc, char **argv)
{
	logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);
	int s = 0;
	float x = 0.0;
	float y = 0.0;
	float x2 = 0.0;
	float y2 = 0.0;
	float x3 = 0.0;
	float y3 = 0.0;
	float coordx = 0.0;
	float coordy = 0.0;
	float coorda_deg = 0.0;
	TRAJ_STATE ts = TRAJ_OK;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	Arguments args = robot.getArgs();
//    if (args["nb"] != "0") {
//        nb = atoi(args["nb"].c_str());
//        logger().debug() << "Arg nb set " << args["nb"] << ", nb = " << nb << logs::end;
//    }
	if (args["s"] != "0")
	{
		s = atoi(args["s"].c_str());
		logger().debug() << "Arg s set " << args["s"] << ", s = " << s << logs::end;
	}
	if (args["x"] != "0")
	{
		x = atof(args["x"].c_str());
		logger().debug() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
	}
	if (args["y"] != "0")
	{
		y = atof(args["y"].c_str());
		logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
	}

	if (args["x2"] != "0")
	{
		x2 = atof(args["x2"].c_str());
		logger().debug() << "Arg x2 set " << args["x2"] << ", x2 = " << x2 << logs::end;
	}
	if (args["y2"] != "0")
	{
		y2 = atof(args["y2"].c_str());
		logger().debug() << "Arg y2 set " << args["y2"] << ", y2 = " << y2 << logs::end;
	}

	if (args["x3"] != "0")
	{
		x3 = atof(args["x3"].c_str());
		logger().debug() << "Arg x3 set " << args["x3"] << ", x3 = " << x3 << logs::end;
	}
	if (args["y3"] != "0")
	{
		y3 = atof(args["y3"].c_str());
		logger().debug() << "Arg y3 set " << args["y3"] << ", y3 = " << y3 << logs::end;
	}

	coordx = atof(args['+']["coordx"].c_str());
	coordy = atof(args['+']["coordy"].c_str());
	coorda_deg = atof(args['+']["coorda"].c_str());

	logger().info() << "COORD avec x=" << coordx << " y=" << coordy << " a=" << coorda_deg << logs::end;

	robot.asserv().startMotionTimerAndOdo(true); //reset et lancement du thread position, attente et moteur à 1 attente
	robot.setMyColor(PMXBLUE);
	while (robot.asserv().pos_getX_mm() < 10)
	{
		robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (robot.getMyColor() != PMXBLUE));
	}

	logger().info() << "setposition done:" << " x=" << robot.asserv().pos_getX_mm() << " y="
			<< robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << " color="
			<< robot.getMyColor() << logs::end;
	robot.svgPrintPosition();

//    robot.asserv().assistedHandling();
	robot.asserv().setMaxSpeed(true, s);
	//robot.asserv().setLowSpeedForward(true, 40);

	//detection adverse
	robot.actions().start();
	robot.actions().sensors().addTimerSensors(65);
	robot.chrono().start();

	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	bool frontcenter = robot.actions().sensors().getAvailableFrontCenter();
	logger().info() << "frontcenter=" << frontcenter << " " << logs::end;

	/*
	 robot.asserv().gotoChain(300, 800);
	 robot.asserv().gotoChain(400, 800);
	 robot.asserv().gotoChain(500, 800);
	 robot.asserv().gotoChain(600, 800);
	 TRAJ_STATE ts = robot.asserv().gotoXY(700, 800);


	 if (ts != TRAJ_FINISHED) {
	 robot.logger().error() << " whileMoveForwardTo  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
	 robot.asserv().resetEmergencyOnTraj();

	 }
	 */
	/*
	 logger().info() << "GOTO x=" << x << " y=" << y << logs::end;
	 ts = robot.asserv().doMoveForwardTo(x, y, true);

	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().info() << "===== TRAJ_INTERRUPTED 1 CONFIRMED" << logs::end;
	 //robot.asserv().stopMotors();


	 logger().info() << "1.Attente 3 sec!" << logs::end;
	 utils::sleep_for_secs(3);
	 robot.asserv().resetEmergencyOnTraj("===== TRAJ_INTERRUPTED 1 CONFIRMED");
	 }else
	 {
	 logger().info() << "===== TRAJ=" << ts << logs::end;
	 }

	 logger().info() << "GOTO x+200=" << x + 200 << " y=" << y << logs::end;
	 ts = robot.asserv().doMoveForwardTo(x + 200, y, true);

	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().info() << "===== TRAJ_INTERRUPTED 2 CONFIRMED" << logs::end;
	 //robot.asserv().stopMotors();


	 logger().info() << "2.Attente 3 sec!" << logs::end;
	 utils::sleep_for_secs(3);
	 robot.asserv().resetEmergencyOnTraj("===== TRAJ_INTERRUPTED 2 CONFIRMED");
	 }else
	 {
	 logger().info() << "===== TRAJ=" << ts << logs::end;
	 }

	 logger().info() << "GOTO x+250=" << x + 250 << " y=" << y << logs::end;
	 ts = robot.asserv().doMoveForwardTo(x + 250, y, true);

	 if (ts == TRAJ_INTERRUPTED)
	 {
	 logger().info() << "===== TRAJ_INTERRUPTED 3 CONFIRMED" << logs::end;
	 //robot.asserv().stopMotors();


	 logger().info() << "3.Attente 3 sec!" << logs::end;
	 utils::sleep_for_secs(3);
	 robot.asserv().resetEmergencyOnTraj("===== TRAJ_INTERRUPTED 3 CONFIRMED");
	 }else
	 {
	 logger().info() << "===== TRAJ=" << ts << logs::end;
	 }
	 */
	logger().info() << "GOTO x=" << x << " y=" << y << logs::end;
	ts = robot.ia().iAbyPath().whileMoveForwardTo(x, y, true, 1000000, 5, 5, false);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << " whileMoveForwardTo  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().resetEmergencyOnTraj();
	}
	logger().info() << "END GOTO1 ts=" << ts << logs::end;

	robot.actions().ax12_bras_droit(-1);
	robot.actions().ax12_bras_droit(-1);
	robot.actions().ax12_bras_droit_init(-1);
	robot.actions().ax12_bras_droit_init(-1);
	robot.actions().ax12_bras_gauche_init(-1);

	robot.actions().ax12_bras_gauche(-1);
	robot.actions().ax12_bras_gauche(-1);
	robot.actions().ax12_bras_gauche_init(-1);
	robot.actions().ax12_bras_gauche_init(-1);
	robot.actions().ax12_bras_droit_init(-1);

	if (x2 != 0 && y2 != 0)
	{
		//sleep(3);
		logger().info() << "GOTO2 x2=" << x2 << " y2=" << y2 << logs::end;

		TRAJ_STATE ts = robot.ia().iAbyPath().whileMoveForwardTo(x2, y2, true, 3000000, 3, 3, false);
		if (ts != TRAJ_FINISHED)
		{
			robot.logger().error() << " whileMoveForwardTo x2,y2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
					<< logs::end;
			robot.asserv().resetEmergencyOnTraj();
		}
		logger().info() << "END GOTO2 ts=" << ts << logs::end;
	}

	robot.actions().ax12_bras_droit(-1);
	robot.actions().ax12_bras_droit(-1);
	robot.actions().ax12_bras_droit_init(-1);
	robot.actions().ax12_bras_droit_init(-1);
	robot.actions().ax12_bras_gauche_init(-1);

	robot.actions().ax12_bras_gauche(-1);
	robot.actions().ax12_bras_gauche(-1);
	robot.actions().ax12_bras_gauche_init(-1);
	robot.actions().ax12_bras_gauche_init(-1);
	robot.actions().ax12_bras_droit_init(-1);

	if (x3 != 0 && y3 != 0)
	{
		//sleep(3);
		logger().info() << "GOTO2 x3=" << x3 << " y3=" << y3 << logs::end;

		TRAJ_STATE ts = robot.ia().iAbyPath().whileMoveForwardTo(x3, y3, true, 3000000, 3, 3, false);
		if (ts != TRAJ_FINISHED)
		{
			robot.logger().error() << " whileMoveForwardTo x3,y3  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
					<< logs::end;
			robot.asserv().resetEmergencyOnTraj();
		}
		logger().info() << "END GOTO3 ts=" << ts << logs::end;
	}

	robot.actions().ax12_bras_droit(-1);
	robot.actions().ax12_bras_droit(-1);
	robot.actions().ax12_bras_droit_init(-1);
	robot.actions().ax12_bras_droit_init(-1);
	robot.actions().ax12_bras_gauche_init(-1);

	robot.actions().ax12_bras_gauche(-1);
	robot.actions().ax12_bras_gauche(-1);
	robot.actions().ax12_bras_gauche_init(-1);
	robot.actions().ax12_bras_gauche_init(-1);
	robot.actions().ax12_bras_droit_init(-1);

	logger().info() << "time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms " << " x="
			<< robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
			<< robot.asserv().pos_getThetaInDegree() << logs::end;

	robot.svgPrintPosition();

	logger().info() << "Happy End." << logs::end;
}
