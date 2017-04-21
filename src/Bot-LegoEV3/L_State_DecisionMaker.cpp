#include "L_State_DecisionMaker.hpp"

#include <unistd.h>

#include "../Common/IA/IAbyZone.hpp"
#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

int plotdepart_done = 0;

bool L_plotdepart()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.logger().info() << "L_plotdepart" << logs::end;

	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.asserv().ignoreFrontCollision(true);
	robot.asserv().ignoreRearCollision(true);

	robot.ia().iAbyZone().goToZone("zone_plotdepart", &path, &zone);
	ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	if (ts != TRAJ_OK) return false;
	robot.svgPrintPosition();

	//abaisser la pince
	robot.actions().pince_HerculeDown();

	ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	if (ts != TRAJ_OK) return false;
	robot.svgPrintPosition();

	//prendre le plot car on est deja dessus
	robot.actions().pince_Close(0);

	//avancer sur le plot de depart
	ts = robot.asserv().doMoveForwardTo(1000.0, 300.0);
	if (ts != TRAJ_OK) return false;
	robot.svgPrintPosition();

	//prendre le plot car on est deja dessus
	robot.actions().pince_Rotate();

	robot.asserv().ignoreFrontCollision(false);

	//aller à la zone de depose
	robot.ia().iAbyZone().goToZone("zone_deposeplotdepart", &path, &zone);
	while (robot.asserv().doMoveForwardTo(path.x, path.y) != TRAJ_OK)
	{
		usleep(1000);
	}
	robot.svgPrintPosition();

	while (robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta) != TRAJ_OK)
	{
		usleep(1000);
	}
	robot.svgPrintPosition();

	//lacher le plot
	robot.actions().pince_Open();

	robot.asserv().ignoreFrontCollision(true);

	//on recule de 10cm
	while (robot.asserv().doLineAbs(-100) != TRAJ_OK)
	{
		usleep(1000);
	}
	robot.svgPrintPosition();

	//remettre la pince prete a prendre
	robot.actions().pince_InitRotation();
	robot.actions().pince_Open();

	return true; //return true si ok sinon false si interruption
}

bool L_plot2()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.logger().info() << "L_plot2" << logs::end;

	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.ia().iAbyZone().goToZone("zone_plot2", &path, &zone);
	//ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	//if (ts != TRAJ_OK) return false;
	//robot.svgPrintPosition();
	ts = robot.asserv().doMoveForwardTo(zone.x, zone.y);
	if (ts != TRAJ_OK) return false;
	robot.svgPrintPosition();

	//prendre plot2
	robot.actions().pince_Close(0);
	robot.actions().pince_Rotate();

	while (robot.asserv().doLineAbs(-350) != TRAJ_OK)
	{
		usleep(1000);
	}
	robot.svgPrintPosition();

	robot.ia().iAbyZone().goToZone("zone_deposeplotdepart", &path, &zone);
	// ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	//if (ts != TRAJ_OK) return false;
	//robot.svgPrintPosition();
	while (robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y - 100.0, zone.theta) != TRAJ_OK)
	{
		usleep(1000);
	}
	robot.svgPrintPosition();

	//lacher plot2
	robot.actions().pince_Open();

	//on recule de 10cm
	while (robot.asserv().doLineAbs(-100) != TRAJ_OK)
	{
		usleep(1000);
	}
	robot.svgPrintPosition();

	//remettre la pince prete a prendre
	robot.actions().pince_InitRotation();
	robot.actions().pince_Open();

	return true; //return true si ok sinon false si interruption
}

