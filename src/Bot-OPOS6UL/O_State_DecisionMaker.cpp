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

bool O_take_dbu()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_take_tower1." << logs::end;

	robot.asserv().assistedHandling();
	robot.asserv().ignoreFrontCollision(true);
	robot.asserv().ignoreRearCollision(true);

	robot.actions().arm_deploy(1000);
	//robot.actions().nose_back(1000);

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

	float x_depose_milieu;
	float y_depose_milieu;
//MATCH
	x_depose_milieu = 1500;
	y_depose_milieu = 900;
//TABLE DE TEST
	x_depose_milieu = 1200;
	y_depose_milieu = 900;

	while (robot.asserv().doMoveForwardAndRotateTo(x_depose_milieu, y_depose_milieu, 90) != TRAJ_OK)
	{
		robot.logger().error() << "boucle !!!!! " << robot.asserv().getIgnoreFrontCollision()
				<< logs::end;
		usleep(300000);
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
		usleep(800000);
		robot.actions().nose_turn(0);
	}

	robot.actions().arm_center(150);
	robot.actions().arm_retract(150);
	sleep(1);

	//bascule 45 deg
	robot.actions().arm_semi_deploy(150);
	//lache le plot
	robot.actions().nose_up(150);
	robot.actions().arm_retract(150);

	robot.logger().info() << "on pousse, on avance et on recule" << logs::end;
	robot.asserv().doLineAbs(85);
	robot.svgPrintPosition();
	robot.asserv().doLineAbs(-85);
	robot.svgPrintPosition();

	if (robot.getMyColor() == PMXYELLOW)
	{
		robot.asserv().doRotateAbs(90);
	}
	else
		robot.asserv().doRotateAbs(-90);

	robot.svgPrintPosition();
	/*
	 robot.logger().info() << "danse du nez" << logs::end;
	 robot.actions().nose_down(200);
	 robot.actions().nose_up(150);
	 robot.actions().nose_down(200);
	 */

	robot.logger().info() << "on va chercher le 2ème plot" << logs::end;
	while (robot.asserv().doMoveForwardTo(900, 950) != TRAJ_OK)
	{
		usleep(500000);
	}
	robot.svgPrintPosition();

	robot.asserv().doFaceTo(900, 1400);

	robot.actions().arm_deploy(200);

	while (robot.asserv().doMoveForwardTo(900, 1400, -330) != TRAJ_OK)
	{
		usleep(500000);
	}
	robot.svgPrintPosition();

	robot.logger().info() << "on prend le plot 2" << logs::end;
	robot.actions().nose_down(150);
	usleep(500000);
	robot.actions().arm_retract(500);
	robot.actions().arm_back_cylinder(500);
	usleep(500000);

	robot.logger().info() << "on recule" << logs::end;
	robot.asserv().doLineAbs(-80);

	robot.logger().info() << "on va le deposer au centre" << logs::end;
	while (robot.asserv().doMoveForwardAndRotateTo(x_depose_milieu, y_depose_milieu - 50, 90)
			!= TRAJ_OK)
	{
		usleep(300000);
	}
	robot.svgPrintPosition();

	robot.logger().info() << "on depose" << logs::end;

	robot.actions().arm_center(150);
	robot.actions().arm_retract(150);

	robot.logger().info() << "mode on pousse" << logs::end;
	robot.actions().arm_pousser_plot(100);

	robot.logger().info() << "on pousse" << logs::end;
	/*
	 while (robot.asserv().doMoveForwardAndRotateTo(x_depose_milieu, y_depose_milieu + 75, 90)
	 != TRAJ_OK)
	 {
	 usleep(300000);
	 }*/
	robot.asserv().doLineAbs(75);
	robot.svgPrintPosition();


	robot.actions().arm_retract(150);

	robot.asserv().doLineAbs(-65);

	//bascule 45 deg
	robot.actions().arm_semi_deploy(150);
	//lache le plot
	robot.actions().nose_up(150);
	robot.actions().arm_retract(150);

	robot.logger().info() << "on pousse, on avance et on recule" << logs::end;
	robot.asserv().doLineAbs(80);
	robot.svgPrintPosition();
	robot.asserv().doLineAbs(-85);
	robot.svgPrintPosition();

	robot.logger().info() << "O_take_tower1 done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool O_take_opposite()
{

	float x_depose_milieu;
	float y_depose_milieu;
	//MATCH
	x_depose_milieu = 1500;
	y_depose_milieu = 900;
	//TABLE DE TEST
	//x_depose_milieu = 1200;
	//y_depose_milieu = 900;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_take_opposite." << logs::end;
	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.logger().info() << "aller zone opposite " << logs::end;
	robot.ia().iAbyZone().goToZone("zone_take_opposite", &path, &zone);

	robot.logger().info() << "do move to zone opposite " << logs::end;
	int tried = 0;
	while (robot.asserv().doMoveForwardTo(zone.x, zone.y) != TRAJ_OK)
	{
		usleep(500000);
		tried++;
		if (tried == 3) return false;
	}
	robot.svgPrintPosition();

	robot.logger().info() << "faire face" << logs::end;
	robot.asserv().doFaceTo(2100, 1400);

	robot.actions().arm_deploy(200);
	robot.logger().info() << "doMoveForwardTo to zone opposite " << logs::end;
	tried = 0;

	while (robot.asserv().doMoveForwardTo(2100, 1400, -330) != TRAJ_OK)
	{
		usleep(500000);
		tried++;
		if (tried == 3) return false;
	}
	robot.svgPrintPosition();

	robot.logger().info() << "on prend le plot adverse" << logs::end;
	robot.actions().nose_down(150);
	usleep(500000);
	robot.actions().arm_retract(500);
	robot.actions().arm_back_cylinder(500);
	usleep(500000);

	robot.actions().arm_center(150);
	robot.actions().arm_retract(150);

	robot.logger().info() << "mode on pousse" << logs::end;
	robot.actions().arm_pousser_plot(100);
	/*
	 robot.logger().info() << "on pousse" << logs::end;
	 while (robot.asserv().doMoveForwardAndRotateTo(x_depose_milieu, y_depose_milieu + 75, 90)
	 != TRAJ_OK)
	 {
	 usleep(300000);
	 }
	 robot.svgPrintPosition();*/

	robot.actions().arm_retract(150);

	//robot.asserv().doLineAbs(-85);

	//bascule 45 deg
	robot.actions().arm_semi_deploy(150);
	//lache le plot
	robot.actions().nose_up(150);
	robot.actions().arm_retract(150);

	/*
	 robot.logger().info() << "on recule" << logs::end;
	 robot.asserv().doLineAbs(-80);



	 //aller deposer


	 robot.logger().info() << "on va le deposer au centre" << logs::end;
	 while (robot.asserv().doMoveForwardAndRotateTo(x_depose_milieu, y_depose_milieu - 50, 90)
	 != TRAJ_OK)
	 {
	 usleep(300000);
	 }
	 robot.svgPrintPosition();

	 robot.logger().info() << "on depose" << logs::end;

	 robot.actions().arm_center(150);
	 robot.actions().arm_retract(150);

	 robot.logger().info() << "mode on pousse" << logs::end;
	 robot.actions().arm_pousser_plot(100);

	 robot.logger().info() << "on pousse" << logs::end;
	 while (robot.asserv().doMoveForwardAndRotateTo(x_depose_milieu, y_depose_milieu + 75, 90)
	 != TRAJ_OK)
	 {
	 usleep(300000);
	 }
	 robot.svgPrintPosition();

	 robot.actions().arm_retract(150);

	 robot.asserv().doLineAbs(-85);

	 //bascule 45 deg
	 robot.actions().arm_semi_deploy(150);
	 //lache le plot
	 robot.actions().nose_up(150);
	 robot.actions().arm_retract(150);

	 robot.logger().info() << "on pousse, on avance et on recule" << logs::end;
	 robot.asserv().doLineAbs(65);
	 robot.svgPrintPosition();
	 robot.asserv().doLineAbs(-65);
	 robot.svgPrintPosition();

	 */

	robot.logger().info() << "O_take_opposite done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

bool O_take_lego()
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
	robot.logger().info() << "start O_take_lego." << logs::end;

	TRAJ_STATE ts = TRAJ_OK;
	RobotPosition path, zone;

	robot.logger().info() << "aller zone lego " << logs::end;
	robot.ia().iAbyZone().goToZone("zone_take_lego", &path, &zone);

	int tried = 0;
	while (robot.asserv().doMoveForwardTo(zone.x, zone.y) != TRAJ_OK)
	{
		usleep(500000);
	}

	robot.actions().arm_deploy(200);

	while (robot.asserv().doMoveForwardTo(500, 1100, -300) != TRAJ_OK)
	{
		usleep(500000);
	}

	robot.logger().info() << "on prend le plot" << logs::end;
	robot.actions().nose_down(150);
	usleep(500000);
	robot.actions().arm_retract(500);
	robot.actions().arm_back_cylinder(500);
	usleep(500000);

	//on se retourne
	while (robot.asserv().doFaceTo(900, 1400) != TRAJ_OK)
	{
		usleep(500000);
	}
	while (robot.asserv().doMoveForwardTo(550, 1100) != TRAJ_OK)
	{
		usleep(500000);
	}
	while (robot.asserv().doMoveForwardTo(900, 1400, -300) != TRAJ_OK)
	{
		usleep(500000);
	}

	//on depose
	robot.actions().arm_center(150);
	robot.actions().arm_retract(150);

	//bascule 45 deg
	robot.actions().arm_semi_deploy(150);
	//lache le plot
	robot.actions().nose_up(150);
	robot.actions().arm_retract(150);

	robot.logger().info() << "on pousse, on avance et on recule" << logs::end;
	robot.asserv().doLineAbs(95);
	robot.svgPrintPosition();
	robot.asserv().doLineAbs(-95);
	robot.svgPrintPosition();

	robot.logger().info() << "O_take_lego done." << logs::end;
	return true; //return true si ok sinon false si interruption
}

