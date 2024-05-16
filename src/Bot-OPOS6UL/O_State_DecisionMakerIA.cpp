#include "O_State_DecisionMakerIA.hpp"

#include "../Common/Action/Sensors.hpp"
#include "../Common/Asserv/Asserv.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

O_State_DecisionMakerIA::O_State_DecisionMakerIA(Robot &robot) :
		robot_(robot)
{
}

bool O_take_top()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_take_top." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	ROBOTPOSITION zone;
	robot.logger().info() << "start O_take_top zone_flower_top x=" << zone.x << " y=" << zone.y << logs::end;
	robot.ia().iAbyPath().goToZone("zone_flower_top", &zone);

	ts = TRAJ_OK;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), true, 100000, 5, 2,
			true, 40);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_take_top : zone_flower_top  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

	}
	robot.svgPrintPosition();

	return true; //return true si ok sinon false si interruption
}

bool O_end_of()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_end_of_match_top2." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	ROBOTPOSITION zone;

	robot.asserv().setMaxSpeed(true, 800);

	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	ts = robot.ia().iAbyPath().whileMoveForwardTo(1700, 600, true, 1000000, 10, 15, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_end_of_match_top : 1200, 1000  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

		return false;
	}
	robot.svgPrintPosition();

	ts = robot.ia().iAbyPath().whileMoveForwardTo(2600, 800, true, 1000000, 10, 15, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_end_of_match_top : 2600, 800  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

		return false;
	}
	robot.svgPrintPosition();

	ts = robot.ia().iAbyPath().whileMoveForwardTo(2600, 800, true, 1000000, 10, 15, true);
		if (ts != TRAJ_FINISHED)
		{
			robot.logger().error() << "O_end_of_match_top : 2600, 800  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
					<< logs::end;
			robot.asserv().stopMotors();
			robot.asserv().resetEmergencyOnTraj();

			return false;
		}
		robot.svgPrintPosition();

	robot.points += 10;
	robot.displayPoints();
	return true; //return true si ok sinon false si interruption
}
bool O_end_of_match_top()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_end_of_match_top." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	ROBOTPOSITION zone;

	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	ts = robot.ia().iAbyPath().whileMoveForwardTo(1200, 1000, true, 1000000, 10, 4, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_end_of_match_top : zone_end_top  ===== PB COLLISION FINALE - Que fait-on? ts="
				<< ts << logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

		return false;
	}
	robot.svgPrintPosition();

	robot.logger().info() << "start O_end_of_match_top zone_end_top x=" << zone.x << " y=" << zone.y << logs::end;

	robot.ia().iAbyPath().goToZone("zone_end_top", &zone);

	robot.displayPoints();
//	while (ts != TRAJ_FINISHED)
//	{
	robot.logger().info() << "start O_end_of_match_top zone_end_top x=" << zone.x << " y=" << zone.y << logs::end;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), true, 1000000, 30, 10,
			true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_end_of_match_top : zone_end_top  ===== PB COLLISION FINALE - Que fait-on? ts="
				<< ts << logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();
		return false;

	}
	robot.svgPrintPosition();
//	}
	robot.points += 10;
	robot.displayPoints();
	robot.asserv().doLineAbs(-100);

	/*
	 robot.actions().ax12_bras_droit(0);
	 robot.actions().ax12_bras_gauche(-1);
	 robot.actions().ax12_bras_droit_init(0);
	 robot.actions().ax12_bras_gauche_init(-1);

	 robot.actions().ax12_bras_droit(0);
	 robot.actions().ax12_bras_gauche(-1);
	 robot.actions().ax12_bras_droit_init(0);
	 robot.actions().ax12_bras_gauche_init(-1);

	 robot.actions().ax12_bras_droit(0);
	 robot.actions().ax12_bras_gauche(-1);
	 robot.actions().ax12_bras_droit_init(0);
	 robot.actions().ax12_bras_gauche_init(-1);

	 robot.actions().ax12_bras_droit(0);
	 robot.actions().ax12_bras_gauche(-1);
	 robot.actions().ax12_bras_droit_init(0);
	 robot.actions().ax12_bras_gauche_init(-1);

	 robot.actions().ax12_bras_droit(0);
	 robot.actions().ax12_bras_gauche(-1);
	 robot.actions().ax12_bras_droit_init(0);
	 robot.actions().ax12_bras_gauche_init(-1);
	 */
	return true; //return true si ok sinon false si interruption
}

bool O_end_of_match_bottom()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_end_of_match_bottom." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	ROBOTPOSITION zone;
	robot.logger().info() << "start O_end_of_match_bottom zone_end_bottom x=" << zone.x << " y=" << zone.y << logs::end;
	robot.ia().iAbyPath().goToZone("zone_end_bottom", &zone);

	robot.logger().info() << "start O_end_of_match_bottom zone_end_bottom x=" << zone.x << " y=" << zone.y << logs::end;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), true, 100000, 5, 2,
			true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error()
				<< "O_end_of_match_bottom : zone_end_bottom  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

	}
	robot.svgPrintPosition();
	robot.points += 10;
	return true; //return true si ok sinon false si interruption
}

