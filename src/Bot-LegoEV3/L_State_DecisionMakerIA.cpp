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

bool L_push_3() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_3" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true); //TODO uniquement centre avec la balise
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.logger().info() << "L_push_3 : move first point" << logs::end;
    //ts = robot.ia().iAbyPath().whileMoveForwardTo(700, 1620, true, 1000000, 5, 5, true);
    ts = robot.ia().iAbyPath().whileMoveForwardTo(550, 1700, true, 1000000, 5, 5, true);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_3 : move first point  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();

    }
    robot.svgPrintPosition();

    robot.ia().iAbyPath().goToZone("zone_push_3", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 5, 5, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_3 : zone_push_3  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return true; //on oublie cette strat
    }
    robot.svgPrintPosition();

    robot.logger().info() << "L_push_3 : on recul de -80" << logs::end;
    ts = robot.asserv().doLineAbs(-80);
    robot.svgPrintPosition();

    robot.logger().info() << "L_push_3 : on ouvre les bras" << logs::end;
    robot.actions().fork_open();

    robot.logger().info() << "move forward" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(900, 1350, true, 1000000, 10, 10, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "TAKE 3 / move forward  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.svgPrintPosition();

    //desactivation de la zone de prise
    if (robot.getMyColor() == PMXYELLOW) {
        robot.ia().iAbyPath().enable(robot.ia().area_3_start_yellow, 0);
    }
    else {
        robot.ia().iAbyPath().enable(robot.ia().area_3_start_violet, 0);
    }

//    robot.logger().info() << "runMotorRight..." << logs::end;
//    robot.asserv().freeMotion();
//    robot.asserv().runMotorRight(150, 1500); //TODO mettre le if color
//    robot.asserv().runMotorLeft(250, 1500);
//    robot.svgPrintPosition();
//    robot.asserv().assistedHandling();

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.logger().info() << "runMotorRight like..." << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(850, 1100, true, 1000000, 2, 2, false, 0);
    if (robot.tabletest) ts = robot.ia().iAbyPath().whileMoveForwardTo(600, 600 + 300, true, 1000000, 2, 2, false, 0);
    else ts = robot.ia().iAbyPath().whileMoveForwardTo(600, 600, true, 1000000, 2, 2, false, 0);

//    robot.logger().info() << "L_push_3 : goto zone piedestal+200,+200" << logs::end;
    robot.ia().iAbyPath().goToZone("zone_piedestal", &zone);
//    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x+200, zone.y+200, true, 1000000, 10, 10, true, 30);
//    if (ts != TRAJ_FINISHED) {
//        robot.logger().error() << "L_push_3 : piedestal  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
//        robot.asserv().resetEmergencyOnTraj();
//    }
//    robot.svgPrintPosition();

    robot.logger().info() << "L_push_3 : goto zone piedestal" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 10, 10, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_3 : piedestal  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    robot.svgPrintPosition();

    ts = robot.asserv().doCalage(150, 60);
    robot.svgPrintPosition();

    robot.points += 5;

    robot.logger().info() << "L_push_3 : piedestal on recule de -150" << logs::end;
    ts = robot.asserv().doLineAbs(-150);
    robot.svgPrintPosition();

    robot.logger().info() << "L_push_3 : on ferme la pince a moitié" << logs::end;
    robot.actions().fork_open_half();

    robot.logger().info() << "L_push_3 : calage 200" << logs::end;
    ts = robot.asserv().doCalage(200, 70);
    robot.svgPrintPosition();

    robot.points += 10;

    robot.logger().info() << "L_push_3: piedestal on recule de -100" << logs::end;
    ts = robot.asserv().doLineAbs(-100);
    robot.svgPrintPosition();

    robot.logger().info() << "L_push_3 : on ferme la pince" << logs::end;
    robot.actions().fork_close();

    //TODO optimiser pour prendre le trophy si c'est la premiere action

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_take_trophy() {

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_take_trophy" << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.ia().iAbyPath().goToZone("zone_piedestal", &zone);

    ts = robot.ia().iAbyPath().whileMoveBackwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 5, 10, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_take_trophy : zone_push_alea  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();

    robot.logger().info() << "L_take_trophy: piedestal on se cale en arriere" << logs::end;
    ts = robot.asserv().doLineAbs(-50);
    ts = robot.asserv().doCalage(-200, 20);

    // take Trophy
    robot.actions().cube_push_and_take_trophy(4000);

    robot.points += 15; //POINT - 5+10 prise statuette et depose cube

    robot.logger().info() << "L_take_trophy : piedestal on avancede 200" << logs::end;
    ts = robot.asserv().doLineAbs(200);

    robot.trophy_taken = true;

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_push_all_carres() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_all_carres" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
//    if (robot.getMyColor() == PMXYELLOW) {
//        robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);
//    }
//    else {
//        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);
//    }
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.ia().iAbyPath().goToZone("zone_first_carre", &zone);
    int y_carre = zone.y;
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 5, 5, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " L_push_all_carres  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }

    //tabaisse du bras pour lire la resistance
    if (robot.getMyColor() == PMXYELLOW) {
        robot.actions().arm_right_deploy(500);
    }
    else {
        robot.actions().arm_left_deploy(500);
    }

    robot.actions().sensors().RecordADC(true);
    robot.asserv().setLowSpeedForward(true, 33);

    //on tourne au meme point sans le pathfnding
    robot.logger().info() << "L_push_all_carres on tourne au meme point sans le pathfnding" << logs::end;

    ts = robot.asserv().doAbsoluteRotateTo(zone.theta);
        if (ts != TRAJ_FINISHED) {
            ts = robot.asserv().doAbsoluteRotateTo(zone.theta);
            if (ts != TRAJ_FINISHED) {
                ts = robot.asserv().doAbsoluteRotateTo(zone.theta);
            }
        }
    //ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 100000, 5, 5, false, 0);
    robot.logger().info() << "L_push_all_carres on avance de 370" << logs::end;
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x + 370, zone.y, zone.theta, true, 100000, 5, 5, false, 0);

    //lecture / ADC / CODE
    //default =>510 => 0
    //4,7k    =>462 => 99
    //1k      =>340 =>10
    //470     =>246 =>10
    //courcircuit =>0 =>1
    robot.actions().sensors().RecordADC(false);

    //analyse
    int badsquare = 0;
    for (int i = 0; i < 100; i++) {
        robot.logger().info() << "ADC " << i << " " << robot.actions().sensors().tabADC[i] << logs::end;
        if (robot.actions().sensors().tabADC[i] > 90) {
            badsquare++;
            //square_pattern = 1;
            //robot.logger().info() << "L_push_all_carres PATTERN 1" << logs::end;
        }
        else if (robot.actions().sensors().tabADC[i] < 15) {
            //square_pattern = 3;
            //robot.logger().info() << "L_push_all_carres PATTERN 3" << logs::end;
        }
    }

    if (badsquare >= 2) {
        robot.square_pattern = 1;
        robot.logger().info() << "L_push_all_carres PATTERN 1" << logs::end;
    }
    else {
        robot.square_pattern = 3;
        robot.logger().info() << "L_push_all_carres PATTERN 3" << logs::end;
    }

    exit(0);
    /*
     int nb = 5;
     int val[nb];
     int sum = 0;
     for (int i = 0; i < nb; i++) {
     val[i] = robot.actions().sensors().getADC();
     std::this_thread::sleep_for(std::chrono::milliseconds(200));
     }
     for (int i = 0; i < nb; i++) {
     sum += (val[i]);
     robot.logger().error() << " L_push_all_carres v1  val[i]=" << val[i] << logs::end;
     }
     int moy = sum / nb;

     robot.logger().error() << " L_push_all_carres v1  moy=" << moy << logs::end;

     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().arm_right_init(1000);
     }
     else {
     robot.actions().arm_left_init(1000);
     }



     int nbtests = 0;
     while (moy > 500) {
     nbtests++;
     //if (moy > 500) {
     //test de la resistance
     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().arm_right_init(1000);
     }
     else {
     robot.actions().arm_left_init(1000);
     }
     ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 0, 5, 5, false, 0);

     //test de la resistance
     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().arm_right_deploy(2000);
     }
     else {
     robot.actions().arm_left_deploy(2000);
     }
     //on recom
     for (int i = 0; i < nb; i++) {
     val[i] = robot.actions().sensors().getADC();
     std::this_thread::sleep_for(std::chrono::milliseconds(100));
     }
     for (int i = 0; i < nb; i++) {
     sum += (val[i]);
     robot.logger().error() << " L_push_all_carres v2  val[i]=" << val[i] << logs::end;
     }
     moy = sum / nb;

     robot.logger().error() << " L_push_all_carres v2  moy=" << moy << logs::end;

     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().arm_right_init(1000);
     }
     else {
     robot.actions().arm_left_init(1000);
     }
     if (moy < 500 || nbtests > 3) {

     break;
     }
     }
     */
    /*
     //int pattern = 0;
     if (moy > 430 && moy < 500) pattern = 1;
     else {
     pattern = 3;

     //on remonte le bras
     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().arm_right_init(1000);
     }
     else {
     robot.actions().arm_left_init(1000);
     }
     //carre 1
     robot.svgPrintPosition();
     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().square_push_right(1500);
     robot.actions().square_middle_init(1500);
     }
     else {
     robot.actions().square_push_left(1500);
     robot.actions().square_middle_init(1500);
     }

     robot.points += 5;
     }
     */
    //carre 2
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(852, y_carre, zone.theta, true, 0, 5, 5, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " carre 2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    else {
        robot.svgPrintPosition();
        if (robot.getMyColor() == PMXYELLOW) {
            robot.actions().square_push_right(1500);
            robot.actions().square_middle_init(1500);
        }
        else {
            robot.actions().square_push_left(1500);
            robot.actions().square_middle_init(1500);
        }
    }
    robot.points += 5;

    //carre 3
    if (robot.square_pattern == 1) {
        ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1037, y_carre, zone.theta, true, 0, 5, 5, false, 0);
        if (ts != TRAJ_FINISHED) {
            robot.logger().error() << " carre 3  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
            robot.asserv().resetEmergencyOnTraj();
        }
        else {
            robot.svgPrintPosition();
            if (robot.getMyColor() == PMXYELLOW) {
                robot.actions().square_push_right(1500);
                robot.actions().square_middle_init(1500);
            }
            else {
                robot.actions().square_push_left(1500);
                robot.actions().square_middle_init(1500);
            }
        }
        robot.points += 5;
    }

    if (robot.chrono().getElapsedTimeInSec() >= 90) {
        robot.logger().info() << "zone_carre 3 : t>=90 on ne fait pas l'action" << logs::end;

        return true;
    }

    //carré 4
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1222, y_carre, zone.theta, true, 0, 5, 5, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_carre 4  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    else {
        robot.svgPrintPosition();
        if (robot.getMyColor() == PMXYELLOW) {
            robot.actions().square_push_right(1500);
            robot.actions().square_middle_init(1500);
        }
        else {
            robot.actions().square_push_left(1500);
            robot.actions().square_middle_init(1500);
        }
        robot.points += 5;
    }

    //carre 5
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1407, y_carre, zone.theta, true, 0, 5, 5, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_carre 5  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    else {
        robot.svgPrintPosition();
        if (robot.getMyColor() == PMXYELLOW) {
            robot.actions().square_push_right(1500);
            robot.actions().square_middle_init(1500);
        }
        else {
            robot.actions().square_push_left(1500);
            robot.actions().square_middle_init(1500);
        }
    }
    /*
     //carre 6
     ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1407 + 185, y_carre, true, 1000000, 5, 5, false, 0);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " zone_carre  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     else {
     robot.svgPrintPosition();
     if (robot.getMyColor() == PMXYELLOW) {
     robot.actions().square_push_right(1500);
     robot.actions().square_middle_init(1500);
     }
     else {
     robot.actions().square_push_left(1500);
     robot.actions().square_middle_init(1500);
     }
     }
     robot.points += 5;
     robot.points += 5;
     */

    robot.asserv().setLowSpeedForward(false, 100);

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_push_carre() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_carre" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.ia().iAbyPath().goToZone("zone_carre", &zone);

    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 5, 5, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_carre  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();
    if (robot.getMyColor() == PMXYELLOW) {
        robot.actions().square_push_right(1500);
        robot.actions().square_middle_init(1500);
//    robot.actions().square_push_right(1500);
//    robot.actions().square_middle_init(1500);
    }
    else {
        robot.actions().square_push_left(1500);
        robot.actions().square_middle_init(1500);
    }

    robot.points += 5;

    //carré 4
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1222, zone.y, zone.theta, true, 0, 5, 5, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " zone_carre 4  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    else {
        robot.svgPrintPosition();
        if (robot.getMyColor() == PMXYELLOW) {
            robot.actions().square_push_right(1500);
            robot.actions().square_middle_init(1500);
        }
        else {
            robot.actions().square_push_left(1500);
            robot.actions().square_middle_init(1500);
        }
        robot.points += 5;
    }

