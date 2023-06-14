#include "L_State_DecisionMakerIA.hpp"

#include <src/pmr_playground.h>
#include <unistd.h>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

L_State_DecisionMakerIA::L_State_DecisionMakerIA(Robot &robot) :
        robot_(robot)
{
}

bool L_push_cake_A2()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_cake_A2" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.ia().iAbyPath().goToZone("zone_cake_A2", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 55, 55, true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_cake_A2 : zone_cake_A2 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();

    int level = 0;
    //robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    level = robot.actions().sensors().front(false);
    robot.logger().error() << "L_push_cake_A2 :  level=" << level << logs::end;

    if (level >= 4) {

        std::this_thread::sleep_for(std::chrono::seconds(1));
        level = robot.actions().sensors().front(false);
        if (level >= 4) {
            robot.logger().error() << "L_push_cake_A2 : on kill la tache level=" << level << logs::end;
            return true;
        }

    }

    /*
     //on avance
     ts = robot.ia().iAbyPath().whileMoveForwardTo(450, 675, true, 1000000, 5, 5, false);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error()
     << "L_push_cake_A2 : move closed to the 2 cakes ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
     << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     return false;
     }
     robot.svgPrintPosition();
     */
    //on ouvre les pinces
    robot.actions().arm_right_deploy(0);
    robot.actions().arm_left_deploy(0);

    ///on avance doucement
    robot.asserv().setLowSpeedForward(true, 30);
    robot.asserv().doLineAbs(200);
    robot.svgPrintPosition();

    //on ferme les fork pour prendre les cakes
    //robot.actions().fork_front_right_deploy(0);
    //robot.actions().fork_front_left_deploy(0);
    robot.actions().fork_open_take_slow(true);

    robot.asserv().setLowSpeedForward(false);

    //on depose directement
    ts = robot.ia().iAbyPath().whileMoveForwardTo(250, 300, true, 1000000, 30, 30, false);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error()
                << "L_push_cake_A2 : move closed to the 2 cakes ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        // on continue de toute facon //return false;
    }
    robot.svgPrintPosition();

    //on retire les fork doucement
    //robot.actions().fork_front_right_init_slow(0);
    //robot.actions().fork_front_left_init_slow(2000);
    robot.actions().fork_init_slow(true);

    //on recule
    robot.asserv().doLineAbs(-150);
    robot.points += 12;
    //on ferme les pinces
    //robot.actions().init_servos();
    robot.actions().ranger_pinces();

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_push_cake_black_B3()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_cake_black_B3" << logs::end;

    robot.ia().iAbyPath().enable(robot.ia().area_B3, 0);
    robot.ia().iAbyPath().enable(robot.ia().area_B4, 0);
    robot.ia().iAbyPath().enable(robot.ia().area_A3, 0);

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;
    //robot.asserv().setLowSpeedForward(true, 60);

    robot.ia().iAbyPath().goToZone("zone_cake_black_B3", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 30, 30, true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error()
                << "L_push_cake_black_B3 : zone_cake_black_B3 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();

        //return false;

    }
    robot.svgPrintPosition();

    //on ouvre les pinces
    robot.actions().arm_right_deploy(0);
    robot.actions().arm_left_deploy(0);

    //robot.asserv().setLowSpeedForward(true, 50);
    //on pousse
    ts = robot.ia().iAbyPath().whileMoveForwardTo(800, 1850, true, 1000000, 30, 30, false);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_cake_black_B3 : 800, 1850 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();

    }
    robot.svgPrintPosition();

    //decision
    if (robot.B3_is_taken && robot.B4_is_taken) {
        return true; //on passe la tache
    }

    //on abaisse le gauche en vert
    if (robot.getMyColor() == PMXGREEN) {
        robot.actions().fork_front_left_deploy(1000);
    } else {
        robot.actions().fork_front_right_deploy(1000);
    }

    //robot.asserv().setLowSpeedForward(true, 50);
    //on pousse
    //800,1350 800, 1850
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(700, 2750, 90, true, 1000000, 30, 30, false);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_cake_black_B3 : 700, 2750, 90 ===== PB COLLISION FINALE - Que fait-on? ts="
                << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    //on abaisse le droite en vert
    if (robot.getMyColor() == PMXGREEN) {
        robot.actions().fork_front_right_deploy(0);
    } else {
        robot.actions().fork_front_left_deploy(0);
    }
    //std::this_thread::sleep_for(std::chrono::seconds(2));

    //on releve les fork
    robot.actions().fork_init_slow(true);

    //on recule
    robot.asserv().doLineAbs(-100);

    if (!robot.B3_is_taken) {
        robot.logger().error() << "robot.B3_is_taken=" << robot.B3_is_taken << logs::end;
        robot.points += 3;
    }
    if (!robot.B4_is_taken) {
        robot.points += 3;

        //on ferme les pinces
        robot.actions().ranger_pinces();

        robot.svgPrintPosition();
        return true; //return true si ok sinon false si interruption
    }

    //TODO return ?
}

