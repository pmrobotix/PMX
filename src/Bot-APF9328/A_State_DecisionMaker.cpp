#include "A_State_DecisionMaker.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"
#include "../Common/IA/IAbyZone.hpp"
#include "../Common/Position.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
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
	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
	ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	if (ts != TRAJ_OK)
		return false;

	robot.svgPrintPosition();

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
	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
	ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	if (ts != TRAJ_OK)
		return false;

	robot.svgPrintPosition();

	robot.logger().info() << "A_porte2 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}



bool A_action1()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.logger().info() << "start action1." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.ia().iAbyZone().goToZone("zone1", &path, &zone);
	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
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
	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
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
	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	if (ts != TRAJ_OK)
		return false;
	robot.svgPrintPosition();
	ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	if (ts != TRAJ_OK)
		return false;

	robot.svgPrintPosition();

	robot.logger().info() << "action3 done." << logs::end;
	return true;
}


IAutomateState*
A_State_DecisionMaker::execute(Robot &, void *)
{
	logger().info() << "A_State_DecisionMaker" << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.svgPrintPosition();
	robot.chrono().start();
	//IASetupDemo();
	IASetupHomologation();
	robot.ia().iAbyZone().ia_start(); //launch IA

	robot.stop();
	return NULL; //finish all state
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

	robot.ia().iAbyZone().ia_createZone("tour2", 800, 1900, 100, 100, 900, 1600, 90);
	robot.ia().iAbyZone().ia_createZone("porte2", 500, 1800, 200, 200, 600, 1800, 90);

	robot.ia().iAbyZone().ia_setPath("depart", "tour2", 350,1250);
	robot.ia().iAbyZone().ia_setPath("depart", "porte2", 350,1250); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("tour2", "porte2", 600, 1600);

	robot.ia().iAbyZone().ia_addAction("tour2", &A_tour2);
	robot.ia().iAbyZone().ia_addAction("porte2", &A_porte2);
}

