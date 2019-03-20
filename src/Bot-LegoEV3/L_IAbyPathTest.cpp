/*!
 * \file
 * \brief Implémentation de la classe L_IAByPathTest.
 */

#include "L_IAbyPathTest.hpp"

#include <src/pmr_node.h>
#include <src/pmr_path_result.h>
#include <src/pmr_point.h>
#include <iterator>
#include <string>
#include <vector>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

bool L_IAbyPathTest_action1()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.logger().info() << "start action1." << logs::end;

	/*
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
	 */
	robot.svgPrintPosition();

	robot.logger().info() << "action1 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool L_IAbyPathTest_action2()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.logger().info() << "start action2." << logs::end;
/*
	RobotPosition zone;
	robot.ia().iAbyPath().goToZone("zone_plot2", &zone);

	FoundPath * found_path = NULL;
	Point startPoint =
	{ x : robot.asserv().pos_getX_mm(), y : robot.asserv().pos_getY_mm() };
	Point secondPosition =
	{ x : zone.x, y : zone.y };

	// First move
	robot.ia().playgroundFindPath(found_path, startPoint, secondPosition);

	std::vector<Node*>::iterator nodes_it;
	if (found_path != NULL)
	{
		bool node_found = false;
		//svg::Polyline path_polyline(svg::Stroke(5, svg::Color::Green));

		for (nodes_it = found_path->path.begin(); nodes_it < found_path->path.end(); nodes_it++)
		{
			Node* node = *nodes_it;
			if (!node_found)
			{
				node_found = true;
			}
			//path_polyline << svg::Point(node->x, node->y);
			robot.asserv().doMoveForwardTo(node->x, node->y);
		}
		if (node_found)
		{
			//doc << path_polyline;
		}
	}

	//toSVG(p, path, "testRobot3.svg"); //TODO afficher le path
*/

	//TRAJ_STATE ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);

	/*
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
	 */
	robot.svgPrintPosition();
	//delete found_path;
	robot.logger().info() << "action2 done." << logs::end;
	return true;
}

bool L_IAbyPathTest_action3()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.logger().info() << "start action3." << logs::end;
	/*
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
	 */
	robot.svgPrintPosition();

	robot.logger().info() << "action3 done." << logs::end;
	return true;
}

void L_IAByPathTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.asserv().startMotionTimerAndOdo(true);
	robot.asserv().setPositionAndColor(800, 150, 0.0, (robot.getMyColor() != PMXVIOLET));
	robot.svgPrintPosition();
	robot.chrono().start();
	IASetup();
	robot.ia().iAbyPath().ia_start(); //launch IA

	robot.stopAll();

	logger().info() << "Happy End - " << robot.chrono().getElapsedTimeInSec() << " sec"
			<< logs::end;
}

void L_IAByPathTest::IASetup()
{
	logger().debug() << "IASetup" << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.ia().iAbyPath().ia_createZone("depart", 710, 0, 300, 300, 800, 200, 90);
	robot.ia().iAbyPath().ia_createZone("zone_plot1", 900, 500, 200, 100, 1000, 400, 90);

	if (robot.getMyColor() == PMXYELLOW)
	{
		robot.ia().iAbyPath().ia_createZone("zone_deposeplot", 0, 700, 100, 400, 300, 770, 180);
		robot.ia().iAbyPath().ia_createZone("zone_plot2", 100, 500, 200, 100, 280, 600, -135);
	}
	else if (robot.getMyColor() == PMXVIOLET)
	{
		robot.ia().iAbyPath().ia_createZone("zone_deposeplot", 0, 700, 100, 400, 300, 840, 180);
		robot.ia().iAbyPath().ia_createZone("zone_plot2", 100, 500, 200, 100, 250, 650, -135);

	}

	robot.ia().iAbyZone().ia_createZone("zone_plot3", 700, 1700, 200, 100, 700, 1500, 90);

	robot.ia().iAbyPath().ia_addAction("action1", &L_IAbyPathTest_action1);
	robot.ia().iAbyPath().ia_addAction("action2", &L_IAbyPathTest_action2);
	robot.ia().iAbyPath().ia_addAction("action3", &L_IAbyPathTest_action3);
}
