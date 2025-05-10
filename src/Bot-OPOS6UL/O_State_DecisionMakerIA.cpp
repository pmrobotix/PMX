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

bool O_end_of_match_top()
{

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << __FUNCTION__ << logs::end;
	TRAJ_STATE ts = TRAJ_IDLE;
	ROBOTPOSITION zone;

	robot.lastAction(true);

	robot.asserv().setMaxSpeed(true, 40);

	robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
//
//	ts = robot.ia().iAbyPath().whileMoveForwardTo(1200, 1000, NO_ROTATION_DETECTION, 1000000, 10, 4, WITH_PATHFINDING);
//	if (ts != TRAJ_FINISHED)
//	{
//		robot.logger().error() << __FUNCTION__ << " zone_end_top  ===== PB COLLISION FINALE - Que fait-on? ts="
//				<< ts << logs::end;
//		robot.asserv().stopMotors();
//		robot.asserv().resetEmergencyOnTraj();
//
//		return false;
//	}

	robot.logger().info() << __FUNCTION__ << " start zone_end_top x=" << zone.x << " y=" << zone.y << logs::end;

	robot.ia().iAbyPath().goToZone("zone_end_top", &zone);

	robot.displayPoints();

	robot.logger().info() << __FUNCTION__ << " start zone_end_top x=" << zone.x << " y=" << zone.y << logs::end;
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), ROTATION_WITH_DETECTION,
			1000000, 30, 10, NO_PATHFINDING);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << __FUNCTION__ << " zone_end_top  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		//robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();
		robot.svgPrintPosition();

		return false; // TODO WAIT ou se deplacer pour y arriver

	}
	robot.svgPrintPosition();

	//attente de 95sec
	while (robot.chrono().getElapsedTimeInSec() <= 96)
	{
		utils::sleep_for_secs(1);
	}

	ts = robot.asserv().doLine(200);
	robot.svgPrintPosition();

	robot.points += 20;
	robot.displayPoints();

	//robot.logger().info() << __FUNCTION__ << " turn doFaceTo(400, 1300)" << logs::end;

	//ts = robot.asserv().doFaceTo(350, 1000);
	//robot.svgPrintPosition();

//	robot.actions().ax12_bras_droit_full();
//	robot.actions().ax12_bras_gauche_full();
//
//	robot.actions().ax12_up();

	robot.svgPrintPosition();

//	 robot.actions().ax12_bras_droit(0);
//	 robot.actions().ax12_bras_gauche(-1);
//	 robot.actions().ax12_bras_droit_init(0);
//	 robot.actions().ax12_bras_gauche_init(-1);
//
//	 robot.actions().ax12_bras_droit(0);
//	 robot.actions().ax12_bras_gauche(-1);
//	 robot.actions().ax12_bras_droit_init(0);
//	 robot.actions().ax12_bras_gauche_init(-1);
//
//	 robot.actions().ax12_bras_droit(0);
//	 robot.actions().ax12_bras_gauche(-1);
//	 robot.actions().ax12_bras_droit_init(0);
//	 robot.actions().ax12_bras_gauche_init(-1);
//
//	 robot.actions().ax12_bras_droit(0);
//	 robot.actions().ax12_bras_gauche(-1);
//	 robot.actions().ax12_bras_droit_init(0);
//	 robot.actions().ax12_bras_gauche_init(-1);
//
//	 robot.actions().ax12_bras_droit(0);
//	 robot.actions().ax12_bras_gauche(-1);
//	 robot.actions().ax12_bras_droit_init(0);
//	 robot.actions().ax12_bras_gauche_init(-1);
//

	return true; //return true si ok sinon false si interruption
}

