#include "O_State_DecisionMakerIA.hpp"

#include <unistd.h>

#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

O_State_DecisionMakerIA::O_State_DecisionMakerIA(Robot& robot) :
        robot_(robot)
{
    p_ = NULL;
}
bool O_fake()
{

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_fake." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    if (robot.todo_fake == 0)
        return true;

    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_fake", &zone);
    ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    robot.svgPrintPosition();
    if (ts != TRAJ_OK) {
        robot.asserv().displayTS(ts);
        return false;
    }

    return true; //return true si ok sinon false si interruption
}

bool O_push_blue()
{

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_blue." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_push_blue", &zone);
    ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    robot.svgPrintPosition();
    robot.asserv().displayTS(ts);
    if (ts != TRAJ_OK) {
        return false;
    }

    robot.logger().info() << "on leve le bras" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().ax12_rightHand();
    } else {
        robot.actions().ax12_leftHand();
    }

    robot.logger().info() << "on pousse le bleu" << logs::end;
    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().doLineAbs(110);
    robot.svgPrintPosition();

    robot.logger().info() << "on retracte le bras" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().ax12_rightHand_retract();
    } else {
        robot.actions().ax12_leftHand_retract();
    }

    robot.points += 20;
    robot.displayPoints(robot.points);

    robot.logger().info() << "on desactive la zone fake" << logs::end;
    //on desactive la zone fake //TODO
    robot.todo_fake = 0;

    robot.logger().info() << "on go au goldenium" << logs::end;
    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    int patchx;
    if (robot.getMyColor() == PMXVIOLET) {
        patchx = 50;
    } else {
        patchx = 0;
    }
    while (robot.asserv().doMoveForwardAndRotateTo(2210.0, 300.0+ patchx, -90.0) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        usleep(2000000);
        robot.asserv().resetDisplayTS();
    }

    robot.svgPrintPosition();

    robot.actions().ax12_left_cil_retract(0);
    robot.actions().ax12_right_cil_retract();

    robot.logger().info() << "on avance pour prendre le goldenium" << logs::end;
    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    ts = robot.asserv().doLineAbs(60);
    robot.asserv().doCalage(50, 2);
    ts = robot.asserv().doLineAbs(-15);

    robot.asserv().displayTS(ts);
    robot.svgPrintPosition();

    robot.logger().info() << "prise du goldenium" << logs::end;
    robot.actions().ax12_left_cil(0);
    robot.actions().ax12_right_cil();
    sleep(1);

    if (!robot.actions().ax12_goldenium_in_cil()) {
        robot.logger().info() << "louuuuuupé !!!! on recommence" << logs::end;
    }
    robot.logger().info() << "on recule un peu" << logs::end;
    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().doLineAbs(-130);
    robot.svgPrintPosition();


    robot.points += 20;
    robot.displayPoints(robot.points);

    robot.logger().info() << "on va deposer" << logs::end;
    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_depose", &zone);
    while (robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        usleep(2000000);

    }
    robot.svgPrintPosition();

    int deg = 25;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.asserv().doRotateLeft(deg);
    } else {
        robot.asserv().doRotateRight(deg);
    }

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    if (robot.getMyColor() == PMXVIOLET) {
        robot.asserv().doMoveForwardAndRotateTo(1320, 1370, 70);
    } else {
        robot.asserv().doMoveForwardAndRotateTo(1330, 1370, 60);

    }

    robot.svgPrintPosition();

    robot.asserv().doCalage(150, 6);
    robot.svgPrintPosition();

    //robot.actions().ax12_left_cil_retract(0);
    //robot.actions().ax12_right_cil_retract();
    robot.actions().ax12_left_cil_release(-1);

    sleep(1);

    //on recule
    robot.asserv().doLineAbs(-150);
    if (robot.getMyColor() == PMXVIOLET) {
        robot.asserv().doRotateLeft(90);
    } else {
        robot.asserv().doRotateRight(90);
    }

    robot.points += 24;
    robot.displayPoints(robot.points);

    return true; //return true si ok sinon false si interruption
}

bool O_push_red()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_red." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.ia().iAbyPath().goToZone("zone_push_red", &zone);
    robot.asserv().resetDisplayTS();
    ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    robot.svgPrintPosition();
    robot.asserv().displayTS(ts);
    if (ts != TRAJ_OK) {

        return false;
    }

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    while (robot.asserv().doLineAbs(-200) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        usleep(2000000);
        robot.actions().lcd2x16().clear();
    }
    robot.svgPrintPosition();
    robot.asserv().displayTS(ts);

    robot.points += 6;
    robot.displayPoints(robot.points);

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    while (robot.asserv().doLineAbs(400) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        usleep(2000000);
    }
    robot.svgPrintPosition();
    robot.asserv().displayTS(ts);

    return true; //return true si ok sinon false si interruption
}