void L_State_DecisionMaker::IASetupHomologation()
{
	logger().debug() << "IAHomologation" << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.ia().iAbyZone().ia_createZone("depart", 0, 0, 300, 300, 100, 180, 0);
	robot.ia().iAbyZone().ia_createZone("zone_plotdepart", 710, 0, 300, 300, 780, 180, 0);
	robot.ia().iAbyZone().ia_createZone("zone_deposeplotdepart", 0, 900, 100, 100, 310, 900, 180);
	robot.ia().iAbyZone().ia_createZone("zone_plot2", 100, 500, 200, 100, 200, 550, -135);

	robot.ia().iAbyZone().ia_setPath("depart", "zone_plotdepart", 590, 180);
	robot.ia().iAbyZone().ia_setPath("depart", "zone_deposeplotdepart", 1200, 400); // ne sert pas
	robot.ia().iAbyZone().ia_setPath("depart", "zone_plot2", 1200, 410); // ne sert pas
	robot.ia().iAbyZone().ia_setPath("zone_plotdepart", "zone_deposeplotdepart", 1300, 800);
	robot.ia().iAbyZone().ia_setPath("zone_plotdepart", "zone_plot2", 500, 800);

	robot.ia().iAbyZone().ia_setPath("zone_deposeplotdepart", "zone_plot2", 350, 750);

	robot.ia().iAbyZone().ia_addAction("plotdepart", &L_plotdepart);
	robot.ia().iAbyZone().ia_addAction("plot2", &L_plot2);

}

IAutomateState*
L_State_DecisionMaker::execute(Robot &, void * data)
{
	logger().info() << "L_State_DecisionMaker" << logs::end;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//Simplifier l'utilisation de data par robot
	//Data* sharedData = (Data*) data;
	robot.sharedData = (Data*) data;

	IASetupHomologation();

	robot.svgPrintPosition();
	logger().info() << "L_State_DecisionMaker => GO..." << logs::end;

	robot.actions().sensors().startSensors(); //Activation de la detection adverse

	robot.ia().iAbyZone().ia_start(); //launch IA

	robot.baseStop();
	robot.actions().servoObjects().releaseAll();

	//wait the execution Wait90
	//while (!sharedData->end90s()) //&& robot.chronometerRobot().getElapsedTimeInSec() < 35)
	while (!robot.sharedData->end90s())
	{
		logger().info() << "robot.sharedData->end90s=" << robot.sharedData->end90s() << " time="
				<< robot.chrono().getElapsedTimeInSec() << logs::end;
		//			robot.base().stop();
		usleep(200000);
	}

	//FUNNY ACTION
	this->logger().info() << "FUNNY ACTION...start... " << robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.actions().funnyAction_Activate();
	sleep(4);
	//robot.actions().parasol().activate(-150);
	//robot.actions().funnyAction().release();
	//sleep(1);

	robot.actions().servoObjects().releaseAll();

	this->logger().info() << "FUNNY ACTION...stop... " << robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.stop();

	return NULL; //finish all state
}

/*

 bool L_tour1()
 {

 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

 robot.logger().info() << "start L_tour1." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition path, zone;

 robot.ia().iAbyZone().goToZone("tour1", &path, &zone);
 ts = robot.asserv().doMoveForwardTo(path.x, path.y);
 if (ts != TRAJ_OK)
 return false;
 robot.svgPrintPosition();

 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
 if (ts != TRAJ_OK)
 return false;
 robot.svgPrintPosition();

 robot.asserv().ignoreFrontCollision(true);
 robot.asserv().ignoreRearCollision(true);
 ts = robot.asserv().doMoveForwardTo(520, 1090);
 if (ts != TRAJ_OK)
 return false;
 robot.svgPrintPosition();

 ts = robot.asserv().doMoveForwardTo(1200, 1080);
 if (ts != TRAJ_OK)
 return false;
 robot.svgPrintPosition();

 ts = robot.asserv().doLineAbs(-150);
 if (ts != TRAJ_OK)
 return false;
 robot.svgPrintPosition();

 robot.asserv().ignoreFrontCollision(false);
 robot.asserv().ignoreRearCollision(false);

 robot.logger().info() << "L_tour1 done." << logs::end;
 return true; //return true si ok sinon false si interruption
 }

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

 void L_State_DecisionMaker::IASetupDemo()
 {
 logger().debug() << "IASetup" << logs::end;

 //LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

 robot.ia().iAbyZone().ia_createZone("depart", 0, 900, 200, 200, 300, 1000, 180);
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
 */