void O_State_DecisionMaker::IASetupHomologation()
{
	logger().debug() << "IASetupHomologation" << logs::end;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	robot.ia().iAbyZone().ia_createZone("depart", 0, 710, 100, 100, 710, 100, 180);
	robot.ia().iAbyZone().ia_createZone("zone_take_dbu", 1000, 1000, 300, 200, 1000, 1100, 90); //ne sert pas
	//robot.ia().iAbyZone().ia_createZone("zone_take_opposite", 2000, 1300, 200, 200, 2100, 900, 0);
	robot.ia().iAbyZone().ia_createZone("zone_take_lego", 400, 1000, 200, 200, 900, 1000, 180);

	robot.ia().iAbyZone().ia_setPath("depart", "zone_take_dbu", 1000, 500); //ne sert pas
	//robot.ia().iAbyZone().ia_setPath("depart", "zone_take_opposite", 1000, 500); //ne sert pas
	robot.ia().iAbyZone().ia_setPath("depart", "zone_take_lego", 1000, 500); //ne sert pas

	robot.ia().iAbyZone().ia_setPath("zone_take_dbu", "zone_take_opposite", 1500, 900);
	robot.ia().iAbyZone().ia_setPath("zone_take_dbu", "zone_take_lego", 1500, 900);
	//robot.ia().iAbyZone().ia_setPath("zone_take_opposite", "zone_take_lego", 1500, 900);

	robot.ia().iAbyZone().ia_addAction("take_dbu", &O_take_dbu);
	//robot.ia().iAbyZone().ia_addAction("take_opposite", &O_take_opposite);
	robot.ia().iAbyZone().ia_addAction("take_lego", &O_take_lego);
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

