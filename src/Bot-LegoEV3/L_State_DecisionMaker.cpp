#include "L_State_DecisionMaker.hpp"

#include <unistd.h>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyZone.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
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

	robot.logger().info() << "abaisser un peu la pince" << logs::end;
	robot.actions().pince_HerculeMiddle();

	robot.logger().info() << "on recule pour passer la bascule" << logs::end;
	/*robot.asserv().setAccel(0.3);
	robot.asserv().setVmax(0.4);
	robot.asserv().setDecel(0.2);*/

	//ts = robot.asserv().doLineAbs(-770.0);
	while (robot.asserv().doMoveBackwardTo(980, robot.asserv().pos_getY_mm()) != TRAJ_OK)
	{
		usleep(20000);
	}

	robot.svgPrintPosition();

	/*robot.asserv().setAccel(0.2);
	robot.asserv().setVmax(0.3);
	robot.asserv().setDecel(0.4);*/

	robot.logger().info() << "on claque la pince pour retirer les grosses roues" << logs::end;
	robot.actions().pince_HerculeDown();
	robot.actions().pince_HerculeMiddle();

	robot.logger().info() << "recalage sur bascule" << logs::end;
	robot.asserv().base()->moveDTime(120, 2500);
	robot.asserv().setPositionAndColor(710.0 + 114.0, robot.asserv().pos_getY_mm(), -180.0,
			(robot.getMyColor() != PMXYELLOW));
	robot.svgPrintPosition();

	robot.logger().info() << "recalage sur bordure" << logs::end;
	robot.asserv().doLineAbs(-60.0);
	robot.svgPrintPosition();

	if (robot.getMyColor() == PMXYELLOW)
		ts = robot.asserv().doRotateAbs(-90.0);
	else
		ts = robot.asserv().doRotateAbs(90.0);
	robot.svgPrintPosition();

	robot.asserv().base()->moveDTime(-120, 2500);
//	robot.asserv().setPositionAndColor(
//			robot.asserv().getRelativeX(robot.asserv().pos_getX_mm()) - 30, 60.0 +30,
//			90.0, (robot.getMyColor() != PMXYELLOW)); //attention ici on pense en match jaune
	robot.asserv().setPositionAndColor(864.0, 60.0, 90.0, (robot.getMyColor() != PMXYELLOW)); //attention ici on pense en match jaune

	robot.svgPrintPosition();

	robot.logger().info() << "POSITION RECALEE x=" << robot.asserv().pos_getX_mm() << " y="
			<< robot.asserv().pos_getY_mm() << logs::end;

	robot.logger().info() << "on abaisse la pince " << logs::end;
	robot.actions().pince_HerculeDown();
	robot.logger().info() << "on se decale de la bordure " << logs::end;
	ts = robot.asserv().doLineAbs(250.0);

	robot.asserv().ignoreFrontCollision(false);

	//attente apres recalage
	//sleep(1);

	robot.logger().info() << "on ne va plus chercher le plot 1 " << logs::end;
	while (robot.asserv().doMoveForwardTo(1000, 700) != TRAJ_OK)
	{
		usleep(500000);
	}

	/*

	 robot.logger().info() << "on va chercher le plot 1 " << logs::end;
	 while (robot.asserv().doMoveForwardTo(1000, 600, -180) != TRAJ_OK)
	 {
	 usleep(500000);
	 }
	 robot.logger().info() << "on ferme la pince " << logs::end;
	 robot.actions().pince_Close(0);

	 robot.logger().info() << "on pose le plot sur l'aire de depart " << logs::end;
	 //ts = robot.asserv().doMoveForwardTo(850,300,-200);
	 if (robot.getMyColor() == PMXYELLOW)
	 ts = robot.asserv().doRotateRight(180.0);
	 else
	 ts = robot.asserv().doRotateLeft(180.0);
	 //lacher le plot
	 robot.logger().info() << "on lache le plot " << logs::end;
	 robot.actions().pince_Open();

	 robot.logger().info() << "on se decale " << logs::end;
	 ts = robot.asserv().doLineAbs(-300);
	 */

	robot.logger().info() << "aller à la zone de depose " << logs::end;
	robot.ia().iAbyZone().goToZone("zone_plot2", &path, &zone);
	if (robot.getMyColor() == PMXYELLOW)
	{
		while (robot.asserv().doMoveForwardTo(path.x, path.y) != TRAJ_OK)
		{
			usleep(500000);
		}
	}
	else
	{
		while (robot.asserv().doMoveForwardTo(path.x, path.y + 50) != TRAJ_OK)
		{
			usleep(500000);
		}
	}
	robot.svgPrintPosition();

	robot.logger().info() << "on prend le plot 2 " << logs::end;
	if (robot.getMyColor() == PMXYELLOW)
	{
		while (robot.asserv().doMoveForwardTo(zone.x, zone.y + 30, -90) != TRAJ_OK)
		{
			usleep(500000);
		}
	}
	else
	{
		while (robot.asserv().doMoveForwardTo(zone.x, zone.y + 100, -30) != TRAJ_OK)
		{
			usleep(500000);
		}
	}
	robot.svgPrintPosition();

	robot.asserv().ignoreFrontCollision(true);

	robot.logger().info() << "on ferme la pince " << logs::end;
	robot.actions().pince_Close(0);
	robot.actions().pince_Rotate();

	ts = robot.asserv().doLineAbs(-150);

	robot.logger().info() << "zone de depose 1" << logs::end;
	if (robot.getMyColor() == PMXYELLOW)
	{
		while (robot.asserv().doMoveForwardAndRotateTo(290, 900, -180) != TRAJ_OK)
		{
			usleep(20000);
		}
	}
	else
	{
		while (robot.asserv().doMoveForwardAndRotateTo(280, 800, -180) != TRAJ_OK)
		{
			usleep(20000);
		}
	}

	robot.actions().pince_Open();

	robot.logger().info() << "on se decale " << logs::end;
	ts = robot.asserv().doLineAbs(-30);
