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

L_State_DecisionMakerIA::L_State_DecisionMakerIA(Robot &robot) :
        robot_(robot)
{
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
bool L_take_grand_distributeurOld()
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

    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, false, 1000000, 5, 10);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_grand_distributeur  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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
        pos = 1435;
    }

    robot.logger().info() << "go to distrib" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << "go to vert1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(500, pos, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    //f = 0;
    robot.logger().info() << "go to bleu" << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 190, pos, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to bleu  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 185, pos, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().conveyorBelt_Right_low(0);
    } else {
        robot.actions().conveyorBelt_Left_low(0);
    }

    robot.logger().info() << " go to 200" << logs::end;
    //Calage balance

    //float temp_x = robot.asserv().pos_getX_mm();
    float temp_y = robot.asserv().pos_getY_mm();
    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 200 + 200, temp_y, true, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to 200  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.logger().info() << " turn 6 degres" << logs::end;
    robot.asserv().doRelativeRotateBy(10);
    robot.logger().info() << " do calage" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().conveyorBelt_Right_low(0);
    } else {
        robot.actions().conveyorBelt_Left_low(0);
    }

    robot.asserv().doCalage2(240, 60);
    robot.svgPrintPosition();

    robot.logger().info() << "left_eject_all" << logs::end;

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().left_eject_all(0);
        //sleep(5); //on laisse le temps d'ejecter
    } else {
        robot.actions().right_eject_all(0);
    }

    ts = robot.asserv().doLineAbs(-30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    ts = robot.asserv().doLineAbs(-30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    ts = robot.asserv().doLineAbs(-30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    /*
     ts = robot.asserv().doRelativeRotateBy(10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doRelativeRotateBy(-10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doRelativeRotateBy(10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     //ts = robot.asserv().doRelativeRotateBy(-10);
     */
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
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.logger().info() << " position move1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(640, 1250, false, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);

    robot.logger().info() << " position move2" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(440, 800, false, 1000000, 10, 5);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << " position -90" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveRotateTo(-90.0, 1000000, 1);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -90  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.points += 12;

    robot.actions().sensors().setIgnoreBackNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    ts = robot.asserv().doLineAbs(-600);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.actions().init_servos();

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_take_grand_distributeurOptimised()
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

    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, false, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_grand_distributeur  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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
        pos = 1435;
    }

    robot.logger().info() << "go to distrib" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << "go to vert1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(500, pos, true, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    //f = 0;
    robot.logger().info() << "go to bleu" << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 190, pos, true, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to bleu  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 185, pos, true, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().conveyorBelt_Right_low(0);
    } else {
        robot.actions().conveyorBelt_Left_low(0);
    }

    robot.logger().info() << " go to 200" << logs::end;
    //Calage balance

    //float temp_x = robot.asserv().pos_getX_mm();
    float temp_y = robot.asserv().pos_getY_mm();
    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 200 + 200, temp_y, true, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to 200  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.logger().info() << " turn 6 degres" << logs::end;
    robot.asserv().doRelativeRotateBy(10);
    robot.logger().info() << " do calage" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().conveyorBelt_Right_low(0);
    } else {
        robot.actions().conveyorBelt_Left_low(0);
    }

    robot.asserv().doCalage2(240, 60);
    robot.svgPrintPosition();

    robot.logger().info() << "left_eject_all" << logs::end;

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().left_eject_all(0);
        //sleep(5); //on laisse le temps d'ejecter
    } else {
        robot.actions().right_eject_all(0);
    }

    ts = robot.asserv().doLineAbs(-30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    ts = robot.asserv().doLineAbs(-30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    ts = robot.asserv().doLineAbs(-30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    /*
     ts = robot.asserv().doRelativeRotateBy(10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doRelativeRotateBy(-10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doRelativeRotateBy(10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     //ts = robot.asserv().doRelativeRotateBy(-10);
     */
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
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.logger().info() << " position move1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(640, 1250, false, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);

    robot.logger().info() << " position move2" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(440, 800, false, 400000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << " position -90" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveRotateTo(-90.0, 1000000, 1);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -90  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.points += 12;

    robot.actions().sensors().setIgnoreBackNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    ts = robot.asserv().doLineAbs(-600);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.actions().init_servos();

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

int nbbalance = 0;
void depose_balance()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    TRAJ_STATE ts = TRAJ_OK;

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().conveyorBelt_Right_low(0);
    } else {
        robot.actions().conveyorBelt_Left_low(0);
    }

    robot.logger().info() << " go to 200" << logs::end;
    //Calage balance

    //float temp_x = robot.asserv().pos_getX_mm();
    float temp_y = robot.asserv().pos_getY_mm();
    ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 200 + 200 + 80, temp_y, true, 1000000, 8, 1);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to 200  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.logger().info() << " turn 15 degres" << logs::end;
    robot.asserv().doRelativeRotateBy(15);
    robot.logger().info() << " do calage" << logs::end;
//    if (robot.getMyColor() == PMXVIOLET) {
//        robot.actions().conveyorBelt_Right_low(0);
//    } else {
//        robot.actions().conveyorBelt_Left_low(0);
//    }

    robot.asserv().doLineAbs(50);
    robot.svgPrintPosition();

    robot.logger().info() << "left_eject_all" << logs::end;

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().left_eject_all(0);
        //sleep(5); //on laisse le temps d'ejecter
    } else {
        robot.actions().right_eject_all(0);
    }

    int dist = 60 + nbbalance * 50;
    robot.asserv().doCalage2(dist, 40);

    robot.svgPrintPosition();

    usleep(1500000);

//    ts = robot.asserv().doLineAbs(-30);
//    if (ts != TRAJ_FINISHED) {
//        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
//        robot.asserv().resetEmergencyOnTraj();
//    }
//    ts = robot.asserv().doLineAbs(-30);
//    if (ts != TRAJ_FINISHED) {
//        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
//        robot.asserv().resetEmergencyOnTraj();
//    }
//    ts = robot.asserv().doLineAbs(-30);
//    if (ts != TRAJ_FINISHED) {
//        robot.logger().error() << " go to -30  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
//        robot.asserv().resetEmergencyOnTraj();
//    }

    /*
     ts = robot.asserv().doRelativeRotateBy(10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doRelativeRotateBy(-10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doRelativeRotateBy(10);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     //ts = robot.asserv().doRelativeRotateBy(-10);
     */

    if (nbbalance == 0)
        robot.points += 32; //3 palets
    else
        robot.points += 16; //2 palets


    nbbalance++;
    //on fait l'inverse
    if (robot.getMyColor() == PMXVIOLET) {

        robot.actions().right_eject_all(0);
    } else {

        robot.actions().left_eject_all(0);
    }

    ts = robot.asserv().doLineAbs(-40);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -100  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.svgPrintPosition();

    ts = robot.ia().iAbyPath().whileMoveRotateTo(-160, 200000, 1); //Absolute angle
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -165deg  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    if (robot.getMyColor() == PMXVIOLET) {
        robot.logger().info() << "doRunPivotRight clothoid like" << logs::end;
        robot.asserv().doRunPivotRight(600, 530, 1500);
    } else {
        robot.logger().info() << "doRunPivotLeft clothoid like" << logs::end;
        robot.asserv().doRunPivotLeft(530, 600, 1500);
    }
    /*
     ts = robot.asserv().doLineAbs(100);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " go +100 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }*/
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

    robot.logger().info() << "while zone  small distrib." << logs::end;
    //ts = robot.ia().iAbyPath().whileMoveBackwardAndRotateTo(200, 1950, -90, false, 200000, 2, 2);
    ts = robot.ia().iAbyPath().whileMoveBackwardTo(200, 1960, false, 200000, 0, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "go small distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    //robot.asserv().doCalage2(-100, 40);
    robot.svgPrintPosition();

    if (robot.getMyColor() == PMXVIOLET) {
        ts = robot.asserv().doLineAbs(94);

    } else {
        ts = robot.asserv().doLineAbs(97);
    }

    if (ts != TRAJ_FINISHED) {
        robot.asserv().resetEmergencyOnTraj();
    }

    ts = robot.asserv().doAbsoluteRotateTo(0);
    if (ts != TRAJ_FINISHED) {
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.svgPrintPosition();

    robot.logger().info() << "ARMs retracting" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {

        robot.actions().right_arm_retract(0);
        robot.actions().left_arm_retract(0);
        robot.actions().conveyorBelt_Left_low(0);
    } else {

        robot.actions().right_arm_retract(0);
        robot.actions().left_arm_retract(0);
        robot.actions().conveyorBelt_Right_low(0);
    }

    robot.logger().info() << "calage -200" << logs::end;
    //calage
    robot.svgPrintPosition();
    robot.asserv().doCalage2(-220, 50);
    robot.svgPrintPosition();

    if (robot.getMyColor() == PMXVIOLET) {
        robot.logger().info() << "doRunPivotLeft" << logs::end;
        robot.asserv().doRunPivotLeft(-50, 200, 1000);
    } else {
        robot.logger().info() << "doRunPivotLeft" << logs::end;
        robot.asserv().doRunPivotRight(200, -50, 1000);
    }

    //Prise du bleu
    robot.logger().info() << "prendre bleu " << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(2500, 1, true);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(2500, 1, true);

    }
    robot.svgPrintPosition();

    robot.logger().info() << "go to vert" << logs::end;
    /*
     ts = robot.ia().iAbyPath().whileMoveForwardTo(90, 1885, true, 400000, 2, 2);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " go to vert  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }*/
    ts = robot.asserv().doLineAbs(85);
    if (ts != TRAJ_FINISHED) {
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.svgPrintPosition();
    robot.logger().info() << "prendre vert" << logs::end;
    //prendre vert
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(1500, 1);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(1500, 1);
    }
    //degagement
    //robot.asserv().doRelativeRotateBy(-3);
    //robot.asserv().doLineAbs(20);
//    robot.asserv().doRelativeRotateBy(-5);
//    robot.asserv().doLineAbs(30);
    //robot.asserv().doRelativeRotateBy(-7);
    //robot.asserv().doLineAbs(40);
    robot.logger().info() << "doRelativeRotateBy(-90)" << logs::end;
    ts = robot.asserv().doRelativeRotateBy(-90);

    int plus = 0;
    if (robot.getMyColor() == PMXVIOLET) {
        plus = 0;
    } else {
        plus = 10;
    }

    robot.ia().iAbyPath().goToZone("zone_grand_distributeur", &zone);
    robot.logger().info() << "while zone  L_take_grand_distributeur." << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y - plus, 0, false, 200000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_grand_distributeur  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    /*
     robot.logger().info() << "ARMsssss" << logs::end;
     if (robot.getMyColor() == PMXVIOLET) {

     robot.actions().right_arm_take(0);
     robot.actions().conveyorBelt_Left_low(0);
     } else {

     robot.actions().left_arm_take(0);
     robot.actions().conveyorBelt_Right_low(0);
     }
     */
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    //position vert1
    int pos = 0;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);
        pos = 1435;
    } else {
        robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);
        pos = 1435;
    }
    /*
     robot.logger().info() << "go to distrib" << logs::end;
     ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 200000, 2, 2);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     */
    /*
     robot.logger().info() << "go to vert1" << logs::end;
     ts = robot.ia().iAbyPath().whileMoveForwardTo(500, pos, true, 400000, 2, 2);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }

     robot.logger().info() << "prendre vert1" << logs::end;
     //prendre vert1
     if (robot.getMyColor() == PMXVIOLET) {

     robot.logger().info() << "left_prendre_palet" << logs::end;
     robot.actions().left_prendre_palet(1500, 1);

     } else {

     robot.logger().info() << "right_prendre_palet" << logs::end;
     robot.actions().right_prendre_palet(1500, 1);

     }
     */
    //position bleu
    //f = 0;
    robot.logger().info() << "go to bleu distrib" << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(500 + 190, pos, true, 200000, 2, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to bleu distrib ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << "prendre bleu distrib" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(1600, 1, true);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(1600, 1, true);

    }
    robot.svgPrintPosition();
    /*
     //position vert2
     robot.logger().info() << "go to vert2" << logs::end;

     ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 185, pos, true, 400000, 2, 2);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " go to vert2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }

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
     */

    depose_balance();

    //2ème passe pour les verts

    robot.logger().info() << "ARMs retracting" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {

        robot.actions().right_arm_retract(0);
        robot.actions().left_arm_retract(0);
        robot.actions().conveyorBelt_Left_low(0);
    } else {

        robot.actions().right_arm_retract(0);
        robot.actions().left_arm_retract(0);
        robot.actions().conveyorBelt_Right_low(0);
    }

    robot.ia().iAbyPath().goToZone("zone_grand_distributeur", &zone);
    robot.logger().info() << "while zone  L_take_grand_distributeur." << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, false, 200000, 1, 1);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_grand_distributeur  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
                << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);

    } else {
        robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);
    }
    /*
     robot.logger().info() << "go to distrib" << logs::end;
     ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 200000, 2, 2);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     */
    robot.logger().info() << "go to vert1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(495, pos, true, 1000000, 4, 4);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << "prendre vert1" << logs::end;
    //prendre vert1
    if (robot.getMyColor() == PMXVIOLET) {

        robot.logger().info() << "left_prendre_palet" << logs::end;
        robot.actions().left_prendre_palet(2100, 1);

    } else {

        robot.logger().info() << "right_prendre_palet" << logs::end;
        robot.actions().right_prendre_palet(2100, 1);

    }

    //sleep(3);

    robot.svgPrintPosition();
    //position vert2
    robot.logger().info() << "go to vert2" << logs::end;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(500 + 200 + 200, pos, true, 300000, 6, 25);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to vert2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

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

    //depose again
    depose_balance();

//    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
//    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);

    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);

    } else {
        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);
    }

    robot.logger().info() << " position move1" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(640, 1250, true, 2000000, 6, 6);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    //reprise fin
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);

    robot.logger().info() << " position move2" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(440, 820, false, 200000, 10, 2);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to move2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.logger().info() << " position -90" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveRotateTo(-90.0, 200000, 1);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " go to -90  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }

    robot.points += 12;

    robot.actions().sensors().setIgnoreBackNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    ts = robot.asserv().doLineAbs(-600);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.actions().init_servos();

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
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 290, 1405, 90);
    else
        robot.ia().iAbyPath().ia_createZone("zone_grand_distributeur", 500, 1500, 500, 100, 290, 1405, 90);

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
