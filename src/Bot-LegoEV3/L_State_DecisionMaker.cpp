#include "L_State_DecisionMaker.hpp"

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv.Insa/AsservInsa.hpp"
#include "../Common/IA/IAbyZone.hpp"
#include "../Common/Position.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"



bool L_action1()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
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

bool L_action2()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
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

bool L_action3()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
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
L_State_DecisionMaker::execute(Robot & , void *)
{
	logger().info() << "A_State_DecisionMaker" << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//robot.asserv().startMotionTimerAndOdo();//
//	robot.asserv().setPositionAndColor(150, 1050, 0.0, (robot.getMyColor() == PMXGREEN));
	robot.svgPrintPosition();
	robot.chrono().start();
	IASetupDemo();
	robot.ia().iAbyZone().ia_start(); //launch IA


	robot.stop();
	return NULL; //finish all state
}

void L_State_DecisionMaker::IASetupDemo()
{
	logger().debug() << "IASetup" << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

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

	robot.ia().iAbyZone().ia_addAction("action1", &L_action1);
	robot.ia().iAbyZone().ia_addAction("action2", &L_action2);
	robot.ia().iAbyZone().ia_addAction("action3", &L_action3);

}


