#include "O_State_DecisionMaker.hpp"

#include <unistd.h>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyZone.hpp"
#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

bool O_action1()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start action1." << logs::end;

	/*
	 TRAJ_STATE ts = TRAJ_OK;
	 RobotPosition path, zone;

	 robot.ia().iAbyZone().goToZone("zone1", &path, &zone);
	 if (&path != NULL)
	 {
	 ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();
	 }
	 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	 if (ts != TRAJ_OK)
	 return false;
	 */
	robot.svgPrintPosition();

	robot.logger().info() << "action1 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool O_action2()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start action2." << logs::end;
	/*
	 TRAJ_STATE ts = TRAJ_OK;
	 RobotPosition path, zone;

	 robot.ia().iAbyZone().goToZone("zone2", &path, &zone);
	 if (&path != NULL)
	 {
	 ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();
	 }
	 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	 if (ts != TRAJ_OK)
	 return false;
	 */
	robot.svgPrintPosition();

	robot.logger().info() << "action2 done." << logs::end;
	return true;
}

bool O_action3()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start action3." << logs::end;
	/*
	 TRAJ_STATE ts = TRAJ_OK;
	 RobotPosition path, zone;

	 robot.ia().iAbyZone().goToZone("zone3", &path, &zone);
	 if (&path != NULL)
	 {
	 ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();
	 }
	 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	 if (ts != TRAJ_OK)
	 return false;
	 */
	robot.svgPrintPosition();

	robot.logger().info() << "action3 done." << logs::end;
	return true;
}

void O_State_DecisionMaker::IASetupDemo()
{
	logger().debug() << "IASetupDemo" << logs::end;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	/*
	 robot.ia().iAbyZone().ia_createZone("depart", 0, 900, 300, 300, 400, 1050, 180);
	 robot.ia().iAbyZone().ia_createZone("zone1", 500, 1000, 300, 200, 500, 1100, 0);
	 robot.ia().iAbyZone().ia_createZone("zone2", 500, 1800, 200, 200, 600, 1800, 90);
	 robot.ia().iAbyZone().ia_createZone("zone3", 500, 0, 300, 200, 400, 250, 0);

	 robot.ia().iAbyZone().ia_setPath("depart", "zone1", 500, 1100);
	 robot.ia().iAbyZone().ia_setPath("depart", "zone2", 600, 1500);
	 robot.ia().iAbyZone().ia_setPath("depart", "zone3", 450, 500);
	 robot.ia().iAbyZone().ia_setPath("zone1", "zone2", 600, 1500);
	 robot.ia().iAbyZone().ia_setPath("zone1", "zone3", 450, 500);
	 robot.ia().iAbyZone().ia_setPath("zone2", "zone3", 450, 500);

	 robot.ia().iAbyZone().ia_addAction("action1", &O_action1);
	 robot.ia().iAbyZone().ia_addAction("action2", &O_action2);
	 robot.ia().iAbyZone().ia_addAction("action3", &O_action3);
	 */
}

bool O_take_tower1()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_take_tower1." << logs::end;

	robot.asserv().assistedHandling();
	robot.asserv().ignoreFrontCollision(true);
	robot.asserv().ignoreRearCollision(true);

	robot.actions().arm_deploy(1000);
	robot.actions().nose_back(1000);

	robot.logger().info() << "GO" << logs::end;

	robot.asserv().doLineAbs(250);
	robot.svgPrintPosition();

	robot.logger().info() << "on prend le plot 1" << logs::end;
	robot.actions().nose_down(150);
	usleep(500000);
	robot.actions().arm_retract(500);
	robot.actions().arm_back_cylinder(500);
	usleep(500000);

	robot.logger().info() << "on va se positionner " << logs::end;

//MATCH
	robot.asserv().ignoreFrontCollision(false);
//TABLE DE TEST
	//robot.asserv().ignoreFrontCollision(true);

//MATCH
	while (robot.asserv().doMoveForwardAndRotateTo(1500, 900, 90) != TRAJ_OK)
