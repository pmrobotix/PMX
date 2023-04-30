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
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 5, 5, true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_cake_A2 : zone_cake_A2 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_end_of_match()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_end_of_match" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;
    robot.ia().iAbyPath().goToZone("zone_end", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 5, 5, true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_end_of_match : zone_end ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();
    robot.points += 20;


    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

void L_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().debug() << "IASetupActivitiesZone" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    logger().debug() << "color = " << robot.getMyColor() << logs::end;

//definition des zones en zone VERT uniquement
    robot.ia().iAbyPath().ia_createZone("zone_end", 0, 1650, 450, 450, 500, 2100, 0);
    robot.ia().iAbyPath().ia_createZone("zone_cake_A2", 0, 450, 450, 450, 600, 675, -180);

    robot.ia().iAbyPath().ia_addAction("push_cake_A2", &L_push_cake_A2);
    robot.ia().iAbyPath().ia_addAction("end_of_match", &L_end_of_match);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}


void L_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().debug() << "IASetup TableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.tabletest = true;

//pour le test sur TABLETEST on desactive la zone alea
    //robot.ia().iAbyPath().enable(robot.ia().area_alea_yellow, 0);
    //robot.ia().iAbyPath().enable(robot.ia().area_alea_violet, 0);

    int decalagetabletest = 390;
//definition des zones (en zone VERT uniquement, c'est dupliqué automatiquement)
    robot.ia().iAbyPath().ia_createZone("zone_start", 0, 1000, 400, 600, 400, 1150, 0);

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

    logger().info() << "Strategy to be applied = " << robot.strategy() << logs::end;

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
        std::this_thread::sleep_for(std::chrono::microseconds(50000));
        //usleep(50000);
    }

    logger().info() << "L_State_DecisionMakerIA executing..." << logs::end;

//On ajoute le timer de detection
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().addTimerSensors(200);

    robot.points += 4; //POINT - depose statuette + vitrine

//start IA
    robot.ia().iAbyPath().ia_start();

    robot.freeMotion();

    robot.svgPrintEndOfFile();
    logger().info() << "L_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}