bool O_solar_panels()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_solar_panels." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;

	robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_gauche_init(0);
	} else
	{
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_droit_init(0);

	}
	robot.svgPrintPosition();

	robot.points += 5;
	robot.displayPoints();

	//robot.asserv().setLowSpeedForward(true, 25);
	robot.asserv().setMaxSpeed(true, 400);

	//todo ignorer calage et blocage pour continuer
	robot.logger().error() << "O_solar_panels : 470, 185  " << logs::end;
	ts = TRAJ_OK;

	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(470, 185, 0.0, true, 100000, 5, 20, false, 0, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_solar_panels : 470, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

	}
	robot.svgPrintPosition(4);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_gauche_init(0);
	} else
	{
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_droit_init(0);

	}

	robot.points += 5;
	robot.displayPoints();

	ts = TRAJ_OK;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(695, 185, 0.0, true, 100000, 5, 20, false, 0, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_solar_panels : 695, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

	}
	robot.svgPrintPosition(4);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_gauche_init(0);
	} else
	{
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_droit_init(0);

	}

	robot.points += 5;
	robot.displayPoints();

	robot.svgPrintPosition();

	//____________________________4eme

	ts = TRAJ_OK;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1250, 185, 0.0, true, 100000, 5, 20, false, 0, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_solar_panels : 1250, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();
		return true;
	}
	robot.svgPrintPosition(4);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_gauche_init(0);
	} else
	{
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_droit_init(0);

	}

	robot.points += 5;
	robot.displayPoints();

	robot.svgPrintPosition();

	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
	//____________________________5eme

	robot.asserv().setMaxSpeed(true, 200);

	ts = TRAJ_OK;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1475, 185, 0.0, true, 100000, 5, 20, false, 0, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_solar_panels : 1475, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();
		return true;

	}
	robot.svgPrintPosition(4);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_gauche_init(0);
	} else
	{
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_droit_init(0);

	}

	robot.points += 5;
	robot.displayPoints();

	robot.svgPrintPosition();

	//____________________________6eme

	ts = TRAJ_OK;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1700, 185, 0.0, true, 100000, 5, 20, 0, true);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << "O_solar_panels : 1700, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();

		return true;

	}
	robot.svgPrintPosition(4);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_droit_init(-1);
		robot.actions().ax12_bras_gauche_init(0);
	} else
	{
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_gauche_init(-1);
		robot.actions().ax12_bras_droit_init(0);

	}

	robot.points += 5;
	robot.displayPoints();

	robot.svgPrintPosition();

	ts = robot.asserv().doFaceTo(1500, 1000);
	robot.logger().info() << "O_solar_panels : doFaceTo(1500, 1000); ts = " << ts << logs::end;

	//robot.asserv().setMaxSpeed(false);
	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

	//_______________________________

	return true; //return true si ok sinon false si interruption
}

