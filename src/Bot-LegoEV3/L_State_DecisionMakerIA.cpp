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
    //int f = 0;

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.ia().iAbyPath().goToZone("zone_grand_distributeur", &zone);

    robot.logger().info() << "while zone  L_take_grand_distributeur." << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, false, 1000000, 5, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_grand_distributeur  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
    }
    /*
     f = 0;
     while ((ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();

     robot.logger().info() << "L_take_grand_distributeur doMoveForwardAndRotateTo trajstate=" << ts << logs::end;

     //sleep(3);
     robot.resetDisplayTS();

     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " zone_grand_distributeur  ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     //            if (f > 2)
     //                return false;
     f++;
     usleep(100000);
     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " zone_grand_distributeur  ===== COLLISION essai n°" << f << logs::end;
     //            if (f >= 1)
     //                return false;
     f++;
     usleep(100000);
     }
     robot.displayTS(ts);
     }

     robot.svgPrintPosition();
     */

    robot.logger().info() << "ARMsssss" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {

        robot.actions().right_arm_take(0);
        robot.actions().conveyorBelt_Left_low(0);
    } else {

        robot.actions().left_arm_take(0);
        robot.actions().conveyorBelt_Right_low(0);
    }

    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    //position vert1
    int pos = 0;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);
        pos = 1425;
    } else {
        robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);
        pos = 1425;
    }

    robot.logger().info() << "go to distrib" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }

    robot.logger().info() << "go to vert1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(510, pos, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }
    /*
     f = 0;
     while ((ts = robot.asserv().doMoveForwardTo(510, pos, true)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position vert1 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     //            if (f > 2)
     //                return false;
     f++;
     usleep(100000);
     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position vert1 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 3)
     break;
     f++;
     usleep(100000);
     }
     }
     robot.svgPrintPosition();
     */
    robot.logger().info() << "prendre vert1" << logs::end;
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

    //f = 0;
    robot.logger().info() << "go to bleu" << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200, pos, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }
    //robot.asserv().doLineAbs(190);
    /*
     while ((ts = robot.asserv().doMoveForwardTo(520 + 190, pos, true)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position bleu ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position bleu ===== COLLISION essai n°" << f << logs::end;
     if (f >= 3)
     break;
     f++;
     }
     usleep(100000);
     }*/

    robot.logger().info() << "prendre bleu " << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(1500, 2);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(1500, 2);

    }
    robot.svgPrintPosition();

    //position vert2
    robot.logger().info() << "go to vert2" << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 200, pos, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }

    //f = 0;
    //robot.asserv().doLineAbs(190);
    /*
     while ((ts = robot.asserv().doMoveForwardTo(520 + 190 + 190, pos, true)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position vert2 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position vert2 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 3)
     break;
     f++;
     }

     usleep(100000);
     }*/
    robot.logger().info() << " prendre vert2" << logs::end;
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

    robot.logger().info() << " go to 200" << logs::end;
    //Calage balance

    float temp_x = robot.asserv().pos_getX_mm();
    float temp_y = robot.asserv().pos_getY_mm();
    ts = robot.ia().iAbyPath().whileMoveForwardTo(temp_x + 200, temp_y, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to 200  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.logger().info() << " turn 6 degres" << logs::end;
    robot.asserv().doRelativeRotateBy(8);
    robot.logger().info() << " do calage" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().conveyorBelt_Right_low(0);
    } else {
        robot.actions().conveyorBelt_Left_low(0);
    }

    robot.asserv().doCalage(240, 60);
    robot.svgPrintPosition();

    robot.logger().info() << "left_eject_all" << logs::end;
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
        robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    ts = robot.asserv().doLineAbs(-60);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -60  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().left_eject_all(5000);
        //sleep(5); //on laisse le temps d'ejecter
    } else {
        robot.actions().right_eject_all(5000);
    }

    robot.points += 28;

    ts = robot.asserv().doLineAbs(-120);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -120  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.svgPrintPosition();

    ts = robot.ia().iAbyPath().whileMoveRotateTo(-165, 1000000, 1); //Absolute angle
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to 200  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }

    /*
     f = 0;
     while (robot.asserv().doRelativeRotateBy(-165) != TRAJ_FINISHED) {

     f++;
     robot.logger().info() << "doRotateRight(165) f=" << f << logs::end;
     if (f > 10)
     break;
     usleep(200000);
     }*/
    /*
     //f = 0;
     if (robot.getMyColor() == PMXVIOLET) {

     robot.asserv().doLineAbs(200);

     //         float temp_x = robot.asserv().pos_getX_mm();
     //         float temp_y = robot.asserv().pos_getY_mm();
     //         while ((ts = robot.asserv().doMoveForwardTo(temp_x + 200, temp_y, true)) != TRAJ_FINISHED) {
     //         robot.svgPrintPosition();
     //         if (ts == TRAJ_NEAR_OBSTACLE) {
     //         robot.logger().error() << " position +200 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     //         f++;
     //
     //         }
     //         if (ts == TRAJ_COLLISION) {
     //         robot.logger().error() << " position +200 ===== COLLISION essai n°" << f << logs::end;
     //         if (f >= 3)
     //         break;
     //         f++;
     //         }
     //         usleep(100000);
     //         }
     //        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
     //                robot.logger().info() << " turn 6 degres" << logs::end;
     //                robot.asserv().doRelativeRotateBy(8);
     robot.logger().info() << " do calage" << logs::end;
     robot.actions().conveyorBelt_Right_low(0);
     robot.asserv().doCalage(240, 60);
     robot.svgPrintPosition();

     robot.logger().info() << "left_eject_all" << logs::end;

     robot.asserv().doLineAbs(-60);
     robot.actions().left_eject_all(0);

     //        robot.asserv().doRotateRight(2);
     //         robot.asserv().doRotateLeft(15);
     //         robot.asserv().doRotateRight(2);
     //         robot.asserv().doRotateLeft(5);
     //robot.asserv().doRotateLeft(15);
     sleep(5);

     robot.points += 28;

     robot.asserv().doLineAbs(-120);
     robot.svgPrintPosition();
     //robot.actions().init_servos();
     f = 0;
     while (robot.asserv().doRelativeRotateBy(-165) != TRAJ_FINISHED) {

     f++;
     robot.logger().info() << "doRotateRight(165) f=" << f << logs::end;
     if (f > 10)
     break;
     usleep(200000);
     }

     } else {
     robot.asserv().doLineAbs(200);
     robot.asserv().doRelativeRotateBy(-8);

     robot.actions().conveyorBelt_Left_low(0);

     robot.asserv().doCalage(240, 60);
     robot.svgPrintPosition();

     robot.logger().info() << "left_eject_all" << logs::end;

     robot.asserv().doLineAbs(-60);
     robot.svgPrintPosition();
     robot.actions().right_eject_all(0);
     //sleep(2);
     //        robot.asserv().doRotateLeft(2);
     //         robot.asserv().doRotateRight(15);
     //         robot.asserv().doRotateLeft(2);
     //         robot.asserv().doRotateRight(5);
     //robot.asserv().doRotateRight(15);
     sleep(5);

     robot.points += 28;

     robot.asserv().doLineAbs(-120);
     robot.svgPrintPosition();
     //        robot.actions().init_servos();
     robot.asserv().doRelativeRotateBy(+165);
     robot.svgPrintPosition();

     //        robot.asserv().doLineAbs(210);l
     //        robot.asserv().doRotateRight(15);
     //        robot.asserv().doCalage(220, 4, 70);
     //        robot.logger().info() << "right_eject_all" << logs::end;
     //        robot.actions().right_eject_all(3);
     //        sleep(2);
     //        robot.asserv().doLineAbs(-100);
     //        robot.asserv().doRotateLeft(170);
     }
     */
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.logger().info() << " position move1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(640, 1250, false, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }
    /*
     f = 0;
     while ((ts = robot.asserv().doMoveForwardTo(640, 1250)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position move1 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position move1 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;
     }

     if (f >= 1)
     usleep(200000);
     }
     robot.svgPrintPosition();
     //sleep(10);*/

    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);

    robot.logger().info() << " position move2" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(440, 800, false, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }
    /*
     f = 0;
     while ((ts = robot.asserv().doMoveForwardTo(440, 800)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position move2 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position move2 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;
     }

     if (f >= 1)
     usleep(200000);
     }
     robot.svgPrintPosition();*/

    robot.logger().info() << " position -90" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveRotateTo(-90.0, 1000000, 1);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -90  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
    }

    /*
     f = 0;
     while ((ts = robot.asserv().doAbsoluteRotateTo(-90)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position rouge3 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position rouge3 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;
     }

     if (f >= 1)
     usleep(200000);
     }
     robot.svgPrintPosition();*/

    robot.points += 12;

    robot.actions().sensors().setIgnoreBackNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    ts = robot.asserv().doLineAbs(-600);

    robot.actions().init_servos();

    /*
     robot.logger().info() << " position rouge3" << logs::end;
     f = 0;
     while ((ts = robot.asserv().doMoveForwardTo(1100, pos)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position rouge3 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position rouge3 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;
     }

     if (f >= 1)
     usleep(200000);
     }
     robot.actions().init_servos();

     while ((ts = robot.asserv().doMoveForwardTo(810, pos)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " position rouge3 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;

     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << " position rouge3 ===== COLLISION essai n°" << f << logs::end;
     if (f >= 1)
     break;
     f++;
     }
     usleep(100000);
     }
     robot.asserv().doRotateLeft(170);

     if (robot.getMyColor() == PMXVIOLET) {

     robot.logger().info() << "left_prendre_palet rouge 3" << logs::end;
     robot.actions().left_prendre_palet(1500, 2);

     } else {

     robot.logger().info() << "right_prendre_palet rouge 3" << logs::end;
     robot.actions().right_prendre_palet(1500, 2);

     }
     */
    /*
     robot.asserv().doMoveForwardTo(700, 1300);
     robot.svgPrintPosition();
     if (robot.getMyColor() == PMXVIOLET) {
     robot.ia().iAbyPath().enable(robot.ia().area_palet_start_violet, 0);
     } else {
     robot.ia().iAbyPath().enable(robot.ia().area_palet_start_yellow, 0);
     }

     while ((ts = robot.asserv().doMoveForwardTo(200, 700)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     robot.asserv().displayTS(ts);

     sleep(2);
     robot.asserv().resetDisplayTS();
     }
     robot.svgPrintPosition();
     while ((ts = robot.asserv().doMoveBackwardTo(600, 1200)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     robot.asserv().displayTS(ts);

     sleep(2);
     robot.asserv().resetDisplayTS();

     }*/
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
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 300, 1405, 90);
    else
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 300, 1405, 90);

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
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().addTimerSensors(100);

    //start IA
    robot.ia().iAbyPath().ia_start();

    robot.freeMotion();

    robot.svgPrintEndOfFile();
    logger().info() << "L_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}