//----------------------------------
	//on se tourne
	while (robot.asserv().doFaceTo(600, robot.asserv().pos_getY_mm()) != TRAJ_OK)
	{
		usleep(20000);
	}

	robot.logger().info() << "on se recale " << logs::end;
	robot.asserv().base()->moveDTime(-120, 4000);
	robot.asserv().setPositionAndColor(214.0, robot.asserv().pos_getY_mm(), 0.0,
			(robot.getMyColor() != PMXYELLOW)); //attention ici on pense en match jaune
	ts = robot.asserv().doLineAbs(50);

	robot.actions().pince_InitRotation();

	while (robot.asserv().doMoveForwardTo(300, 1250) != TRAJ_OK)
	{
		usleep(20000);
	}

	while (robot.asserv().doMoveForwardTo(500, 1400) != TRAJ_OK)
	{
		usleep(20000);
	}

	//MATCH
	while (robot.asserv().doMoveForwardTo(800, 1850, -80) != TRAJ_OK)
	{
		usleep(20000);
	}

	//TABLE TEST
//	while (robot.asserv().doMoveForwardTo(600, 1450) != TRAJ_OK)
//	{
//		usleep(20000);
//	}

	robot.logger().info() << "on ferme la pince " << logs::end;
	robot.actions().pince_Close(0);
	robot.actions().pince_Rotate();

	while (robot.asserv().doMoveForwardTo(800, 1850) != TRAJ_OK)
	{
		usleep(20000);
	}


	//ts = robot.asserv().doLineAbs(-150);

	//on va deposer