bool O_push_prise_bas()
{

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << __FUNCTION__ << logs::end;
	TRAJ_STATE ts = TRAJ_IDLE;
	ROBOTPOSITION zone;

	robot.asserv().setMaxSpeed(true, 40);
	robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
	robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
	robot.logger().info() << __FUNCTION__ << " start push_prise_bas x=" << zone.x << " y=" << zone.y << logs::end;
	robot.ia().iAbyPath().goToZone("zone_prise_bas", &zone);
	ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), ROTATION_WITH_DETECTION,
			1000000, 30, 30, NO_PATHFINDING);
	if (ts != TRAJ_FINISHED)
	{



		robot.logger().error() << __FUNCTION__ << " zone_prise_bas  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;




		//robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();
		robot.svgPrintPosition();
		return false;

	}
	robot.svgPrintPosition();



	ts = robot.ia().iAbyPath().whileMoveForwardTo(775, 200, ROTATION_WITH_DETECTION, 1000000, 10, 10, NO_PATHFINDING);
	if (ts != TRAJ_FINISHED)
	{
		robot.logger().error() << __FUNCTION__ << " 775, 200  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
				<< logs::end;
		//robot.asserv().stopMotors();
		robot.asserv().resetEmergencyOnTraj();
		robot.svgPrintPosition();
		return true;
	}
	robot.svgPrintPosition();

	//robot.asserv().doLineAbs(-50);
//
//	//on recule
//	ts = robot.ia().iAbyPath().whileMoveBackwardTo(775, 350, NO_ROTATION_DETECTION, 1000000, 5, 5, NO_PATHFINDING);
//	if (ts != TRAJ_FINISHED)
//	{
//
//
//		robot.logger().error() << __FUNCTION__ << " 775, 350  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
//				<< logs::end;
//		//robot.asserv().stopMotors();
//		robot.asserv().resetEmergencyOnTraj();
//		robot.svgPrintPosition();
//		return true;
//	}
	robot.svgPrintPosition();

	return true; //return true si ok sinon false si interruption
}