bool O_push_alea()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_alea." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.ia().iAbyPath().goToZone("zone_alea_violet", &zone);
    robot.asserv().resetDisplayTS();
    ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    robot.svgPrintPosition();
    robot.asserv().displayTS(ts);
    if (ts != TRAJ_OK) {

        return false;
    }

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    while (robot.asserv().doMoveForwardTo(300, 700) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        usleep(2000000);

    }
    robot.svgPrintPosition();
    robot.asserv().displayTS(ts);

    robot.points += 7;
    robot.displayPoints(robot.points);

    return true; //return true si ok sinon false si interruption
}

void O_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().info() << "IASetupActivitiesZone definition" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 700, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_red", 400, 400, 100, 100, 700, 450, 0);
    if (robot.getMyColor() == PMXVIOLET) {
        robot.ia().iAbyPath().ia_createZone("zone_push_blue", 1680, 0, 200, 200, 1680, 255, 0);
    } else {
        robot.ia().iAbyPath().ia_createZone("zone_push_blue", 1680, 0, 200, 200, 1680, 220, 0);
    }

    robot.ia().iAbyPath().ia_createZone("zone_fake", 1300, 0, 200, 200, 1300, 900, 0);
    robot.ia().iAbyPath().ia_createZone("zone_depose", 1300, 1400, 200, 200, 1180, 1250, 180);
    robot.ia().iAbyPath().ia_createZone("zone_alea_violet", 1000, 1100, 100, 100, 1300, 1150, -145);

    //robot.ia().iAbyPath().ia_addAction("push_red", &O_push_red);
    robot.ia().iAbyPath().ia_addAction("push_blue", &O_push_blue);
    robot.ia().iAbyPath().ia_addAction("push_alea", &O_push_alea);
    robot.ia().iAbyPath().ia_addAction("fake", &O_fake);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().info() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    logger().debug() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void O_State_DecisionMakerIA::initPlayground()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    p_ = new Playground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0);

    //bordure terrain
    p_->add_rectangle(1500, 0, 3000, 300, 0); //bottom
    p_->add_rectangle(1500, 2000, 3000, 300, 0); //top
    p_->add_rectangle(0, 1000, 300, 2000, 0); //left
    p_->add_rectangle(3000, 1000, 300, 2000, 0); //right

    //zone aleatoire
    p_->add_circle(robot.ia().area_alea_violet, 1000.0, 1050.0, 220.0, 8);
    p_->add_circle(robot.ia().area_alea_yellow, 2000.0, 1050.0, 220.0, 8);

    //zone palets depart
    p_->add_rectangle(robot.ia().area_palet_start_violet, 500.0, 750.0, 300.0, 950.0, 0.0);
    p_->add_rectangle(robot.ia().area_palet_start_yellow, 2500.0, 750.0, 300.0, 950.0, 0.0);

    p_->compute_edges();

    robot.ia().iAbyPath().addPlayground(p_);
    robot.ia().iAbyPath().toSVG();
}

void O_State_DecisionMakerIA::execute()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //wait for init!
    while (!robot.waitForInit()) {
        usleep(1000);
        //logger().error() << "waitForInit..." << logs::end;
    }
    logger().debug() << "waitForInit passed !!!!!!!" << logs::end;

    //____________________________________________________________________________________________________
    int tabletest = 0;
    //____________________________________________________________________________________________________

    if (tabletest == 1) {
        IASetupActivitiesZoneTableTest(); //A COMMENTER pour match
        initPlayground(); //definit les zones de non droit
        //enable zone
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    } else {
        IASetupActivitiesZone(); //definit les activities
        initPlayground(); //definit les zones de non droit
        //enable zone
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);
    }

    //wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(10000);
    }

    logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;
    //robot.svgPrintPosition();

    //On ajoute le timer de detection
    robot.actions().sensors().addTimerSensors();

    robot.ia().iAbyPath().ia_start(); //launch IA

    robot.actions().ledBar().startK2mil(100, 100000, LED_GREEN);
    //TODO danse de fin ?
    //robot.actions().ledBar().k2mil(1, 50000, LED_GREEN); //Ne doit pas trop long...

    //logger().info() << "O_State_DecisionMakerIA executed" << logs::end;
    robot.freeMotion();
    //logger().info() << "O_State_DecisionMakerIA freeMotion" << logs::end;
    robot.actions().stop();
    //logger().info() << "O_State_DecisionMakerIA actions().stop" << logs::end;

    robot.svgPrintEndOfFile();
    logger().info() << "O_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}