//	while (robot.asserv().doMoveForwardTo(1000, 1600, -100) != TRAJ_OK)
//	{
//		usleep(20000);
//	}

	while (robot.asserv().doFaceTo(1100, 1600) != TRAJ_OK)
	{
		usleep(20000);
	}

	robot.asserv().doLineAbs(10);
	robot.actions().pince_Open();
	robot.asserv().doLineAbs(120);

	/*
	 while (robot.asserv().doMoveForwardTo(300, 1250) != TRAJ_OK)
	 {
	 usleep(20000);
	 }

	 robot.asserv().setDecel(0.50);
	 robot.asserv().setVmax(0.5);
	 robot.logger().info() << "on kick " << logs::end;
	 if (robot.getMyColor() == PMXYELLOW)
	 {
	 while (robot.asserv().doMoveForwardTo(850, 1470, -110) != TRAJ_OK)
	 {
	 usleep(20000);
	 }
	 ts = robot.asserv().doLineAbs(-100);
	 while (robot.asserv().doMoveForwardTo(900, 1470, -110) != TRAJ_OK)
	 {
	 usleep(20000);
	 }
	 }
	 else
	 {
	 while (robot.asserv().doMoveForwardTo(850, 1500, -110) != TRAJ_OK)
	 {
	 usleep(20000);
	 }
	 ts = robot.asserv().doLineAbs(-100);
	 while (robot.asserv().doMoveForwardTo(900, 1470, -110) != TRAJ_OK)
	 {
	 usleep(20000);
	 }
	 }

	 robot.asserv().setAccel(0.2);
	 robot.asserv().setVmax(0.3);
	 robot.asserv().setDecel(0.4);
	 ts = robot.asserv().doLineAbs(-80);

	 //a la place de kicker
	 while (robot.asserv().doMoveForwardTo(850, 1470, -110) != TRAJ_OK)
	 {
	 usleep(20000);
	 }

	 robot.actions().pince_InitRotation();
	 robot.actions().pince_Open();

	 while (robot.asserv().doMoveForwardTo(500, 1100, -120) != TRAJ_OK)
	 {
	 usleep(20000);
	 }

	 robot.actions().pince_Close(0);

	 while (robot.asserv().doMoveForwardTo(1000, 800) != TRAJ_OK)
	 {
	 usleep(20000);
	 }

	 while (robot.asserv().doMoveForwardTo(1000, 300, -220) != TRAJ_OK)
	 {
	 usleep(20000);
	 }

	 robot.actions().pince_Open();
	 ts = robot.asserv().doLineAbs(-100);*/
	/*
	 //on avance
	 robot.asserv().setAccel(0.4);
	 robot.asserv().setVmax(0.6);
	 robot.asserv().setDecel(0.4);


	 ts = robot.asserv().doLineAbs(-450.0);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();


	 //abaisser la pince
	 robot.actions().pince_HerculeMiddle();

	 robot.asserv().setAccel(0.4);
	 robot.asserv().setVmax(0.7);
	 robot.asserv().setDecel(0.4);

	 ts = robot.asserv().doLineAbs(-450.0);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();
	 */

	/*
	 robot.svgPrintPosition();
	 ts = robot.asserv().doLineAbs(-450.0);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();

	 robot.asserv().setVmax(0.2);
	 robot.asserv().setAccel(0.2);
	 //abaisser la pince
	 robot.actions().pince_HerculeDown();
	 sleep(1);
	 ts = robot.asserv().doLineAbs(-150.0);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();

	 */

	//abaisser la pince
	//robot.actions().pince_HerculeDown();
	/*
	 robot.asserv().setAccel(0.1);
	 robot.asserv().setVmax(0.1);
	 robot.asserv().setDecel(0.4);
	 robot.ia().iAbyZone().goToZone("zone_plot1", &path, &zone);
	 while (robot.asserv().doMoveForwardTo(zone.x, zone.y) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();
	 */
	/*
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
	 ts = robot.asserv().doMoveForwardTo(1100.0, 300.0);
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
	 */
	return true; //return true si ok sinon false si interruption
}