//    if (robot.trophy_taken) { // TODO a rajouter avec la tache && robot.alea_done) {
//        robot.logger().info() << "L_push_carre : on se degage par l'arriere" << logs::end;
//        ts = robot.asserv().doMoveBackwardTo(600, zone.y + 200);
//
//        robot.svgPrintPosition();
//    }

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

bool L_push_alea() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_push_alea" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    if (robot.chrono().getElapsedTimeInSec() >= 94) {
        robot.logger().info() << "L_push_alea : t>=94 on ne fait pas l'action" << logs::end;
        robot.ia().iAbyPath().goToZone("zone_push_alea", &zone);
        ts = robot.ia().iAbyPath().whileMoveForwardTo(800, zone.y, true, 300000, 2, 2, false, 0);
        robot.points += 20; //TODO check du x y a la fin du programme
        if (ts != TRAJ_FINISHED) {
            robot.logger().error() << "L_push_alea : zone_push_alea  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
            robot.asserv().resetEmergencyOnTraj();
            return true;
        }
        return true;
    }

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.ia().iAbyPath().goToZone("zone_push_alea", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 2, 2, true, 30);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_alea : zone_push_alea  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }

    robot.logger().info() << "L_push_alea : on ouvre les bras" << logs::end;
    robot.actions().fork_open();

    //desactivation de la zone de prise
    if (robot.getMyColor() == PMXYELLOW) {
        robot.ia().iAbyPath().enable(robot.ia().area_alea_yellow, 0);
    }
    else {
        robot.ia().iAbyPath().enable(robot.ia().area_alea_violet, 0);
    }

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

    robot.ia().iAbyPath().goToZone("zone_start", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 5, 10, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_push_alea:  zone_push_alea  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        //return false;
    }
    robot.svgPrintPosition();

    robot.points += 5; //1+1+1 + 1+1

    robot.alea_done = true;

    robot.logger().info() << "L_push_alea : depart on recul de -300" << logs::end;
    ts = robot.asserv().doLineAbs(-100);
    robot.svgPrintPosition();

    robot.logger().info() << "L_push_alea : on ferme la pince" << logs::end;
    robot.actions().fork_close();

    return true; //return true si ok sinon false si interruption
}