void O_State_DecisionMakerIA::IASetupActivitiesZone()
{
	logger().info() << "IASetupActivitiesZone homologation" << logs::end;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	logger().debug() << "color = " << robot.getMyColor() << logs::end;

	robot.ia().iAbyPath().ia_createZone("zone_end_top", 0, 1550, 450, 450, 400, 1700, 90);
	robot.ia().iAbyPath().ia_createZone("zone_end_bottom", 0, 0, 450, 450, 400, 350, 45);
	robot.ia().iAbyPath().ia_createZone("zone_solar", 800, 0, 450, 100, 800, 230, 0);

	robot.ia().iAbyPath().ia_createZone("zone_flower_top", 900, 1300, 200, 100, 1000, 1600, -90);
	robot.ia().iAbyPath().ia_createZone("zone_flower_bottom", 900, 600, 200, 100, 1000, 300, 90);

	robot.ia().iAbyPath().ia_addAction("solar_panels", &O_solar_panels);
	//robot.ia().iAbyPath().ia_addAction("end_of_match_top", &O_end_of_match_top);
	robot.ia().iAbyPath().ia_addAction("end_of", &O_end_of);

	logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
	logger().error() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	logger().debug() << "color = " << robot.getMyColor() << logs::end;

	robot.ia().iAbyPath().ia_createZone("zone_end_top", 0, 1550, 450, 450, 400, 1700, 90);
	robot.ia().iAbyPath().ia_createZone("zone_end_bottom", 0, 0, 450, 450, 400, 350, 45);
	robot.ia().iAbyPath().ia_createZone("zone_solar", 800, 0, 450, 100, 800, 230, 0);

	robot.ia().iAbyPath().ia_createZone("zone_flower_top", 900, 1300, 200, 100, 1000, 1000, -90);
	robot.ia().iAbyPath().ia_createZone("zone_flower_bottom", 900, 600, 200, 100, 1000, 300, 90);

	//robot.ia().iAbyPath().ia_addAction("solar_panels", &O_solar_panels);
	robot.ia().iAbyPath().ia_addAction("end_of_match_top", &O_end_of_match_top);

	/*
	 robot.ia().iAbyPath().ia_createZone("zone_end_top", 0, 1550, 450, 450, 400, 1750, -45);
	 robot.ia().iAbyPath().ia_createZone("zone_end_bottom", 0, 0, 450, 450, 400, 350, 45);

	 robot.ia().iAbyPath().ia_createZone("zone_flower_top", 900, 1300, 200, 100, 1000, 1600, -90);

	 robot.ia().iAbyPath().ia_createZone("zone_flower_bottom", 900, 600, 200, 100, 1000, 300, 90);

	 robot.ia().iAbyPath().ia_addAction("take_top", &O_take_top);

	 robot.ia().iAbyPath().ia_addAction("end_of_match_bottom", &O_end_of_match_bottom);
	 */
	logger().debug() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void O_State_DecisionMakerIA::execute()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

//wait for init!
	while (!robot.waitForInit())
	{
		usleep(1000);
		//logger().error() << "waitForInit..." << logs::end;
	}

	logger().info() << "Strategy to be applied = " << robot.strategy() << logs::end;

	if (robot.strategy() == "tabletest")
	{
		IASetupActivitiesZoneTableTest();

		//enable zone if necessary
		//robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
		//robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

	} else if (robot.strategy() == "all")
	{
		IASetupActivitiesZone(); //definit les activities

		//enable zone if necessary
		//robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
		//robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

	} else
	{
		logger().error() << "NO STRATEGY " << robot.strategy() << " FOUND !!! " << logs::end;
	}

//wait for the start of the chrono !
	while (!robot.chrono().started())
	{
		usleep(10000);
	}

	logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;
	/*
	 //init rouge
	 TRAJ_STATE ts = robot.asserv().doMoveForwardTo(330, 463);
	 if (ts != TRAJ_FINISHED) {
	 robot.logger().error() << " doMoveForwardTo(330, 463);  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
	 << logs::end;
	 robot.asserv().resetEmergencyOnTraj();
	 }
	 ts = robot.asserv().doAbsoluteRotateTo(150);

	 robot.points += 6;
	 robot.displayPoints();
	 ts = robot.asserv().doLineAbs(-100);
	 if (ts != TRAJ_FINISHED) {
	 robot.logger().error() << " doLineAbs(-100)  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
	 robot.asserv().resetEmergencyOnTraj();
	 }
	 robot.points += 35;
	 robot.displayPoints();

	 if (robot.getMyColor() == PMXBLUE) {
	 robot.ia().iAbyPath().enable(robot.ia().area_alea_yellow, 0);
	 } else {
	 robot.ia().iAbyPath().enable(robot.ia().area_alea_violet, 0);
	 }*/

	//On ajoute le timer de detection
	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
	robot.actions().sensors().addTimerSensors(65);

	robot.ia().iAbyPath().ia_start();        //launch IA

	//robot.actions().ledBar().startK2mil(100, 100000, LED_GREEN);

	robot.freeMotion();

	robot.svgPrintEndOfFile();
	logger().info() << "O_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}

/*
 bool O_launch_balls_1()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_launch_balls_1." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;

 robot.ia().iAbyPath().enable(robot.ia().area_B3, 0);
 robot.ia().iAbyPath().enable(robot.ia().area_A3, 0);

 robot.ia().iAbyPath().goToZone("zone_launch1", &zone);
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), true, 2000000, 1, 1,
 true, 40);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << "O_launch_balls_1 : zone_launch1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }
 robot.svgPrintPosition();

 robot.asserv().doFaceTo(1550, 1000);

 robot.actions().lancer_les_balles(88); //90 pour 76cm //127 pour 160cm
 //usleep(1000000);
 utils::sleep_for_secs(1);

 robot.actions().aspiration_lacher_les_balles();
 //usleep(3000000);
 utils::sleep_for_secs(3);
 robot.actions().stopper_lanceur_de_balles();
 robot.actions().aspiration_closed_init(-1);

 utils::sleep_for_secs(5);
 robot.actions().stopper_lanceur_de_balles();

 return true; //return true si ok sinon false si interruption
 }

 bool O_take_ball_BC1()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_take_ball_BC1." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;

 robot.ia().iAbyPath().goToZone("zone_ball_BC1", &zone);
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), true, 2000000, 3, 3,
 true, 40);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << "O_take_ball_BC1 : zone_ball_BC1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }
 robot.svgPrintPosition();

 return true; //return true si ok sinon false si interruption
 }

 bool O_take_ball_D3()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_take_ball_D3." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;

 robot.ia().iAbyPath().goToZone("zone_ball_D3", &zone);
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), true, 2000000, 3, 3,
 true, 40);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << "O_take_ball_D3 : zone_ball_D3  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }
 robot.svgPrintPosition();

 return true; //return true si ok sinon false si interruption
 }
 */