void O_State_DecisionMakerIA::IASetupActivitiesZone()
{
	logger().info() << "IASetupActivitiesZone homologation" << logs::end;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	logger().debug() << "color = " << robot.getMyColor() << logs::end;

	robot.ia().iAbyPath().ia_createZone("zone_end_top", 150, 1550, 450, 450, 450, 1300, 90);
	robot.ia().iAbyPath().ia_createZone("zone_start", 1000, 0, 450, 450, 1300, 400, 90);
	robot.ia().iAbyPath().ia_createZone("zone_prise_bas", 550, 0, 450, 100, 775, 550, -90);

	//robot.ia().iAbyPath().ia_addAction("push_prise_bas", &O_push_prise_bas);

	/*
	 robot.ia().iAbyPath().ia_createZone("zone_end_bottom", 0, 0, 450, 450, 400, 350, 45);
	 robot.ia().iAbyPath().ia_createZone("zone_solar", 800, 0, 450, 100, 800, 230, 0);

	 robot.ia().iAbyPath().ia_createZone("zone_steal_adv", 1900, 600, 200, 100, 1200, 400, 45);

	 robot.ia().iAbyPath().ia_createZone("zone_flower_top", 900, 1300, 200, 100, 1000, 1000, 90);
	 //robot.ia().iAbyPath().ia_createZone("zone_flower_bottom", 900, 600, 200, 100, 1000, 400, 90);
	 robot.ia().iAbyPath().ia_createZone("zone_flower_bottom", 900, 600, 200, 100, 1150, 400, 120);

	 robot.ia().iAbyPath().ia_addAction("solar_panels", &O_solar_panels);
	 robot.ia().iAbyPath().ia_addAction("solar_panels_adv", &O_solar_panels_adv);
	 //robot.ia().iAbyPath().ia_addAction("steal_adv", &O_steal_adv);
	 robot.ia().iAbyPath().ia_addAction("take_flower_bottom", &O_take_flowers);
	 robot.ia().iAbyPath().ia_addAction("put_flowers_distributor", &O_put_flowers_distributor);
	 */
	robot.ia().iAbyPath().ia_addAction("end_of_match_top", &O_end_of_match_top);

	logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
	logger().error() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	logger().debug() << "color = " << robot.getMyColor() << logs::end;

	robot.ia().iAbyPath().ia_createZone("zone_end_top", 150, 1550 - 420, 450, 450, 450, 1300 - 420, 90);
	robot.ia().iAbyPath().ia_createZone("zone_start", 1000, 0, 450, 450, 1300, 400, 90);
	robot.ia().iAbyPath().ia_createZone("zone_prise_bas", 550, 0, 450, 100, 775, 550, -90);

	//robot.ia().iAbyPath().ia_addAction("push_prise_bas", &O_push_prise_bas);

//		robot.ia().iAbyPath().ia_addAction("solar_panels", &O_solar_panels);
//		robot.ia().iAbyPath().ia_addAction("solar_panels_adv", &O_solar_panels_adv);
	//robot.ia().iAbyPath().ia_addAction("steal_adv", &O_steal_adv);
	//robot.ia().iAbyPath().ia_addAction("take_flower_bottom", &O_take_flowers);
	//robot.ia().iAbyPath().ia_addAction("put_flowers_distributor", &O_put_flowers_distributor);

	robot.ia().iAbyPath().ia_addAction("end_of_match_top", &O_end_of_match_top);
	logger().debug() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void O_State_DecisionMakerIA::execute()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

//wait for init!
	while (!robot.waitForInit())
	{
		//usleep(1000);
		utils::sleep_for_micros(1000);
		//logger().error() << "waitForInit..." << logs::end;
	}

	logger().info() << __FUNCTION__ << " Strategy to be applied = " << robot.strategy() << logs::end;

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
		//usleep(10000);
		utils::sleep_for_micros(10000);
	}

	logger().info() << __FUNCTION__ << " executing..." << logs::end;
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
	logger().info() << __FUNCTION__ << " >>>>>>   svgPrintEndOfFile DONE.........." << logs::end;

}
/*
 *
 bool O_take_top()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;
 robot.logger().info() << __FUNCTION__ << "start zone_flower_top x=" << zone.x << " y=" << zone.y << logs::end;
 robot.ia().iAbyPath().goToZone("zone_flower_top", &zone);

 ts = TRAJ_OK;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), NO_ROTATION_DETECTION,
 100000, 5, 2, NO_PATHFINDING, 40);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << " zone_flower_top  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();

 }
 robot.svgPrintPosition();

 return true; //return true si ok sinon false si interruption
 }

 bool O_steal_adv()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;

 if (robot.isLastAction() == true)
 return true;

 robot.asserv().setMaxSpeed(true, 20);

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 robot.ia().iAbyPath().goToZone("zone_steal_adv", &zone);
 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, NO_ROTATION_DETECTION, 1000000, 5, 5, NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << " 1200, 400  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition();
 return false;
 }
 robot.svgPrintPosition();


 robot.actions().ax12_init();

 ts = robot.ia().iAbyPath().whileMoveForwardTo(2700, 1000, NO_ROTATION_DETECTION, 1000000, 5, 5, NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << " 2700, 1000  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition();
 return true;
 }
 robot.svgPrintPosition();

 robot.points += 2;
 robot.displayPoints();

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);


 //robot.asserv().setMaxSpeed(false);

 ts = robot.asserv().doLineAbs(-120);
 robot.svgPrintPosition();

 robot.actions().ax12_retract_in_block(2000);

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 return true; //return true si ok sinon false si interruption
 }
 bool O_end_of_match_top()
 {

 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;

 robot.lastAction(true);

 robot.asserv().setMaxSpeed(true, 40);

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //
 //	ts = robot.ia().iAbyPath().whileMoveForwardTo(1200, 1000, NO_ROTATION_DETECTION, 1000000, 10, 4, WITH_PATHFINDING);
 //	if (ts != TRAJ_FINISHED)
 //	{
 //		robot.logger().error() << __FUNCTION__ << " zone_end_top  ===== PB COLLISION FINALE - Que fait-on? ts="
 //				<< ts << logs::end;
 //		robot.asserv().stopMotors();
 //		robot.asserv().resetEmergencyOnTraj();
 //
 //		return false;
 //	}


 ts = robot.asserv().doLineAbs(100);
 ts = robot.asserv().doLineAbs(-100);
 robot.svgPrintPosition();

 robot.logger().info() << __FUNCTION__ << " start zone_end_top x=" << zone.x << " y=" << zone.y << logs::end;

 robot.ia().iAbyPath().goToZone("zone_end_top", &zone);

 robot.displayPoints();

 robot.logger().info() << __FUNCTION__ << " start zone_end_top x=" << zone.x << " y=" << zone.y << logs::end;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), NO_ROTATION_DETECTION,
 1000000, 30, 10, NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << " zone_end_top  ===== PB COLLISION FINALE - Que fait-on? ts="
 << ts << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition();
 return false;

 }
 robot.svgPrintPosition();

 robot.points += 10;
 robot.displayPoints();

 ts = robot.asserv().doLineAbs(-280);
 robot.svgPrintPosition();

 robot.logger().info() << __FUNCTION__ << " turn doFaceTo(400, 1300)"  << logs::end;

 ts= robot.asserv().doFaceTo(700, 1400);
 robot.svgPrintPosition();

 robot.actions().ax12_bras_droit_full();
 robot.actions().ax12_bras_gauche_full();

 robot.actions().ax12_up();

 robot.svgPrintPosition();


 //	 robot.actions().ax12_bras_droit(0);
 //	 robot.actions().ax12_bras_gauche(-1);
 //	 robot.actions().ax12_bras_droit_init(0);
 //	 robot.actions().ax12_bras_gauche_init(-1);
 //
 //	 robot.actions().ax12_bras_droit(0);
 //	 robot.actions().ax12_bras_gauche(-1);
 //	 robot.actions().ax12_bras_droit_init(0);
 //	 robot.actions().ax12_bras_gauche_init(-1);
 //
 //	 robot.actions().ax12_bras_droit(0);
 //	 robot.actions().ax12_bras_gauche(-1);
 //	 robot.actions().ax12_bras_droit_init(0);
 //	 robot.actions().ax12_bras_gauche_init(-1);
 //
 //	 robot.actions().ax12_bras_droit(0);
 //	 robot.actions().ax12_bras_gauche(-1);
 //	 robot.actions().ax12_bras_droit_init(0);
 //	 robot.actions().ax12_bras_gauche_init(-1);
 //
 //	 robot.actions().ax12_bras_droit(0);
 //	 robot.actions().ax12_bras_gauche(-1);
 //	 robot.actions().ax12_bras_droit_init(0);
 //	 robot.actions().ax12_bras_gauche_init(-1);
 //

 return true; //return true si ok sinon false si interruption
 }

 bool O_end_of_match_bottom()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;
 robot.logger().info() << __FUNCTION__ << " start zone_end_bottom x=" << zone.x << " y=" << zone.y << logs::end;
 robot.ia().iAbyPath().goToZone("zone_end_bottom", &zone);

 robot.logger().info() << __FUNCTION__ << "start zone_end_bottom x=" << zone.x << " y=" << zone.y << logs::end;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), NO_ROTATION_DETECTION,
 100000, 5, 2, NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error()
 << __FUNCTION__ << " zone_end_bottom  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();

 }
 robot.svgPrintPosition();
 robot.points += 10;
 return true; //return true si ok sinon false si interruption
 }

 bool O_solar_panels()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 if (robot.getMyColor() == PMXYELLOW)
 {
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 } else
 {
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }
 robot.svgPrintPosition();

 robot.points += 5;
 robot.displayPoints();

 //robot.asserv().setLowSpeedForward(true, 25);
 //robot.asserv().setMaxSpeed(true, 25);

 //todo ignorer calage et blocage pour continuer
 robot.logger().error() << __FUNCTION__<< " 470, 185  " << logs::end;
 ts = TRAJ_OK;

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(470, 185, 0.0, NO_ROTATION_DETECTION, 1000000, 5, 5, NO_PATHFINDING, 0,
 true);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__<< " 470, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();

 }
 robot.svgPrintPosition(4);

 if (robot.getMyColor() == PMXYELLOW)
 {
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 } else
 {
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }

 robot.points += 5;
 robot.displayPoints();

 ts = TRAJ_OK;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(695, 185, 0.0, NO_ROTATION_DETECTION, 1000000, 5, 5, NO_PATHFINDING, 0,
 true);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__<< " 695, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();

 }
 robot.svgPrintPosition(4);

 if (robot.getMyColor() == PMXYELLOW)
 {
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 } else
 {
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }

 robot.points += 5;
 robot.displayPoints();


 //PAUSE
 utils::sleep_for_secs(8);

 robot.asserv().setMaxSpeed(false);

 robot.svgPrintPosition();

 return true; //return true si ok sinon false si interruption
 }

 bool O_solar_panels_adv()
 {

 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 robot.asserv().setMaxSpeed(true, 90);

 //____________________________4eme

 ts = TRAJ_OK;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1250, 185, 0.0, NO_ROTATION_DETECTION, 2000000, 5, 5, NO_PATHFINDING,
 0, false);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << " 1250, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition();
 return true;
 }
 robot.svgPrintPosition(4);

 if (robot.getMyColor() == PMXYELLOW)
 {
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 } else
 {
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }

 robot.points += 5;
 robot.displayPoints();

 robot.svgPrintPosition();

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //____________________________5eme

 robot.asserv().setMaxSpeed(true, 20);

 ts = TRAJ_OK;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1460, 185, 0.0, NO_ROTATION_DETECTION, 2000000, 5, 5,
 NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << " 1460, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition();
 return true;

 }
 robot.svgPrintPosition(4);

 if (robot.getMyColor() == PMXYELLOW)
 {
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 } else
 {
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }

 robot.points += 5;
 robot.displayPoints();

 robot.svgPrintPosition();

 //____________________________6eme

 ts = TRAJ_OK;
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1690, 185, 0.0, NO_ROTATION_DETECTION, 2000000, 5, 5, 0,
 NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__<< " 1690, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();

 return true;

 }
 robot.svgPrintPosition(4);

 if (robot.getMyColor() == PMXYELLOW)
 {
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 } else
 {
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }

 robot.points += 5;
 robot.displayPoints();

 robot.svgPrintPosition();

 robot.actions().ax12_retract_in_block(2000);


 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 //On recule
 ts = TRAJ_OK;
 ts = robot.ia().iAbyPath().whileMoveBackwardTo(1200, 185, NO_ROTATION_DETECTION, 200000, 5, 5, 0,
 NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error() << __FUNCTION__ << "whileMoveBackwardTo 1700, 185  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition();
 return true;

 }
 robot.svgPrintPosition(4);

 //ts = robot.asserv().doFaceTo(1500, 1000);
 //robot.logger().info() << __FUNCTION__ << " doFaceTo(1500, 1000); ts = " << ts << logs::end;
 //robot.svgPrintPosition();

 //robot.asserv().setMaxSpeed(false);
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 //_______________________________
 robot.asserv().setMaxSpeed(false);

 return true; //return true si ok sinon false si interruption
 }

 bool O_take_flowers()
 {

 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << __FUNCTION__ << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 ROBOTPOSITION zone;
 robot.logger().info() << __FUNCTION__ << " start zone_flower_bottom x=" << zone.x << " y=" << zone.y
 << logs::end;

 if (robot.isLastAction() == true)
 return true;

 if (robot.asserv().pos_getY_mm() < 700)
 robot.ia().iAbyPath().goToZone("zone_flower_bottom", &zone);
 else
 robot.ia().iAbyPath().goToZone("zone_flower_top", &zone);


 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, radToDeg(zone.theta), NO_ROTATION_DETECTION,
 1000000, 5, 2, NO_PATHFINDING);
 if (ts != TRAJ_FINISHED)
 {
 robot.logger().error()
 << __FUNCTION__ << " zone_flower_bottom  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 //robot.asserv().stopMotors();
 robot.asserv().resetEmergencyOnTraj();

 }
 robot.svgPrintPosition();



 robot.asserv().setMaxSpeed(true, 20);



 //	robot.asserv().doLineAbs(100);
 //	robot.svgPrintPosition();
 //
 //	robot.asserv().doLineAbs(-100);
 //	robot.svgPrintPosition();
 //
 //	robot.actions().ax12_up();

 robot.isEmpty(false);

 //robot.asserv().setMaxSpeed(false);
 //robot.asserv().setMaxSpeed(true, 50);
 robot.actions().ax12_init_LR();

 return true; //return true si ok sinon false si interruption
 }
 */

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