/*
 bool O_push_button()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_button." << logs::end;

 robot.svgPrintPosition();

 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 //on avance en vert
 if (robot.getMyColor() == PMXVIOLET) {
 } else {
 robot.asserv().doLineAbs(100);
 }

 robot.asserv().ignoreFrontCollision(false);
 robot.asserv().ignoreRearCollision(true);
 robot.ia().iAbyPath().goToZone("zone_push_button", &zone);
 ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
 if (ts != TRAJ_OK)
 return false;

 robot.svgPrintPosition();

 //on recule un peu
 robot.asserv().doLineAbs(-120);
 robot.svgPrintPosition();

 //recalage
 robot.asserv().doCalage(-80, 3);
 robot.svgPrintPosition();

 robot.asserv().doLineAbs(150);
 robot.svgPrintPosition();

 robot.logger().info() << "O_push_button done." << logs::end;

 robot.points += 25;

 return true; //return true si ok sinon false si interruption
 }

 bool O_push_cube()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_cube." << logs::end;

 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 robot.asserv().ignoreFrontCollision(false);
 robot.asserv().ignoreRearCollision(true);
 robot.ia().iAbyPath().goToZone("zone_push_cube", &zone);
 ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
 if (ts != TRAJ_OK)
 return false;

 if (robot.getMyColor() == PMXVIOLET) {
 robot.ia().iAbyPath().enable(robot.ia().oarea_cube1, 0);
 robot.ia().iAbyPath().enable(robot.ia().oarea_buildzone, 1);
 } else {
 robot.ia().iAbyPath().enable(robot.ia().garea_cube1, 0);
 robot.ia().iAbyPath().enable(robot.ia().garea_buildzone, 1);
 }

 //on avance au point pour deposer
 while (robot.asserv().doMoveForwardTo(850, 400) != TRAJ_OK) {
 usleep(2000000);
 }
 robot.svgPrintPosition();

 //on recule un peu
 robot.asserv().doLineAbs(-100);
 robot.svgPrintPosition();

 robot.points += 5;

 robot.logger().info() << "O_push_cube done." << logs::end;
 return true; //return true si ok sinon false si interruption
 }

 bool O_push_bee()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_bee." << logs::end;

 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 robot.ia().iAbyPath().goToZone("zone_push_bee", &zone);
 ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
 if (ts != TRAJ_OK)
 return false;

 if (robot.getMyColor() == PMXVIOLET) {
 ts = robot.asserv().doLineAbs(-40);
 ts = robot.asserv().doRotateAbs(90);
 } else {
 ts = robot.asserv().doLineAbs(-70);
 ts = robot.asserv().doRotateAbs(-90);
 }

 robot.svgPrintPosition();
 //ts = robot.asserv().doLineAbs(-100);
 robot.asserv().doCalage(-250, 8);

 ts = robot.asserv().doLineAbs(50);
 robot.svgPrintPosition();
 //    robot.actions().arm_left_full(0);
 //    robot.actions().arm_right_full(-1);
 //
 //    robot.actions().arm_left_retract(0);
 //    robot.actions().arm_right_retract(-1);
 //
 //    robot.actions().arm_left_full(0);
 //    robot.actions().arm_right_full(-1);
 //
 //    robot.actions().arm_left_retract(0);
 //    robot.actions().arm_right_retract(-1);

 robot.actions().releaseAll();

 //abeille points
 robot.points += 50;

 ts = robot.asserv().doLineAbs(120);
 robot.svgPrintPosition();

 if (robot.getMyColor() == PMXVIOLET)
 ts = robot.asserv().doRotateRight(45);
 else
 ts = robot.asserv().doRotateLeft(45);

 ts = robot.asserv().doLineAbs(120);
 robot.svgPrintPosition();
 //sleep(3);
 //aller au distributeur

 if (robot.getMyColor() == PMXVIOLET) {
 ts = robot.asserv().doMoveForwardAndRotateTo(200, 1000, -90);
 robot.svgPrintPosition();
 ts = robot.asserv().doLineAbs(90);
 robot.svgPrintPosition();
 } else {
 ts = robot.asserv().doMoveForwardAndRotateTo(210, 1000, -90);
 robot.svgPrintPosition();
 ts = robot.asserv().doLineAbs(70);
 robot.svgPrintPosition();
 }

 ts = robot.asserv().doRotateRight(2);
 ts = robot.asserv().doRotateLeft(4);
 ts = robot.asserv().doRotateRight(2);
 ts = robot.asserv().doRotateLeft(4);
 ts = robot.asserv().doRotateRight(2);
 ts = robot.asserv().doRotateLeft(4);
 ts = robot.asserv().doRotateRight(2);
 ts = robot.asserv().doRotateLeft(4);
 robot.svgPrintPosition();

 ts = robot.asserv().doLineAbs(-150);
 robot.svgPrintPosition();
 //points distributeurs
 robot.points += 10;

 //petite danse
 //robot.actions().servo_init_end();
 ts = robot.asserv().doRotateRight(2);
 ts = robot.asserv().doRotateLeft(4);
 ts = robot.asserv().doRotateRight(2);
 ts = robot.asserv().doRotateLeft(4);
 //robot.actions().servo_init_end();

 robot.logger().info() << "O_push_bee done." << logs::end;
 return true; //return true si ok sinon false si interruption
 }
 */
