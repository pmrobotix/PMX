#include "A_State_DecisionMaker.hpp"

#include <unistd.h>

#include "../Asserv.Insa/AsservInsa.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/IA/IAbyZone.hpp"
#include "../Common/Position.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328AsservExtended.hpp"
#include "APF9328IAExtended.hpp"
#include "APF9328RobotExtended.hpp"

bool A_tour2()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start A_tour2." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.ia().iAbyZone().goToZone("tour2", &path, &zone);
	if (&path != NULL)
	{
		ts = robot.asserv().doMoveForwardTo(path.x, path.y);
		if (ts != TRAJ_OK)
			return false;
		robot.svgPrintPosition();
	}

	ts = robot.asserv().doMoveForwardAndRotateTo(600, 1000, -90);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	//ouvrir pinces
	robot.actions().servoObjects().leftDeploy(80, false); //deploy
	robot.actions().servoObjects().rightDeploy(80, false); //deploy

	if (robot.sharedData->strategy() == "strat5")
	{
		robot.logger().info() << "strat5." << logs::end;
		ts = robot.asserv().doMoveForwardAndRotateTo(700, 800, -90);
		if (ts != TRAJ_OK)
			return false;
		robot.svgPrintPosition();
	}
	else
	{
		robot.logger().info() << "All." << logs::end;
		ts = robot.asserv().doMoveForwardAndRotateTo(700, 550, -90);
		if (ts != TRAJ_OK)
			return false;
		robot.svgPrintPosition();
	}

	ts = robot.asserv().doMoveForwardTo(400, 450);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	//ouvrir pinces
	robot.actions().servoObjects().leftDeploy(0, false); //deploy
	robot.actions().servoObjects().rightDeploy(0, false); //deploy

	ts = robot.asserv().doMoveForwardTo(200, 1000);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	//ouvrir pinces
	robot.actions().servoObjects().leftDeploy(80, false); //deploy
	robot.actions().servoObjects().rightDeploy(80, false); //deploy

	ts = robot.asserv().doLineAbs(-250);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	/*
	 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();

	 //ouvrir pinces
	 robot.actions().servoObjects().leftDeploy(80, false); //deploy
	 robot.actions().servoObjects().rightDeploy(80, false); //deploy

	 ts = robot.asserv().doMoveForwardAndRotateTo(880, 1730, 90);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();

	 //fermer pinces
	 robot.actions().servoObjects().leftDeploy(30, false); //retract
	 robot.actions().servoObjects().rightDeploy(30, false); //retract

	 //reculer de 30
	 robot.asserv().ignoreFrontCollision(true);
	 ts = robot.asserv().doLineAbs(-380);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();
	 robot.asserv().ignoreFrontCollision(false);

	 ts = robot.asserv().doRotateTo(-180);
	 if (ts != TRAJ_OK)
	 return false;
	 robot.svgPrintPosition();

	 if (robot.getMyColor() == PMXVIOLET)
	 robot.asserv().doMoveArcRotate(180, 400); //todo gerer dans l'autre couleur
	 if (robot.getMyColor() == PMXGREEN)
	 robot.asserv().doMoveArcRotate(-180, -400);
	 robot.svgPrintPosition();
	 */
	robot.logger().info() << "A_tour2 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool A_porte2()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start A_porte2." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.ia().iAbyZone().goToZone("porte2", &path, &zone);
	//if (path !=NULL)
	//{