bool L_put_trophy() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //cas uniquement si trophy est deja pris
    if (!robot.trophy_taken) return false;

    robot.logger().info() << "start L_put_trophy" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.ia().iAbyPath().goToZone("zone_vitrine", &zone);

    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 5, 10, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_put_trophy zone_vitrine  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();

    robot.logger().info() << "L_put_trophy : on recule de 140mm" << logs::end;
    ts = robot.asserv().doLineAbs(-120);
    robot.svgPrintPosition();

    robot.logger().info() << "L_put_trophy : petit calage pour pousser" << logs::end;
    ts = robot.asserv().doCalage(-200, 20);
    robot.svgPrintPosition();

    robot.logger().info() << "L_put_trophy : on lache le trophy" << logs::end;
    robot.actions().cube_normal_pos(1000);

    robot.points += 20; //POINT - 15 +5 statuette + vitrine allumée

    //std::this_thread::sleep_for(std::chrono::seconds(5));

    robot.logger().info() << "L_put_trophy : on avance de 120 pour se degager" << logs::end;
    ts = robot.asserv().doLineAbs(140);
    robot.svgPrintPosition();

    return true; //return true si ok sinon false si interruption
}

bool L_end_of_match() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.logger().info() << "start L_put_trophy" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    ts = robot.ia().iAbyPath().whileMoveForwardTo(400, 1000, true, 1000000, 5, 10, false, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_end_of_match  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.points += 20;

    //TODO Choisir la meilleur position de fin entre differente position

    robot.svgPrintPosition();
    return true; //return true si ok sinon false si interruption
}