bool L_plot1()
{
	/*
	 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	 robot.logger().info() << "L_plotdepart" << logs::end;

	 robot.asserv().ignoreFrontCollision(true);
	 robot.asserv().ignoreRearCollision(true);

	 //TRAJ_STATE ts = TRAJ_OK;
	 RobotPosition path, zone;

	 robot.ia().iAbyZone().goToZone("zone_plot1", &path, &zone);
	 while (robot.asserv().doMoveForwardTo(path.x, path.y) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 //abaisser la pince
	 robot.actions().pince_HerculeDown();

	 while (robot.asserv().doMoveForwardTo(zone.x, zone.y) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 robot.asserv().ignoreFrontCollision(false);
	 robot.asserv().ignoreRearCollision(false);

	 while (robot.asserv().doMoveForwardTo(1000.0, 800.0) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 //prendre le plot car on est deja dessus
	 robot.actions().pince_Close(0);
	 robot.actions().pince_Rotate();

	 //aller à la zone de depose
	 robot.ia().iAbyZone().goToZone("zone_deposeplot", &path, &zone);
	 while (robot.asserv().doMoveForwardTo(path.x, path.y) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 robot.asserv().ignoreFrontCollision(true);

	 while (robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 //lacher le plot
	 robot.actions().pince_Open();

	 //on recule de 10cm
	 while (robot.asserv().doLineAbs(-200) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 //remettre la pince prete a prendre
	 robot.actions().pince_InitRotation();
	 robot.actions().pince_Open();

	 robot.asserv().ignoreFrontCollision(false);
	 */
	return true; //return true si ok sinon false si interruption
}

bool L_plot2()
{/*
 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
 robot.logger().info() << "L_plot2" << logs::end;

 //TRAJ_STATE ts = TRAJ_OK;
 RobotPosition path, zone;

 robot.ia().iAbyZone().goToZone("zone_plot2", &path, &zone);
 //ts = robot.asserv().doMoveForwardTo(path.x, path.y);
 //if (ts != TRAJ_OK) return false;
 //robot.svgPrintPosition();
 while (robot.asserv().doMoveForwardTo(zone.x, zone.y) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 //prendre plot2
 robot.actions().pince_Close(0);
 robot.actions().pince_Rotate();

 while (robot.asserv().doLineAbs(-250) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 robot.ia().iAbyZone().goToZone("zone_deposeplot", &path, &zone);
 // ts = robot.asserv().doMoveForwardTo(path.x, path.y);
 //if (ts != TRAJ_OK) return false;
 //robot.svgPrintPosition();
 while (robot.asserv().doMoveForwardAndRotateTo(zone.x - 1.0, zone.y + 140.0, 180.0) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 //lacher plot2
 robot.actions().pince_Open();

 //on recule de 10cm
 while (robot.asserv().doLineAbs(-400) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 //on va kicker!
 if (robot.getMyColor() == PMXBLUE)
 {
 while (robot.asserv().doMoveForwardAndRotateTo(700, 1250, 45.0) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();
 }
 else if (robot.getMyColor() == PMXYELLOW)
 {
 while (robot.asserv().doMoveForwardAndRotateTo(780, 1200, 45.0) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();
 }

 robot.asserv().doLineAbs(200);

 robot.svgPrintPosition();

 //on recule de 10cm
 while (robot.asserv().doLineAbs(-200) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 //remettre la pince prete a prendre
 robot.actions().pince_InitRotation();
 robot.actions().pince_Open();
 */
	return true; //return true si ok sinon false si interruption
}

