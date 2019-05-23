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

//    robot.asserv().setIgnoreFrontNearObstacle(false);
//    robot.asserv().setIgnoreBackNearObstacle(true);
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

//    robot.asserv().setIgnoreFrontNearObstacle(false);
//    robot.asserv().setIgnoreBackNearObstacle(true);
    robot.ia().iAbyPath().goToZone("zone_grand_distributeur", &zone);
    f = 0;
    while ((ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta)) != TRAJ_OK) {
        robot.svgPrintPosition();

        robot.asserv().displayTS(ts);
        robot.logger().info() << "L_take_grand_distributeur doMoveForwardAndRotateTo trajstate=" << ts << logs::end;

        sleep(3);
        robot.asserv().resetDisplayTS();

    }

    robot.svgPrintPosition();

//    robot.asserv().setIgnoreFrontNearObstacle(true);
//    robot.asserv().setIgnoreBackNearObstacle(true);

    //position vert1
    int pos = 0;
    if (robot.getMyColor() == PMXVIOLET) {
        pos = 1420;
    } else
        pos = 1420;
    f = 0;
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

    //prendre vert1
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(1500, 1);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(1500, 1);

    }

    //position bleu
//    robot.asserv().setIgnoreFrontNearObstacle(true);
//    robot.asserv().setIgnoreBackNearObstacle(true);

    //robot.asserv().doLineAbs(190);
    f = 0;
    while ((ts = robot.asserv().doMoveForwardTo(520 + 190, pos)) != TRAJ_OK) {
        robot.svgPrintPosition();
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " position bleu ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            f++;

        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " position bleu ===== COLLISION essai n°" << f << logs::end;
            f++;
        }
    }

    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(1500, 2);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(1500, 2);

    }
    robot.svgPrintPosition();

    //position vert2
    f = 0;
    while ((ts = robot.asserv().doMoveForwardTo(520 + 190 + 205, pos)) != TRAJ_OK) {
        robot.svgPrintPosition();
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " position vert2 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            f++;

        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " position vert2 ===== COLLISION essai n°" << f << logs::end;
            f++;
        }
    }
    if (robot.getMyColor() == PMXVIOLET) {
        //robot.asserv().doLineAbs(205);
        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(1300, 1);

    } else {
        //robot.asserv().doLineAbs(200);
        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(1300, 1);

    }
    robot.svgPrintPosition();

    //Calage balance
    if (robot.getMyColor() == PMXVIOLET) {
        robot.asserv().doLineAbs(200);
        robot.asserv().doRotateLeft(12);
        //robot.asserv().doCalage(220, 4, 70);
        robot.asserv().doCalage(240, 50);
        robot.svgPrintPosition();
        robot.asserv().doLineAbs(-60);
        robot.logger().info() << "left_eject_all" << logs::end;
        robot.actions().left_eject_all(0);
        //sleep(2);
        robot.asserv().doRotateRight(15);
        robot.asserv().doRotateLeft(15);
        robot.asserv().doRotateRight(15);
        robot.asserv().doRotateLeft(15);

        robot.asserv().doLineAbs(-100);
        robot.svgPrintPosition();
        robot.actions().init_servos();
        robot.asserv().doRotateRight(150);

    } else {
        robot.asserv().doLineAbs(200);
        robot.asserv().doRotateRight(12);
        //robot.asserv().doCalage(220, 4, 70);
        robot.asserv().doCalage(240, 50);
        robot.svgPrintPosition();
        robot.asserv().doLineAbs(-60);
        robot.logger().info() << "left_eject_all" << logs::end;
        robot.actions().right_eject_all(0);
        //sleep(2);
        robot.asserv().doRotateLeft(15);
        robot.asserv().doRotateRight(15);
        robot.asserv().doRotateLeft(15);
        robot.asserv().doRotateRight(15);

        robot.asserv().doLineAbs(-100);
        robot.svgPrintPosition();
        robot.actions().init_servos();
        robot.asserv().doRotateLeft(150);

//        robot.asserv().doLineAbs(210);
//        robot.asserv().doRotateRight(15);
//        robot.asserv().doCalage(220, 4, 70);
//        robot.logger().info() << "right_eject_all" << logs::end;
//        robot.actions().right_eject_all(3);
//        sleep(2);
//        robot.asserv().doLineAbs(-100);
//        robot.asserv().doRotateLeft(170);
    }

    robot.asserv().doMoveForwardTo(700, 1300);
    robot.svgPrintPosition();
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
    robot.svgPrintPosition();
    while ((ts = robot.asserv().doMoveBackwardTo(600, 1200)) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);

        sleep(2);
        robot.asserv().resetDisplayTS();

    }
    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

void L_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().debug() << "IASetupActivitiesZone" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    //definition des zones en zone ORANGE uniquement
    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 700, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_palet", 400, 700, 100, 100, 700, 750, -180);
    if (robot.getMyColor() == PMXVIOLET)
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 300, 1420, 90);
    else
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 300, 1417, 90);

    //robot.ia().iAbyPath().ia_addAction("push_palet", &L_push_palet);
    robot.ia().iAbyPath().ia_addAction("take_grand_distributeur", &L_take_grand_distributeur);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void L_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().debug() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;
    /*
     //definition des zones en zone ORANGE uniquement
     robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 500, 0);
     robot.ia().iAbyPath().ia_createZone("zone_push_button", 1000, 0, 300, 400, 1020, 250, 90);

     robot.ia().iAbyPath().ia_addAction("push_button", &L_push_button);
     */
    logger().debug() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void L_State_DecisionMakerIA::execute()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //wait for init!
    while (!robot.waitForInit()) {
        usleep(50000);
    }
    logger().debug() << "waitForInit passed !!!!!!!" << logs::end;

    //TODO depend des strategies
    IASetupActivitiesZone(); //definit les activities

    //wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(10000);
    }

    logger().info() << "L_State_DecisionMakerIA executing..." << logs::end;

    //On ajoute le timer de detection
    //robot.actions().sensors().startSensors();

    //start IA
    robot.ia().iAbyPath().ia_start();

    robot.freeMotion();

    robot.svgPrintEndOfFile();
    logger().info() << "L_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}