bool L_push_cake_A5()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_cake_A5" << logs::end;

    if (robot.A5_is_taken) {
        return true;
    }

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    //robot.asserv().setLowSpeedForward(true, 50);

    robot.ia().iAbyPath().goToZone("zone_cake_A5", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 4, 4, true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_cake_A5 : zone_cake_A5 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return true;
        //force end of match
//        robot.force_end_of_match = true;
//        return true;

    }
    robot.svgPrintPosition();

    //on ouvre les pinces
    robot.actions().arm_right_deploy(0);
    robot.actions().arm_left_deploy(0);

    ///on avance doucement
    robot.asserv().setLowSpeedForward(true, 40);
    robot.asserv().doLineAbs(380); //on attend un calage
    robot.svgPrintPosition();



    //on ferme les fork pour prendre les cakes
    //robot.actions().fork_front_right_deploy(0);
    //robot.actions().fork_front_left_deploy(0);
    robot.actions().fork_open_take_slow(true);

    robot.asserv().doLineAbs(-80);
            robot.svgPrintPosition();

    robot.asserv().setLowSpeedForward(false);

    //on depose directement
    ts = robot.ia().iAbyPath().whileMoveForwardTo(250, zone.y - 375 - 100, true, 1000000, 10, 3, false);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error()
                << "L_push_cake_A5 : move closed to the 2 cakes ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();

        //force end of match
        //robot.force_end_of_match = true;
    }
//    robot.svgPrintPosition();
//    robot.asserv().doLineAbs(-120);
//    robot.svgPrintPosition();

    //on retire les fork doucement
//    robot.actions().fork_front_right_init_slow(0);
//    robot.actions().fork_front_left_init_slow(2000);
    robot.actions().fork_init_slow(true);

    //on recule
    robot.asserv().doLineAbs(-150);
    robot.svgPrintPosition();

    robot.points += 12;

    //on ferme les pinces
    robot.actions().init_servos();

    robot.svgPrintPosition();
    return true;        //return true si ok sinon false si interruption
}

bool L_push_cake_D5()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_cake_D5" << logs::end;

    //robot.ia().iAbyPath().enable(robot.ia().area_B3, 0);
    robot.ia().iAbyPath().enable(robot.ia().area_C4, 0);

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    //robot.asserv().setLowSpeedForward(true, 40);

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.ia().iAbyPath().goToZone("zone_cake_D5", &zone);

    //PATCH 50 en vert!!!!!!!!
    int y_patch = 0;
//    if (robot.getMyColor() == PMXGREEN) {
//        y_patch = 50;
//    }

    if (robot.D5_is_taken) {
        return true;
    }

    //robot.asserv().setLowSpeedForward(true, 40);

    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y + y_patch, zone.theta, true, 1000000, 5, 2,
            true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_cake_D5 : zone_cake_D5 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        //robot.asserv().setLowSpeedForward(true, 40);

        if (robot.D5_is_taken) {
            return true;
        }
        //force end of match
        robot.force_end_of_match = true;
        return true;

    }
    robot.svgPrintPosition();

    //on ouvre les pinces
    robot.actions().arm_right_deploy(0);
    robot.actions().arm_left_deploy(0);

    ///on avance doucement
    robot.asserv().setLowSpeedForward(true, 40);
    robot.asserv().doLineAbs(360);        //calcage
    robot.svgPrintPosition();



    //on ferme les fork pour prendre les cakes
    //robot.actions().fork_front_right_deploy(0);
    //robot.actions().fork_front_left_deploy(0);
    robot.actions().fork_open_take_slow(true);

    robot.asserv().doLineAbs(-80);
       robot.svgPrintPosition();

//    robot.asserv().setLowSpeedForward(false);
//    robot.asserv().setLowSpeedForward(true, 40);

    //on depose directement
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x + 250, zone.y + 375, true, 1000000, 10, 2, false);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error()
                << "L_push_cake_D5 : move closed to the 2 cakes ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        //force end of match
//        robot.force_end_of_match = true;

        //robot.asserv().doLineAbs(-150);
        //robot.svgPrintPosition();

    }
    robot.svgPrintPosition();



    //on retire les fork doucement
//    robot.actions().fork_front_right_init_slow(0);
//    robot.actions().fork_front_left_init_slow(2000);
    robot.actions().fork_init_slow(true);

    //on recule
    robot.asserv().doLineAbs(-150);
    robot.svgPrintPosition();

    robot.points += 12;
    //desactiver la zone B4
    robot.ia().iAbyPath().enable(robot.ia().area_B4, 0);

    //on ferme les pinces
    robot.actions().init_servos();

    robot.svgPrintPosition();
    return true;        //return true si ok sinon false si interruption
}

