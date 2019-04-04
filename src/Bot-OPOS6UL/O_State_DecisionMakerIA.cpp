#include "O_State_DecisionMakerIA.hpp"

#include <unistd.h>

#include "../Common/Action/LedBar.hpp"
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

void O_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().info() << "IASetupActivitiesZone" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().info() << "color = " << robot.getMyColor() << logs::end;

    //definition des zones en zone ORANGE uniquement
    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 500, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_button", 1000, 0, 300, 400, 1020, 250, 90);
    robot.ia().iAbyPath().ia_createZone("zone_push_cube", 800, 500, 100, 300, 850, 950, -90);
    robot.ia().iAbyPath().ia_createZone("zone_push_bee", 0, 1700, 300, 300, 220, 1700, -90);

    robot.ia().iAbyPath().ia_addAction("push_button", &O_push_button);
    robot.ia().iAbyPath().ia_addAction("push_cube", &O_push_cube);
    robot.ia().iAbyPath().ia_addAction("push_bee", &O_push_bee);

    logger().info() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().info() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().info() << "color = " << robot.getMyColor() << logs::end;

    //definition des zones en zone ORANGE uniquement
    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 500, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_button", 1000, 0, 300, 400, 1020, 250, 90);
    robot.ia().iAbyPath().ia_createZone("zone_push_cube", 800, 500, 100, 300, 850, 950, -90);
    robot.ia().iAbyPath().ia_createZone("zone_push_bee", 0, 1700, 300, 300, 180, 1300, -90); //modifié

    robot.ia().iAbyPath().ia_addAction("push_button", &O_push_button);
    robot.ia().iAbyPath().ia_addAction("push_cube", &O_push_cube);
    robot.ia().iAbyPath().ia_addAction("push_bee", &O_push_bee);

    logger().info() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void O_State_DecisionMakerIA::initPlayground()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().info() << "color = " << robot.getMyColor() << logs::end;

    p_ = new Playground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0);

    //bordure terrain
    p_->add_rectangle(1500, 0, 3000, 140, 0); //bottom
    p_->add_rectangle(1500, 2000, 3000, 140, 0); //top
    p_->add_rectangle(0, 1000, 140, 2000, 0); //left
    p_->add_rectangle(3000, 1000, 140, 2000, 0); //right

    //orange parts
    //cubes
    p_->add_circle(robot.ia().oarea_cube1, 850.0, 530.0, 300.0, 6);
    p_->add_circle(robot.ia().oarea_cube2high, 1100.0, 1500.0, 300.0, 6);
    p_->add_circle(robot.ia().oarea_cube3left, 300.0, 1200.0, 250.0, 6);

    //distributeurs
    p_->add_rectangle(robot.ia().oarea_distribadverse, 600, 1900, 200.0, 200.0, 0);

    //zone de construction
    p_->add_rectangle(robot.ia().oarea_buildzone, 650, 180, 500.0, 220.0, 0);
    if (robot.getMyColor() == PMXVIOLET)
        p_->enable(robot.ia().oarea_buildzone, 0);

    //Green parts
    //cubes
    p_->add_circle(robot.ia().garea_cube1, 2150.0, 530.0, 300.0, 6);
    p_->add_circle(robot.ia().garea_cube2high, 1900.0, 1500.0, 300.0, 6);
    p_->add_circle(robot.ia().garea_cube3left, 2700.0, 1200.0, 250.0, 6);

    //distributeurs
    p_->add_rectangle(robot.ia().garea_distribadverse, 2400, 1900, 200.0, 200.0, 0);

    //zone de construction
    p_->add_rectangle(robot.ia().garea_buildzone, 2350, 180, 500.0, 220.0, 0);
    if (robot.getMyColor() != PMXVIOLET)
        p_->enable(robot.ia().garea_buildzone, 0);

    //stations d'épuration
    p_->add_rectangle(robot.ia().area_stations, 1500.0, 1850.0, 1400.0, 300.0, 0);
    //p_->add_circle(robot.ia().opponent_1, 1200.0, 1200.0, 100.0, 6);

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
        robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    } else {
        IASetupActivitiesZone(); //definit les activities
        initPlayground(); //definit les zones de non droit
        robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);
    }

    //wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(10000);
    }

    logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;
    //robot.svgPrintPosition();

    //robot.actions().sensors().startSensors();
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
