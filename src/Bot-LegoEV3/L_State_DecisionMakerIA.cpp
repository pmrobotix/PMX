#include "L_State_DecisionMakerIA.hpp"

#include <src/pmr_playground.h>
#include <unistd.h>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

L_State_DecisionMakerIA::L_State_DecisionMakerIA(Robot& robot) :
        robot_(robot)
{
    p_ = NULL; //TODO !!!! utiliser le p_ du ia extended???
}

bool L_push_palet()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_palet." << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.ia().iAbyPath().goToZone("zone_push_palet", &zone);
    ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    if (ts != TRAJ_OK)
        return false;
    robot.svgPrintPosition();
    robot.asserv().doMoveForwardTo(500, 750);
//    robot.asserv().doLineAbs(200);
//    robot.svgPrintPosition();
//    robot.asserv().doLineAbs(-200);
//    robot.svgPrintPosition();
//    robot.asserv().doLineAbs(350);
    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}
bool L_take_grand_distributeur()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_take_grand_distributeur." << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;
    int f = 0;

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.ia().iAbyPath().goToZone("zone_grand_distributeur", &zone);

    while ((ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta)) != TRAJ_OK) {
        robot.svgPrintPosition();

        robot.asserv().displayTS(ts);
        robot.logger().info() << "L_take_grand_distributeur doMoveForwardAndRotateTo trajstate=" << ts << logs::end;

        sleep(3);
        robot.asserv().resetDisplayTS();

    }

    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);

    //position vert1
    int pos = 0;
    if (robot.getMyColor() == PMXVIOLET) {
        pos = 1385;
    } else
        pos = 1445;
    while ((ts = robot.asserv().doMoveForwardTo(520, pos)) != TRAJ_OK) {
        robot.svgPrintPosition();
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " position vert1 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
//            if (f > 2)
//                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " position vert1 ===== COLLISION essai n°" << f << logs::end;
//            if (f >= 1)
//                return false;
            f++;
        }
    }
    robot.svgPrintPosition();

    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet();

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet();

    }

    //position bleu
    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);

    robot.asserv().doLineAbs(200);
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet();

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet();

    }
    robot.svgPrintPosition();
sleep(20);
    //position vert2

    if (robot.getMyColor() == PMXVIOLET) {
        robot.asserv().doLineAbs(205);
        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet();

    } else {
        robot.asserv().doLineAbs(200);
        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet();

    }
    robot.svgPrintPosition();

    //Calage balance
    if (robot.getMyColor() == PMXVIOLET) {
        robot.asserv().doLineAbs(200);
        robot.asserv().doRotateLeft(15);
        robot.asserv().doCalage(220, 5);

        robot.logger().info() << "left_eject_all" << logs::end;
        robot.actions().left_eject_all();
        sleep(2);
        robot.asserv().doLineAbs(-100);
        robot.asserv().doRotateRight(170);

    } else {
        robot.asserv().doLineAbs(210);
        robot.asserv().doRotateRight(15);
        robot.asserv().doCalage(220, 5);
        robot.logger().info() << "right_eject_all" << logs::end;
        robot.actions().right_eject_all();
        sleep(2);
        robot.asserv().doLineAbs(-100);
        robot.asserv().doRotateLeft(170);
    }

    robot.asserv().doMoveForwardTo(700, 1300);

    if (robot.getMyColor() == PMXVIOLET) {
        robot.ia().iAbyPath().enable(robot.ia().area_palet_start_violet, 0);
    } else {
        robot.ia().iAbyPath().enable(robot.ia().area_palet_start_yellow, 0);
    }

    while ((ts = robot.asserv().doMoveForwardTo(200, 700)) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);

        sleep(2);
        robot.asserv().resetDisplayTS();
    }

    while ((ts = robot.asserv().doMoveBackwardTo(600,1200)) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);

        sleep(2);
        robot.asserv().resetDisplayTS();

    }

    return true; //return true si ok sinon false si interruption
}

void L_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().info() << "IASetupActivitiesZone" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().info() << "color = " << robot.getMyColor() << logs::end;

    //definition des zones en zone ORANGE uniquement
    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 700, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_palet", 400, 700, 100, 100, 700, 750, -180);
    if (robot.getMyColor() == PMXVIOLET)
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 300, 1385, 90);
    else
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 300, 1420, 90);

    //robot.ia().iAbyPath().ia_addAction("push_palet", &L_push_palet);
    robot.ia().iAbyPath().ia_addAction("take_grand_distributeur", &L_take_grand_distributeur);

    logger().info() << " END IASetupActivitiesZone" << logs::end;
}

void L_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().info() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().info() << "color = " << robot.getMyColor() << logs::end;
    /*
     //definition des zones en zone ORANGE uniquement
     robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 500, 0);
     robot.ia().iAbyPath().ia_createZone("zone_push_button", 1000, 0, 300, 400, 1020, 250, 90);

     robot.ia().iAbyPath().ia_addAction("push_button", &L_push_button);
     */
    logger().info() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}
/*
 void L_State_DecisionMakerIA::initPlayground()
 {

 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
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
 }*/

void L_State_DecisionMakerIA::execute()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //wait for init!
    while (!robot.waitForInit()) {
        usleep(50000);
        //logger().error() << "waitForInit..." << logs::end;
    }
    logger().debug() << "waitForInit passed !!!!!!!" << logs::end;

    IASetupActivitiesZone(); //definit les activities

    //wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(50000);
    }

    logger().info() << "L_State_DecisionMakerIA executing..." << logs::end;
    //robot.svgPrintPosition();

    //pause
    //sleep(5);

    //robot.actions().sensors().startSensors(); // not used here because replace by wait end of match

    //start
    robot.ia().iAbyPath().ia_start();

    //logger().info() << "O_State_DecisionMakerIA executed" << logs::end;
    robot.freeMotion();
    //logger().info() << "O_State_DecisionMakerIA freeMotion" << logs::end;
    robot.actions().stop();
    //logger().info() << "O_State_DecisionMakerIA actions().stop" << logs::end;

    robot.svgPrintEndOfFile();
    logger().info() << "L_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}