bool L_end_of_match()
{

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
//    if (!robot.force_end_of_match) {
//        if (robot.chrono().getElapsedTimeInSec() < 48)
//        {
//            usleep(1000000);
//            return false;
//        }
//    }
    robot.logger().info() << "start L_end_of_match" << logs::end;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    //robot.asserv().setLowSpeedForward(true, 40);

    //TODO gérer le gros robot

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;
    robot.ia().iAbyPath().goToZone("zone_end", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 55, 55, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_end_of_match : zone_end ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        robot.asserv().setLowSpeedForward(true, 40);
        return false;
    }
    robot.svgPrintPosition();
    robot.points += 15;

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

void L_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().debug() << "IASetupActivitiesZone" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    logger().debug() << "color = " << robot.getMyColor() << logs::end;

//definition des zones en zone VERT uniquement
    //robot.ia().iAbyPath().ia_createZone("zone_end", 0, 1650, 450, 450, 500, 2100, 0);
    robot.ia().iAbyPath().ia_createZone("zone_end", 1550, 900, 450, 450, 1550, 1300, 0);
    robot.ia().iAbyPath().ia_createZone("zone_cake_A2", 0, 450, 450, 450, 500, 675, -180);
    robot.ia().iAbyPath().ia_createZone("zone_cake_D5", 1600, 2100, 450, 450, 1500, 2325, 0);
    robot.ia().iAbyPath().ia_createZone("zone_cake_A5", 0, 2100, 450, 450, 500, 2325, -180);
    robot.ia().iAbyPath().ia_createZone("zone_cake_black_B3", 600, 1000, 200, 200, 570, 950, 60);

    robot.ia().iAbyPath().ia_addAction("push_cake_A2", &L_push_cake_A2);
    robot.ia().iAbyPath().ia_addAction("push_cake_black_B3", &L_push_cake_black_B3);
    robot.ia().iAbyPath().ia_addAction("push_cake_A5", &L_push_cake_A5);
    robot.ia().iAbyPath().ia_addAction("push_cake_D5", &L_push_cake_D5);
    robot.ia().iAbyPath().ia_addAction("end_of_match", &L_end_of_match);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void L_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().debug() << "IASetup TableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.tabletest = true;

    int decalagetabletest = 390;
//definition des zones (en zone VERT uniquement, c'est dupliqué automatiquement)
    robot.ia().iAbyPath().ia_createZone("zone_end", 1550, 900, 450, 450, 1050, 1000, 0);
    robot.ia().iAbyPath().ia_createZone("zone_cake_A2", 0, 450, 450, 450, 500, 675, -180);
    robot.ia().iAbyPath().ia_createZone("zone_cake_D5", 1600, 2100, 450, 450, 1500, 2325, 0);
    robot.ia().iAbyPath().ia_createZone("zone_cake_A5", 0, 2100, 450, 450, 500, 2325, -180);
    robot.ia().iAbyPath().ia_createZone("zone_cake_black_B3", 600, 1000, 200, 200, 570, 950, 60);

//       robot.ia().iAbyPath().ia_addAction("push_cake_A2", &L_push_cake_A2);
//       robot.ia().iAbyPath().ia_addAction("push_cake_black_B3", &L_push_cake_black_B3);
//       robot.ia().iAbyPath().ia_addAction("push_cake_A5", &L_push_cake_A5);
//       robot.ia().iAbyPath().ia_addAction("push_cake_D5", &L_push_cake_D5);
    robot.ia().iAbyPath().ia_addAction("end_of_match", &L_end_of_match);

    logger().debug() << " END IASetup TableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void L_State_DecisionMakerIA::execute()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

//wait for init!
    while (!robot.waitForInit()) {
        //usleep(50000);
        std::this_thread::sleep_for(std::chrono::microseconds(50000));
    }

    logger().debug() << "Strategy to be applied = " << robot.strategy() << logs::end;

    if (robot.strategy() == "tabletest") {
        IASetupActivitiesZoneTableTest();

        //enable zone if necessary
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    } else if (robot.strategy() == "all") {
        IASetupActivitiesZone(); //definit les activities

        //enable zone if necessary
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    } else {
        logger().error() << "NO STRATEGY " << robot.strategy() << " FOUND !!! " << logs::end;
    }

//wait for the start of the chrono !
    while (!robot.chrono().started()) {
        std::this_thread::sleep_for(std::chrono::microseconds(5000));
        //usleep(50000);
    }

    logger().info() << "L_State_DecisionMakerIA executing..." << logs::end;

//On ajoute le timer de detection
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    //robot.actions().sensors().addTimerSensors(200); //NORMALEMENT DEJA FAIT DANS L'INIT
    if (!robot.actions().findPTimer("Sensors")) {
        logger().error() << " WARNINGGGGGGGGGG !! PT Sensors doesn't exist! " << logs::end;
    } else
        logger().info() << "GOOD - PT Sensors already exists! " << logs::end;

    //robot.points += 4; //POINT - depose statuette + vitrine

//start IA
    robot.ia().iAbyPath().ia_start();

//End IA
    robot.freeMotion();
    robot.svgPrintEndOfFile();
    logger().info() << "L_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;
}