//TABLE DE TEST
	//while (robot.asserv().doMoveForwardAndRotateTo(1200, 900, 90) != TRAJ_OK)
	{
		robot.logger().error() << "boucle !!!!! " << robot.asserv().getIgnoreFrontCollision()
				<< logs::end;
		usleep(100000);
	}
	robot.svgPrintPosition();

	if (robot.getMyColor() == PMXYELLOW)
	{
		robot.actions().nose_turn(1023);
		usleep(200000);
		robot.actions().nose_turn(0);
	}
	else
	{
		robot.actions().nose_turn(1023);
		usleep(1500000);
		robot.actions().nose_turn(0);
	}

	//robot.actions().nose_turn(1023);
	//sleep(1);
	//robot.actions().nose_turn(0);

	robot.actions().arm_center(150);
	robot.actions().arm_retract(150);
	sleep(1);

	//bascule 45 deg
	robot.actions().arm_semi_deploy(150);
	sleep(1);
	//lache le plot
	robot.actions().nose_up(150);

	robot.actions().arm_retract(150);

	robot.asserv().doLineAbs(50);
	robot.asserv().doLineAbs(-50);

	if (robot.getMyColor() == PMXYELLOW)
	{
		robot.asserv().doRotateAbs(90);
	}
	else
		robot.asserv().doRotateAbs(-90);

	robot.actions().nose_down(200);
	robot.actions().nose_up(150);
	robot.actions().nose_down(200);

	/*

	 robot.logger().info() << "doMoveForwardTo(700, 1200)" << logs::end;
	 while (robot.asserv().doMoveForwardTo(700, 1200) != TRAJ_OK)
	 {
	 usleep(500000);
	 }
	 robot.asserv().ignoreFrontCollision(true);


	 robot.asserv().doFaceTo(900, 1400);
	 robot.svgPrintPosition();
	 robot.asserv().doLineAbs(-100);
	 robot.svgPrintPosition();

	 robot.logger().info() << "on se prepare à kicker" << logs::end;
	 robot.actions().arm_retract(150);
	 robot.actions().arm_center(200);

	 robot.logger().info() << "on kicke !!!" << logs::end;
	 robot.actions().arm_kick(50);
	 robot.asserv().doLineAbs(150);
	 robot.svgPrintPosition();
	 robot.asserv().doLineAbs(-150);
	 robot.svgPrintPosition();
	 */
	/*
	 robot.logger().info() << "sortir arm" << logs::end;
	 robot.actions().arm_deploy();
	 robot.actions().nose_back();

	 robot.logger().info() << "GO" << logs::end;
	 //avancer
	 robot.asserv().doMoveForwardTo(950, 170);
	 robot.svgPrintPosition();
	 robot.logger().info() << "prendre le 1er" << logs::end;
	 if (robot.getMyColor() == PMXYELLOW)
	 robot.actions().arm_right_full();
	 else
	 robot.actions().arm_left_full();

	 robot.actions().nose_down();
	 usleep(1500000);
	 robot.actions().arm_retract();
	 robot.actions().arm_back_cylinder();
	 usleep(1500000);

	 //robot.asserv().setLowSpeed(true);

	 if (robot.getMyColor() == PMXYELLOW)
	 robot.asserv().doRotateLeft(5);
	 else
	 robot.asserv().doRotateRight(5);
	 robot.asserv().doLineAbs(-20);
	 robot.asserv().doMoveForwardTo(1000, 170);
	 robot.svgPrintPosition();
	 if (robot.getMyColor() == PMXYELLOW)
	 robot.asserv().doRotateLeft(3);
	 else
	 robot.asserv().doRotateRight(3);

	 robot.logger().info() << "faire trembler" << logs::end;
	 if (robot.getMyColor() == PMXYELLOW)
	 {
	 robot.actions().turn_nene_right_trembl();
	 }
	 else
	 {
	 robot.actions().turn_nene_left_trembl();
	 }

	 robot.logger().info() << "on prend le 4ème" << logs::end;
	 if (robot.getMyColor() == PMXYELLOW)
	 {
	 robot.actions().turn_nene_center_right();
	 }
	 else
	 {
	 robot.actions().turn_nene_center_left();
	 }
	 robot.svgPrintPosition();
	 robot.asserv().doMoveForwardTo(1300, 600);
	 robot.svgPrintPosition();
	 */
	robot.logger().info() << "O_take_tower1 done." << logs::end;
	return true;
}

void O_State_DecisionMaker::IASetupHomologation()
{
	logger().debug() << "IASetupHomologation" << logs::end;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	robot.ia().iAbyZone().ia_createZone("depart", 0, 710, 100, 100, 710, 100, 180);
	robot.ia().iAbyZone().ia_createZone("zone_depose", 1000, 1000, 300, 200, 1000, 1100, 90);

	robot.ia().iAbyZone().ia_setPath("depart", "zone_depose", 1000, 500);

	robot.ia().iAbyZone().ia_addAction("take_tower1", &O_take_tower1);
	//robot.ia().iAbyZone().ia_addAction("take_tower1", &O_action1);
}

IAutomateState*
O_State_DecisionMaker::execute(Robot &, void *data)
{
	logger().info() << "O_State_DecisionMaker" << logs::end;
	Data* sharedData = (Data*) data; //TODO objet sur robot.data ?

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	robot.sharedData = (Data*) data;

	robot.svgPrintPosition();

	//choisir IA suivant sharedData->ia string
	//IASetupDemo();
	IASetupHomologation();

	robot.actions().sensors().startSensors();
	robot.ia().iAbyZone().ia_start(); //launch IA

	robot.actions().servo_init_end();
	robot.baseStop();
	robot.actions().servoObjects().releaseAll();

	//wait the execution FUNNYACTION... 95s ?
	while (!sharedData->end90s()) //&& robot.chronometerRobot().getElapsedTimeInSec() < 35)
	{
		logger().debug() << "O_State_DecisionMaker sharedData->end90s=" << sharedData->end90s()
				<< " time=" << robot.chrono().getElapsedTimeInSec() << logs::end;
//			robot.base().stop();
		usleep(500000);
	}

	//FUNNY ACTION
	this->logger().info() << "FUNNY ACTION...start... " << robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.actions().funnyAction_Activate();
	sleep(4);

	robot.stop();
	return NULL; //finish all state
}