void L_State_DecisionMakerIA::IASetupActivitiesZone() {
    logger().debug() << "IASetupActivitiesZone" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    //definition des zones en zone YELLOW uniquement
    robot.ia().iAbyPath().ia_createZone("zone_start", 0, 1000, 400, 600, 400, 1150, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_alea", 800, 450, 350, 350, 1250, 350, 90 + 45);
    robot.ia().iAbyPath().ia_createZone("zone_piedestal", 0, 0, 350, 350, 400, 400, 45);
    robot.ia().iAbyPath().ia_createZone("zone_vitrine", 0, 1900, 500, 100, 225, 1700, -90);
    robot.ia().iAbyPath().ia_createZone("zone_push_3", 700, 1100, 300, 500, 900, 1550, -90);
    robot.ia().iAbyPath().ia_createZone("zone_carre", 600, 0, 700, 100, 850, 146, 0);
    robot.ia().iAbyPath().ia_createZone("zone_first_carre", 600, 0, 700, 100, 668, 145, 0);

    robot.ia().iAbyPath().ia_addAction("take_trophy", &L_take_trophy);
    robot.ia().iAbyPath().ia_addAction("put_trophy", &L_put_trophy);
    robot.ia().iAbyPath().ia_addAction("push_3", &L_push_3);
    robot.ia().iAbyPath().ia_addAction("push_carre", &L_push_carre);
    //robot.ia().iAbyPath().ia_addAction("push_all_carres", &L_push_all_carres);
    robot.ia().iAbyPath().ia_addAction("push_alea", &L_push_alea);
    robot.ia().iAbyPath().ia_addAction("end_of_match", &L_end_of_match);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void L_State_DecisionMakerIA::IASetupActivitiesZoneTableTest() {
    logger().debug() << "IASetup TableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.tabletest = true;

    //pour le test sur TABLETEST on desactive la zone alea
    robot.ia().iAbyPath().enable(robot.ia().area_alea_yellow, 0);
    robot.ia().iAbyPath().enable(robot.ia().area_alea_violet, 0);

    int decalagetabletest = 390;
    //definition des zones (en zone YELLOW uniquement, c'est dupliqué automatiquement)
    robot.ia().iAbyPath().ia_createZone("zone_start", 0, 1000, 400, 600, 400, 1150, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_alea", 800, 450 + decalagetabletest, 350, 350, 1250, 350 + decalagetabletest, 90 + 45);
    robot.ia().iAbyPath().ia_createZone("zone_piedestal", 0, 0 + decalagetabletest, 350, 350, 400, 400 + decalagetabletest, 45);
    robot.ia().iAbyPath().ia_createZone("zone_vitrine", 0, 1900, 500, 100, 225, 1700, -90);
    robot.ia().iAbyPath().ia_createZone("zone_push_3", 700, 1100, 300, 500, 900, 1550, -90);
    robot.ia().iAbyPath().ia_createZone("zone_carre", 600, 0 + decalagetabletest, 700, 100, 850, 146 + decalagetabletest, 0);
    robot.ia().iAbyPath().ia_createZone("zone_first_carre", 600, 0 + decalagetabletest, 700, 100, 668, 145 + decalagetabletest, 0);

    /* TODO TEST TEMPO a retirer!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     robot.ia().iAbyPath().ia_addAction("take_trophy", &L_take_trophy);
     robot.ia().iAbyPath().ia_addAction("put_trophy", &L_put_trophy);
     robot.ia().iAbyPath().ia_addAction("push_3", &L_push_3);
     */
    //robot.ia().iAbyPath().ia_addAction("push_carre", &L_push_carre);
    robot.ia().iAbyPath().ia_addAction("push_all_carres", &L_push_all_carres);
    robot.ia().iAbyPath().ia_addAction("push_alea", &L_push_alea);
    robot.ia().iAbyPath().ia_addAction("end_of_match", &L_end_of_match);

    logger().debug() << " END IASetup TableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void L_State_DecisionMakerIA::execute() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //wait for init!
    while (!robot.waitForInit()) {
        usleep(50000);
    }

    logger().info() << "Strategy to be applied = " << robot.strategy() << logs::end;

    if (robot.strategy() == "tabletest") {
        IASetupActivitiesZoneTableTest();

        //enable zone if necessary
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    }
    else if (robot.strategy() == "all") {
        IASetupActivitiesZone(); //definit les activities

        //enable zone if necessary
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    }
    else {
        logger().error() << "NO STRATEGY " << robot.strategy() << " FOUND !!! " << logs::end;
    }

    //wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(50000);
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

//___________________________________

/*
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
 if (robot.getMyColor() == PMXYELLOW) {

 //        robot.actions().right_arm_take(0);
 //        robot.actions().conveyorBelt_Left_low(0);
 } else {

 //        robot.actions().left_arm_take(0);
 //        robot.actions().conveyorBelt_Right_low(0);
 }

 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //position vert1
 int pos = 0;
 if (robot.getMyColor() == PMXYELLOW) {
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
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1500, 1);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1500, 1);

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
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1500, 2);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1500, 2);

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
 if (robot.getMyColor() == PMXYELLOW) {
 //robot.asserv().doLineAbs(205);
 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1300, 1);

 } else {
 //robot.asserv().doLineAbs(200);
 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1300, 1);

 }
 robot.svgPrintPosition();

 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().conveyorBelt_Right_low(0);
 } else {
 //        robot.actions().conveyorBelt_Left_low(0);
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
 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().conveyorBelt_Right_low(0);
 } else {
 //        robot.actions().conveyorBelt_Left_low(0);
 }

 robot.asserv().doCalage2(240, 60);
 robot.svgPrintPosition();

 robot.logger().info() << "left_eject_all" << logs::end;

 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().left_eject_all(0);
 //sleep(5); //on laisse le temps d'ejecter
 } else {
 //        robot.actions().right_eject_all(0);
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

 //     ts = robot.asserv().doRelativeRotateBy(10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     ts = robot.asserv().doRelativeRotateBy(-10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     ts = robot.asserv().doRelativeRotateBy(10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     //ts = robot.asserv().doRelativeRotateBy(-10);

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
 //    robot.actions().init_servos();

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
 if (robot.getMyColor() == PMXYELLOW) {

 //        robot.actions().right_arm_take(0);
 //        robot.actions().conveyorBelt_Left_low(0);
 } else {

 //        robot.actions().left_arm_take(0);
 //        robot.actions().conveyorBelt_Right_low(0);
 }

 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //position vert1
 int pos = 0;
 if (robot.getMyColor() == PMXYELLOW) {
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
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1500, 1);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1500, 1);

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
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1500, 2);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1500, 2);

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
 if (robot.getMyColor() == PMXYELLOW) {
 //robot.asserv().doLineAbs(205);
 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1300, 1);

 } else {
 //robot.asserv().doLineAbs(200);
 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1300, 1);

 }
 robot.svgPrintPosition();

 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().conveyorBelt_Right_low(0);
 } else {
 //        robot.actions().conveyorBelt_Left_low(0);
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
 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().conveyorBelt_Right_low(0);
 } else {
 //        robot.actions().conveyorBelt_Left_low(0);
 }

 robot.asserv().doCalage2(240, 60);
 robot.svgPrintPosition();

 robot.logger().info() << "left_eject_all" << logs::end;

 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().left_eject_all(0);
 //sleep(5); //on laisse le temps d'ejecter
 } else {
 //        robot.actions().right_eject_all(0);
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

 //     ts = robot.asserv().doRelativeRotateBy(10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     ts = robot.asserv().doRelativeRotateBy(-10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     ts = robot.asserv().doRelativeRotateBy(10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     //ts = robot.asserv().doRelativeRotateBy(-10);

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
 //    robot.actions().init_servos();

 robot.svgPrintPosition();
 return true; //return true si ok sinon false si interruption
 }

 int nbbalance = 0;
 void depose_balance()
 {
 LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
 TRAJ_STATE ts = TRAJ_OK;

 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().conveyorBelt_Right_low(0);
 } else {
 //        robot.actions().conveyorBelt_Left_low(0);
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
 //    if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().conveyorBelt_Right_low(0);
 //    } else {
 //        robot.actions().conveyorBelt_Left_low(0);
 //    }

 robot.asserv().doLineAbs(50);
 robot.svgPrintPosition();

 robot.logger().info() << "left_eject_all" << logs::end;

 if (robot.getMyColor() == PMXYELLOW) {
 //        robot.actions().left_eject_all(0);
 //sleep(5); //on laisse le temps d'ejecter
 } else {
 //        robot.actions().right_eject_all(0);
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


 //     ts = robot.asserv().doRelativeRotateBy(10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     ts = robot.asserv().doRelativeRotateBy(-10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     ts = robot.asserv().doRelativeRotateBy(10);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << "  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     //ts = robot.asserv().doRelativeRotateBy(-10);


 if (nbbalance == 0)
 robot.points += 32; //3 palets
 else
 robot.points += 16; //2 palets


 nbbalance++;
 //on fait l'inverse
 if (robot.getMyColor() == PMXYELLOW) {

 //        robot.actions().right_eject_all(0);
 } else {

 //        robot.actions().left_eject_all(0);
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

 if (robot.getMyColor() == PMXYELLOW) {
 robot.logger().info() << "doRunPivotRight clothoid like" << logs::end;
 robot.asserv().doRunPivotRight(600, 530, 1500);
 } else {
 robot.logger().info() << "doRunPivotLeft clothoid like" << logs::end;
 robot.asserv().doRunPivotLeft(530, 600, 1500);
 }

 //     ts = robot.asserv().doLineAbs(100);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << " go +100 ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
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

 if (robot.getMyColor() == PMXYELLOW) {
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
 if (robot.getMyColor() == PMXYELLOW) {

 //        robot.actions().right_arm_retract(0);
 //        robot.actions().left_arm_retract(0);
 //        robot.actions().conveyorBelt_Left_low(0);
 } else {

 //        robot.actions().right_arm_retract(0);
 //        robot.actions().left_arm_retract(0);
 //        robot.actions().conveyorBelt_Right_low(0);
 }

 robot.logger().info() << "calage -200" << logs::end;
 //calage
 robot.svgPrintPosition();
 robot.asserv().doCalage2(-220, 50);
 robot.svgPrintPosition();

 if (robot.getMyColor() == PMXYELLOW) {
 robot.logger().info() << "doRunPivotLeft" << logs::end;
 robot.asserv().doRunPivotLeft(-50, 200, 1000);
 } else {
 robot.logger().info() << "doRunPivotLeft" << logs::end;
 robot.asserv().doRunPivotRight(200, -50, 1000);
 }

 //Prise du bleu
 robot.logger().info() << "prendre bleu " << logs::end;
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(2500, 1, true);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(2500, 1, true);

 }
 robot.svgPrintPosition();

 robot.logger().info() << "go to vert" << logs::end;

 //     ts = robot.ia().iAbyPath().whileMoveForwardTo(90, 1885, true, 400000, 2, 2);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << " go to vert  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 ts = robot.asserv().doLineAbs(85);
 if (ts != TRAJ_FINISHED) {
 robot.asserv().resetEmergencyOnTraj();
 }
 robot.svgPrintPosition();
 robot.logger().info() << "prendre vert" << logs::end;
 //prendre vert
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1500, 1);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1500, 1);
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
 if (robot.getMyColor() == PMXYELLOW) {
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

 //     robot.logger().info() << "ARMsssss" << logs::end;
 //     if (robot.getMyColor() == PMXYELLOW) {
 //
 //     robot.actions().right_arm_take(0);
 //     robot.actions().conveyorBelt_Left_low(0);
 //     } else {
 //
 //     robot.actions().left_arm_take(0);
 //     robot.actions().conveyorBelt_Right_low(0);
 //     }

 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //position vert1
 int pos = 0;
 if (robot.getMyColor() == PMXYELLOW) {
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);
 pos = 1435;
 } else {
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);
 pos = 1435;
 }

 //     robot.logger().info() << "go to distrib" << logs::end;
 //     ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 200000, 2, 2);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }


 //     robot.logger().info() << "go to vert1" << logs::end;
 //     ts = robot.ia().iAbyPath().whileMoveForwardTo(500, pos, true, 400000, 2, 2);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //
 //     robot.logger().info() << "prendre vert1" << logs::end;
 //     //prendre vert1
 //     if (robot.getMyColor() == PMXYELLOW) {
 //
 //     robot.logger().info() << "left_prendre_palet" << logs::end;
 //     robot.actions().left_prendre_palet(1500, 1);
 //
 //     } else {
 //
 //     robot.logger().info() << "right_prendre_palet" << logs::end;
 //     robot.actions().right_prendre_palet(1500, 1);
 //
 //     }

 //position bleu
 //f = 0;
 robot.logger().info() << "go to bleu distrib" << logs::end;

 ts = robot.ia().iAbyPath().whileMoveForwardTo(500 + 190, pos, true, 200000, 2, 2);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " go to bleu distrib ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.logger().info() << "prendre bleu distrib" << logs::end;
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //        robot.actions().left_prendre_palet(1600, 1, true);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //        robot.actions().right_prendre_palet(1600, 1, true);

 }
 robot.svgPrintPosition();

 //     //position vert2
 //     robot.logger().info() << "go to vert2" << logs::end;
 //
 //     ts = robot.ia().iAbyPath().whileMoveForwardTo(510 + 200 + 185, pos, true, 400000, 2, 2);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << " go to vert2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //
 //     robot.logger().info() << " prendre vert2" << logs::end;
 //     if (robot.getMyColor() == PMXYELLOW) {
 //     //robot.asserv().doLineAbs(205);
 //     robot.logger().info() << "left_prendre_palet" << logs::end;
 //     robot.actions().left_prendre_palet(1300, 1);
 //
 //     } else {
 //     //robot.asserv().doLineAbs(200);
 //     robot.logger().info() << "right_prendre_palet" << logs::end;
 //     robot.actions().right_prendre_palet(1300, 1);
 //
 //     }
 //     robot.svgPrintPosition();


 depose_balance();

 //2ème passe pour les verts

 robot.logger().info() << "ARMs retracting" << logs::end;
 if (robot.getMyColor() == PMXYELLOW) {

 //        robot.actions().right_arm_retract(0);
 //        robot.actions().left_arm_retract(0);
 //        robot.actions().conveyorBelt_Left_low(0);
 } else {

 //        robot.actions().right_arm_retract(0);
 //        robot.actions().left_arm_retract(0);
 //        robot.actions().conveyorBelt_Right_low(0);
 }

 robot.ia().iAbyPath().goToZone("zone_grand_distributeur", &zone);
 robot.logger().info() << "while zone  L_take_grand_distributeur." << logs::end;
 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, false, 200000, 1, 1);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_grand_distributeur  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 if (robot.getMyColor() == PMXYELLOW) {
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, false);

 } else {
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, true);
 }

 //     robot.logger().info() << "go to distrib" << logs::end;
 //     ts = robot.ia().iAbyPath().whileMoveForwardTo(370, 1410, false, 200000, 2, 2);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error() << " go to distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }

 robot.logger().info() << "go to vert1" << logs::end;
 ts = robot.ia().iAbyPath().whileMoveForwardTo(495, pos, true, 1000000, 4, 4);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " go to vert1  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.logger().info() << "prendre vert1" << logs::end;
 //prendre vert1
 if (robot.getMyColor() == PMXYELLOW) {

 robot.logger().info() << "left_prendre_palet" << logs::end;
 //robot.actions().left_prendre_palet(2100, 1);

 } else {

 robot.logger().info() << "right_prendre_palet" << logs::end;
 //robot.actions().right_prendre_palet(2100, 1);

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
 if (robot.getMyColor() == PMXYELLOW) {
 //robot.asserv().doLineAbs(205);
 robot.logger().info() << "left_prendre_palet" << logs::end;
 //robot.actions().left_prendre_palet(1300, 1);

 } else {
 //robot.asserv().doLineAbs(200);
 robot.logger().info() << "right_prendre_palet" << logs::end;
 //robot.actions().right_prendre_palet(1300, 1);

 }
 robot.svgPrintPosition();

 //depose again
 depose_balance();

 //    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);

 if (robot.getMyColor() == PMXYELLOW) {
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
 //robot.actions().init_servos();

 robot.svgPrintPosition();
 return true; //return true si ok sinon false si interruption
 }
 */