bool L_plot3()
{/*
 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
 robot.logger().info() << "L_plot2" << logs::end;

 //TRAJ_STATE ts = TRAJ_OK;
 RobotPosition path, zone;

 robot.ia().iAbyZone().goToZone("zone_plot3", &path, &zone);
 while (robot.asserv().doMoveForwardTo(zone.x, zone.y) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();
 if (robot.getMyColor() == PMXBLUE)
 {
 while (robot.asserv().doMoveForwardTo(750, 1770) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();
 }
 else if (robot.getMyColor() == PMXYELLOW)
 {
 while (robot.asserv().doMoveForwardTo(795, 1750) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 }

 //prendre plot3
 robot.actions().pince_Close(0);
 robot.actions().pince_Rotate();

 while (robot.asserv().doLineAbs(-250) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 //ON VA DEPOSER
 robot.ia().iAbyZone().goToZone("zone_deposeplot", &path, &zone);
 while (robot.asserv().doMoveForwardTo(path.x, path.y) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();
 while (robot.asserv().doMoveForwardAndRotateTo(zone.x - 1.0, zone.y + 300.0, 179.0) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 while (robot.asserv().doRotateTo(179.0) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 //lacher plot3
 robot.actions().pince_Open();

 //on recule en se positionnant
 while (robot.asserv().doMoveBackwardTo(300, 900) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 if (robot.sharedData->end90s()) return true;

 //remettre la pince prete a prendre
 robot.actions().pince_InitRotation();
 robot.actions().pince_Open();

 //on prend le dernier plot
 while (robot.asserv().doMoveForwardAndRotateTo(400, 1000, 45) != TRAJ_OK)
 {
 usleep(1000);
 }
 robot.svgPrintPosition();

 if (robot.sharedData->end90s()) return true;

 //prendre plot4
 robot.actions().pince_Close(0);
 robot.actions().pince_Rotate();

 if (robot.sharedData->end90s()) return true;*/
	/*
	 while (robot.asserv().doMoveForwardTo(1250, 1100) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 if(robot.sharedData->end90s()) return true;

	 while (robot.asserv().doMoveForwardAndRotateTo(1300, 1400, 135) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();
	 */
	/*
	 robot.asserv().ignoreFrontCollision(false);

	 if (robot.getMyColor() == PMXBLUE)
	 {
	 while (robot.asserv().doMoveForwardTo(1220, 1350) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();
	 }
	 else if (robot.getMyColor() == PMXYELLOW)
	 {
	 while (robot.asserv().doMoveForwardTo(1250, 1350) != TRAJ_OK)
	 {
	 usleep(1000);
	 }
	 robot.svgPrintPosition();

	 }

	 if (robot.sharedData->end90s()) return true;

	 //lacher plot4
	 robot.actions().pince_Open();
	 */
	return true; //return true si ok sinon false si interruption
}

bool L_plot_renvers()
{
	/*
	 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	 robot.logger().info() << "L_plot_renvers" << logs::end;

	 TRAJ_STATE ts = TRAJ_OK;
	 RobotPosition path, zone;

	 robot.ia().iAbyZone().goToZone("zone_renvers", &path, &zone);
	 //ts = robot.asserv().doMoveForwardTo(path.x, path.y);
	 //if (ts != TRAJ_OK) return false;
	 //robot.svgPrintPosition();
	 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();

	 ts = robot.asserv().doMoveForwardAndRotateTo(500, 1050, 50);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();

	 //fermer la pince
	 robot.actions().pince_Close(0);
	 sleep(1);
	 //lever la pince
	 robot.actions().pince_HerculeUp8cm(0);

	 ts = robot.asserv().doMoveForwardTo(900, 1350);
	 if (ts != TRAJ_OK) return false;
	 robot.svgPrintPosition();
	 */
	return true; //return true si ok sinon false si interruption
}
/*
 void L_State_DecisionMaker::IASetupHomologation()
 {
 logger().debug() << "IAHomologation" << logs::end;
 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

 robot.ia().iAbyZone().ia_createZone("depart", 0, 0, 300, 300, 100, 225, 0);
 robot.ia().iAbyZone().ia_createZone("zone_plotdepart", 710, 0, 300, 300, 800, 200, 0);
 robot.ia().iAbyZone().ia_createZone("zone_deposeplotdepart", 0, 900, 100, 100, 330, 900, 180);
 robot.ia().iAbyZone().ia_createZone("zone_plot2", 100, 500, 200, 100, 280, 690, -145);
 robot.ia().iAbyZone().ia_createZone("zone_renvers", 500, 1000, 100, 100, 350, 850, 55);

 robot.ia().iAbyZone().ia_setPath("depart", "zone_plotdepart", 625, 225);
 robot.ia().iAbyZone().ia_setPath("depart", "zone_deposeplotdepart", 1250, 400); // ne sert pas
 robot.ia().iAbyZone().ia_setPath("depart", "zone_plot2", 1200, 410); // ne sert pas
 robot.ia().iAbyZone().ia_setPath("depart", "zone_renvers", 1200, 420); // ne sert pas
 robot.ia().iAbyZone().ia_setPath("zone_plotdepart", "zone_deposeplotdepart", 1300, 800);
 robot.ia().iAbyZone().ia_setPath("zone_plotdepart", "zone_plot2", 500, 800);
 robot.ia().iAbyZone().ia_setPath("zone_plotdepart", "zone_renvers", 1200, 430); //ne sert pas

 robot.ia().iAbyZone().ia_setPath("zone_deposeplotdepart", "zone_plot2", 350, 750);
 robot.ia().iAbyZone().ia_setPath("zone_deposeplotdepart", "zone_renvers", 1200, 440); //ne sert pas

 robot.ia().iAbyZone().ia_setPath("zone_plot2", "zone_renvers", 1200, 450); //ne sert pas

 robot.ia().iAbyZone().ia_addAction("plotdepart", &L_plotdepart);
 robot.ia().iAbyZone().ia_addAction("plot2", &L_plot2);
 robot.ia().iAbyZone().ia_addAction("plot_renvers", &L_plot_renvers);
 }
 */