//	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
//	if (ts != TRAJ_OK)
//		return false;
//	robot.svgPrintPosition();
	//}
	robot.asserv().ignoreRearCollision(true);
	ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	//tentative 01
	ts = robot.asserv().doMoveBackwardAndRotateTo(550, 1970, -90);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	ts = robot.asserv().doMoveBackwardAndRotateTo(550, 1980, -110);

	//tentative 02
	robot.asserv().doLineAbs(100);
	robot.asserv().doLineAbs(-110);

	//move zone
	ts = robot.asserv().doMoveForwardTo(zone.x, zone.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
	robot.asserv().ignoreRearCollision(false);

	robot.logger().info() << "A_porte2 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool A_porte1()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start A_porte1." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.ia().iAbyZone().goToZone("porte1", &path, &zone);
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
	robot.svgPrintPosition();

	robot.asserv().ignoreRearCollision(true);
	ts = robot.asserv().doMoveBackwardAndRotateTo(300, 1980, -90);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();

	ts = robot.asserv().doRotateTo(-110);

	//tentative 02
	robot.asserv().doLineAbs(100);
	robot.asserv().doLineAbs(-110);

	ts = robot.asserv().doMoveForwardTo(zone.x, zone.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
	robot.asserv().ignoreRearCollision(false);

	robot.logger().info() << "A_porte1 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool A_action1()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start action1." << logs::end;
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

	robot.svgPrintPosition();

	robot.logger().info() << "action1 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool A_action2()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start action2." << logs::end;

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

	robot.svgPrintPosition();

	robot.logger().info() << "action2 done." << logs::end;
	return true;
}

bool A_action3()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start action3." << logs::end;

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

	robot.svgPrintPosition();

	robot.logger().info() << "action3 done." << logs::end;
	return true;
}

void A_State_DecisionMaker::IASetupDemo()
{
	logger().debug() << "IASetupDemo" << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

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

	robot.ia().iAbyZone().ia_addAction("action1", &A_action1);
	robot.ia().iAbyZone().ia_addAction("action2", &A_action2);
	robot.ia().iAbyZone().ia_addAction("action3", &A_action3);

}

void A_State_DecisionMaker::IASetupHomologation()
{
	logger().debug() << "IASetupHomologation" << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.ia().iAbyZone().ia_createZone("depart", 0, 1100, 300, 300, 300, 1250, 180);
	robot.ia().iAbyZone().ia_createZone("porte2", 500, 1800, 200, 200, 550, 1600, -90);
	robot.ia().iAbyZone().ia_createZone("porte1", 200, 1800, 200, 200, 300, 1600, -90);
	robot.ia().iAbyZone().ia_createZone("tour2", 800, 1900, 100, 100, 880, 1450, 90);

	robot.ia().iAbyZone().ia_setPath("depart", "tour2", 350, 1250);
	robot.ia().iAbyZone().ia_setPath("depart", "porte2", 550, 1500); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("depart", "porte1", 550, 1450);
	robot.ia().iAbyZone().ia_setPath("porte1", "porte2", 450, 1550);
	robot.ia().iAbyZone().ia_setPath("porte1", "tour2", 600, 1600);
	robot.ia().iAbyZone().ia_setPath("tour2", "porte2", 600, 1600);

	robot.ia().iAbyZone().ia_addAction("porte2", &A_porte2);
	robot.ia().iAbyZone().ia_addAction("porte1", &A_porte1);
	robot.ia().iAbyZone().ia_addAction("tour2", &A_tour2);
}

IAutomateState*
A_State_DecisionMaker::execute(Robot &, void *data)
{
	logger().info() << "A_State_DecisionMaker" << logs::end;
	Data* sharedData = (Data*) data;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.sharedData = (Data*) data;

	robot.svgPrintPosition();

	//choisir IA suivant sharedData->ia string
	//IASetupDemo();
	IASetupHomologation();

	//move 400
//	robot.asserv().doLineAbs(400);
//	robot.svgPrintPosition();
	//clotho

	if (robot.getMyColor() == PMXVIOLET)
		robot.asserv().doMoveArcRotate(55, 450); //todo gerer dans l'autre couleur
	if (robot.getMyColor() == PMXGREEN)
		robot.asserv().doMoveArcRotate(-55, -450);
	robot.svgPrintPosition();

	robot.actions().sensors().startSensors();
	robot.ia().iAbyZone().ia_start(); //launch IA

	//wait the execution Wait90
	while (!sharedData->end90s()) //&& robot.chronometerRobot().getElapsedTimeInSec() < 35)
	{
//			logger().info() << "sharedData->end90s=" << sharedData->end90s() << " time="
//					<< robot.chronometerRobot().getElapsedTimeInSec() << utils::end;
//			robot.base().stop();
		usleep(500000);
	}

	//robot.stop();
	return NULL; //finish all state
}

