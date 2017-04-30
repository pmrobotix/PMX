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

bool O_action2()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
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

bool O_action3()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
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

void O_State_DecisionMaker::IASetupDemo()
{
	logger().debug() << "IASetupDemo" << logs::end;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

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

}

void O_State_DecisionMaker::IASetupHomologation()
{
	logger().debug() << "IASetupHomologation" << logs::end;


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
	IASetupDemo();


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