void L_State_DecisionMaker::IASetupMatchesBelgique()
{
	logger().debug() << "IASetupMatchesBelgique" << logs::end;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.ia().iAbyZone().ia_createZone("depart", 710, 0, 300, 300, 800, 200, 90);
	robot.ia().iAbyZone().ia_createZone("zone_plot1", 900, 500, 200, 100, 1000, 400, 90);

	if (robot.getMyColor() == PMXBLUE)
	{
		robot.ia().iAbyZone().ia_createZone("zone_deposeplot", 0, 700, 100, 400, 300, 770, 180);
		robot.ia().iAbyZone().ia_createZone("zone_plot2", 100, 500, 200, 100, 280, 600, -135);
	}
	else if (robot.getMyColor() == PMXYELLOW)
	{
		robot.ia().iAbyZone().ia_createZone("zone_deposeplot", 0, 700, 100, 400, 300, 840, 180);
		robot.ia().iAbyZone().ia_createZone("zone_plot2", 100, 500, 200, 100, 200, 600, -135);

	}

	robot.ia().iAbyZone().ia_createZone("zone_plot3", 700, 1700, 200, 100, 700, 1500, 90);

	robot.ia().iAbyZone().ia_setPath("depart", "zone_plot1", 978.5, 200.0);
	robot.ia().iAbyZone().ia_setPath("depart", "zone_deposeplot", 0.0, 400.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("depart", "zone_plot2", 0.0, 410.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("depart", "zone_plot3", 0.0, 440.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("zone_plot1", "zone_deposeplot", 500.0, 800.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("zone_plot1", "zone_plot2", 650, 850.0);
	robot.ia().iAbyZone().ia_setPath("zone_plot1", "zone_plot3", 0.0, 450.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("zone_plot2", "zone_deposeplot", 0.0, 430.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("zone_plot2", "zone_plot3", 0.0, 460.0); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("zone_plot3", "zone_deposeplot", 300.0, 1250.0);

	robot.ia().iAbyZone().ia_addAction("plotdepart", &L_plotdepart);
	robot.ia().iAbyZone().ia_addAction("plot1", &L_plot1);
	robot.ia().iAbyZone().ia_addAction("plot2", &L_plot2);
	robot.ia().iAbyZone().ia_addAction("plot3", &L_plot3);
}

IAutomateState*
L_State_DecisionMaker::execute(Robot &, void * data)
{
	logger().info() << "L_State_DecisionMaker" << logs::end;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//Simplifier l'utilisation de data par robot
	//Data* sharedData = (Data*) data;
	robot.sharedData = (Data*) data;

	//IASetupHomologation();
	IASetupMatchesBelgique();

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
		usleep(500000);
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

