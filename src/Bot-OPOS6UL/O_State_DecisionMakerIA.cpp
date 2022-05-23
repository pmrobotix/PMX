#include "O_State_DecisionMakerIA.hpp"

#include <unistd.h>
#include <vector>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

O_State_DecisionMakerIA::O_State_DecisionMakerIA(Robot &robot) :
        robot_(robot)
{
}

bool O_take_distrib_partage() {
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_take_distrib_partage." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.ia().iAbyPath().goToZone("zone_distrib_partage", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 5, 510, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_take_trophy : zone_distrib_partage  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();

    return true; //return true si ok sinon false si interruption
}

bool O_take_distrib() {
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_take_distrib." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.ia().iAbyPath().goToZone("zone_distrib", &zone);
    ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 5, 510, true, 0);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << "L_take_trophy : zone_distrib  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
        return false;
    }
    robot.svgPrintPosition();

    return true; //return true si ok sinon false si interruption
}

void O_State_DecisionMakerIA::IASetupActivitiesZone() {
    logger().info() << "IASetupActivitiesZone homolagation" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.ia().iAbyPath().ia_createZone("zone_start", 0, 1000, 400, 600, 400, 1150, 0);

    robot.ia().iAbyPath().ia_createZone("zone_distrib_partage", 1200, 1800, 300, 200, 1350, 1750, 90);
    robot.ia().iAbyPath().ia_createZone("zone_distrib", 0, 600, 200, 300, 300, 750, 180);
    robot.ia().iAbyPath().ia_createZone("zone_depose1", 450, 1900, 800, 100, 1100, 1750, 180);

    robot.ia().iAbyPath().ia_addAction("take_distrib_partage", &O_take_distrib_partage);
    robot.ia().iAbyPath().ia_addAction("take_distrib", &O_take_distrib);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::IASetupActivitiesZoneTableTest() {
    logger().error() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.ia().iAbyPath().ia_createZone("zone_start", 0, 1000, 400, 600, 400, 1150, 0);

    robot.ia().iAbyPath().ia_createZone("zone_distrib_partage", 1200, 1800, 300, 200, 1350, 1750, 90);
    robot.ia().iAbyPath().ia_createZone("zone_distrib", 0, 600, 200, 300, 300, 750, 180);
    robot.ia().iAbyPath().ia_createZone("zone_depose1", 450, 1900, 800, 100, 1100, 1750, 180);

    robot.ia().iAbyPath().ia_addAction("take_distrib_partage", &O_take_distrib_partage);
    robot.ia().iAbyPath().ia_addAction("take_distrib", &O_take_distrib);

    logger().debug() << " END IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!" << logs::end;
}

void O_State_DecisionMakerIA::execute() {
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

//wait for init!
    while (!robot.waitForInit()) {
        usleep(1000);
        //logger().error() << "waitForInit..." << logs::end;
    }
//logger().debug() << "waitForInit passed !!!!!!!" << logs::end;

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
        usleep(10000);
    }

    logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;
    /*
     //init rouge
     TRAJ_STATE ts = robot.asserv().doMoveForwardTo(330, 463);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " doMoveForwardTo(330, 463);  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
     << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     ts = robot.asserv().doAbsoluteRotateTo(150);

     robot.points += 6;
     robot.displayPoints();
     ts = robot.asserv().doLineAbs(-100);
     if (ts != TRAJ_FINISHED) {
     robot.logger().error() << " doLineAbs(-100)  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
     robot.asserv().resetEmergencyOnTraj();
     }
     robot.points += 35;
     robot.displayPoints();

     if (robot.getMyColor() == PMXYELLOW) {
     robot.ia().iAbyPath().enable(robot.ia().area_alea_yellow, 0);
     } else {
     robot.ia().iAbyPath().enable(robot.ia().area_alea_violet, 0);
     }*/

    //On ajoute le timer de detection
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().addTimerSensors(200);

    robot.ia().iAbyPath().ia_start();        //launch IA

    //robot.actions().ledBar().startK2mil(100, 100000, LED_GREEN);

    robot.freeMotion();

    robot.svgPrintEndOfFile();
    logger().info() << "O_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}

/*
 * 2021
 bool O_push_exp() {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_exp." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

 robot.ia().iAbyPath().goToZone("zone_exp", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " O_push_exp  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }
 robot.svgPrintPosition();
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.asserv().setLowSpeedForward(true, 35);

 if (robot.getMyColor() == PMXYELLOW) {
 robot.actions().ax12_bras_droit(0);
 robot.actions().ax12_bras_gauche(-1);

 }
 else {
 robot.actions().ax12_bras_gauche(0);
 robot.actions().ax12_bras_droit(-1);
 }

 ts = robot.asserv().doRelativeRotateBy(180);
 //while (ts == robot.asserv().doRelativeRotateBy(45))
 //{
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << "on pousse l'exp  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 //}
 robot.svgPrintPosition();

 robot.points += 15;
 robot.displayPoints();

 //robot.asserv().setLowSpeedForward(true, 35);

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 if (robot.getMyColor() == PMXYELLOW) {
 robot.actions().ax12_bras_gauche_init(-1);
 robot.actions().ax12_bras_droit_init(-1);

 }
 else {
 robot.actions().ax12_bras_droit_init(-1);
 robot.actions().ax12_bras_gauche_init(-1);

 }

 return true; //return true si ok sinon false si interruption
 }

 bool O_push_green() {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_green." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

 robot.ia().iAbyPath().goToZone("zone_green_left", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, false, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_green_left  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }
 robot.svgPrintPosition();

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 //ts = robot.asserv().doLineAbs(220);
 ts = robot.ia().iAbyPath().whileMoveForwardTo(300, 1100, false, 1000000, 2, 2, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " on pousse le vert  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 robot.svgPrintPosition();

 robot.points += 2;
 robot.displayPoints();

 ts = robot.asserv().doLineAbs(-80);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " on recule  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 robot.svgPrintPosition();

 robot.points += 13;
 robot.displayPoints();

 return true; //return true si ok sinon false si interruption
 }

 bool O_push_fanion() {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_fanion." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;
 int xmatch = 0;
 int ymatch = 0;
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

 robot.ia().iAbyPath().goToZone("zone_fanion", &zone);

 //point intermediaire
 robot.logger().info() << " zone_fanion intermediaire" << ts << logs::end;
 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_fanion intermediaire table test/match  ===== PB COLLISION FINALE - on retourne à la next strat ts="
 << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);
 return false;
 }
 xmatch = 170;
 ymatch = 1850;

 //table de test A supp pour match!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 //    xmatch = 270;
 //    ymatch = 1410;
 //
 //    robot.logger().info() << " zone_fanion corner" << ts << logs::end;
 //    ts = robot.ia().iAbyPath().whileMoveForwardTo(xmatch, ymatch, true, 1000000, 5, 5, true);
 //    if (ts != TRAJ_FINISHED) {
 //        robot.logger().info() << " zone_fanion corner   ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 //        robot.asserv().resetEmergencyOnTraj();
 //        robot.svgPrintPosition();
 //        //return false;
 //    }


 //on affiche la position
 RobotPosition p = robot.asserv().pos_getPosition();
 robot.logger().info() << " POSITION x=" << p.x << " y="<< p.y << " theta=" << p.theta << logs::end;




 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

 robot.logger().info() << " zone_fanion corner <<" << xmatch << " " << ymatch << logs::end;
 //ts = robot.asserv().doMoveForwardAndRotateTo(xmatch, ymatch, 0);
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(xmatch, ymatch, 0, true, 1000000, 5, 5, false);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_fanion corner  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);

 }
 robot.svgPrintPosition();
 //Pour table test
 robot.logger().info() << " doRelativeRotateBy(0)" << ts << logs::end;
 ts = robot.asserv().doRelativeRotateBy(0);
 robot.svgPrintPosition();

 if (robot.getMyColor() == PMXYELLOW) {

 robot.actions().ax12_bras_gauche_fanion(-1);
 }
 else {

 robot.actions().ax12_bras_droit_fanion(-1);
 }
 robot.svgPrintPosition();

 //robot.asserv().setLowSpeedForward(true, 25);

 ts = robot.asserv().doMoveForwardAndRotateTo(xmatch + 205, ymatch, 0);
 //ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(xmatch+205, ymatch, 0, true, 1000000, 5, 5, false);
 //ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(390, 1840, 0, true, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_fanion 1  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);
 return false;
 }
 robot.svgPrintPosition();

 robot.points += 5;
 robot.displayPoints();

 ts = robot.asserv().doMoveForwardAndRotateTo(xmatch + 440, ymatch, 0);
 //ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x - 185 + 640, zone.y, false, 1000000, 5, 5, false);
 //ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(xmatch+440, ymatch, 0, true, 1000000, 5, 5, false);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_fanion 2  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);
 return false;
 }
 robot.svgPrintPosition();

 //on tourne doucement
 robot.asserv().setLowSpeedForward(true, 40);
 ts = robot.asserv().doRelativeRotateBy(-90);
 robot.svgPrintPosition();
 ts = robot.asserv().doLineAbs(100);

 robot.points += 10;
 robot.displayPoints();

 robot.actions().ax12_bras_gauche_init(0);
 robot.actions().ax12_bras_droit_init(0);

 //ts = robot.asserv().doLineAbs(100);
 robot.svgPrintPosition(1);

 return true; //return true si ok sinon false si interruption

 }

 bool O_goNS() {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_goNS." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;
 //si taches finies et temps < 85
 if (robot.chrono().getElapsedTimeInSec() < 80) {
 utils::sleep_for_secs(1);
 return false;
 }
 robot.logger().info() << "GOOOOOO O_goNS." << logs::end;

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(900, 800, -180, true, 1000000, 1, 1, false);
 //ts = robot.asserv().doMoveForwardAndRotateTo(900, 800, -180);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " centre ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);

 }
 robot.svgPrintPosition();
 robot.logger().info() << "TEST positions adv...." << logs::end;

 utils::sleep_for_secs(3);

 ASensorsDriver::bot_positions vadv = robot.actions().sensors().getPositionsAdv();
 bool south = false;
 int miny = 2000;
 for (ASensorsDriver::bot_positions::size_type i = 0; i < vadv.size(); i++) {

 robot.logger().info() << "TEST en direct : vadv nb="
 << vadv.size()
 << " detected="
 << vadv[i].nbDetectedBots
 << " x="
 << vadv[i].x
 << " y="
 << vadv[i].y
 << " a="
 << vadv[i].theta
 << " d="
 << vadv[i].d
 << logs::end;

 if (vadv[i].y < miny) {
 miny = vadv[i].y;
 }
 }
 if (miny > 800) south = true;
 robot.logger().info() << "south =" << south << logs::end;
 //si temps >85 on termine par cette tache

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

 if (south) {
 robot.logger().info() << " zone_south choosen" << ts << logs::end;
 //robot.ia().iAbyPath().goToZone("zone_north", &zoneN);
 robot.ia().iAbyPath().goToZone("zone_south", &zone);
 }
 else {
 robot.logger().info() << " zone_north choosen" << ts << logs::end;
 robot.ia().iAbyPath().goToZone("zone_north", &zone);
 }

 //detection de l'adversaire

 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 1000000, 2, 2, false);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_south  ===== PB COLLISION FINALE -  ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);
 }

 robot.svgPrintPosition();

 robot.points += 20;
 robot.displayPoints();

 return true; //return true si ok sinon false si interruption
 }

 bool O_push_petitport() {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_petitport." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);

 robot.ia().iAbyPath().goToZone("zone_petitport", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, true, 1000000, 2, 2, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().info() << " zone_green_left  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);
 return false;
 }
 robot.svgPrintPosition();

 ts = robot.asserv().doLineAbs(180);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " on pousse le vert  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 robot.svgPrintPosition(2);
 }
 robot.svgPrintPosition();

 ts = robot.asserv().doLineAbs(-90);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " on pousse le vert  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 robot.svgPrintPosition();
 return true; //return true si ok sinon false si interruption
 }
 */

/*
 bool O_fake_blue()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_fake." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 if (robot.gold_taken == true)
 return true;

 robot.svgPrintPosition();

 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);

 robot.ia().iAbyPath().goToZone("zone_fake_blue", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, false, 1000000, 0, 0, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_fake_blue  ===== PB COLLISION FINALE - on retourne à la next strat ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 return false; //return true si ok sinon false si interruption
 }

 bool O_push_blue()
 {

 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

 if (robot.gold_door_opened == true) {
 robot.logger().info() << "gold_door_opened, then skip blue..." << logs::end;
 return true;
 }

 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;
 robot.svgPrintPosition();

 robot.logger().info() << "start O_push_blue." << logs::end;
 robot.ia().iAbyPath().goToZone("zone_push_blue", &zone);
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 int dest_blue = zone.y;
 int zonex = zone.x;

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, false, 1000000, 0, 0, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_push_blue  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }

 robot.svgPrintPosition();
 robot.logger().error() << "POS1 : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

 float mesure_mm_moy = 0;
 if (robot.getMyColor() == PMXVIOLET) {
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 mesure_mm_moy = robot.actions().sensors().multipleRightSide(10) + 30.0;
 robot.logger().error() << "--------------moy right: " << mesure_mm_moy << logs::end;
 //bool change = robot.asserv().calculateDriftRightSideAndSetPos(260, moy, 70, 450 + 13);
 } else {
 mesure_mm_moy = robot.actions().sensors().multipleLeftSide(10) + 40.0;
 }

 RobotPosition p = robot.asserv().pos_getPosition();
 float pos_x_start_mm = 70;
 float pos_y_start_mm = 450;
 float delta_j_mm = 85;
 float delta_k_mm = 0;
 float robot_size_l_mm = 150;
 int succeed = robot.asserv().adjustRealPosition(pos_x_start_mm, pos_y_start_mm, p, delta_j_mm, delta_k_mm,
 mesure_mm_moy, robot_size_l_mm);

 robot.svgPrintPosition();
 robot.logger().error() << "POS REAL : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm()
 << " " << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;
 if (succeed > 0) {
 //if (change) {
 if (mesure_mm_moy > 260) {
 ts = robot.asserv().doMoveBackwardAndRotateTo(zonex, 260.0, zone.theta);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error()
 << " go to doMoveBackwardAndRotateTo dest_blue  ===== PB COLLISION FINALE - Que fait-on? ts="
 << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 } else {
 ts = robot.asserv().doMoveForwardAndRotateTo(zonex, 260.0, zone.theta);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error()
 << " go to doMoveForwardAndRotateTo dest_blue  ===== PB COLLISION FINALE - Que fait-on? ts="
 << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 }
 } else {
 robot.logger().error() << "succeed=" << succeed << " TODO try AGAIN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
 << logs::end;
 //TODO try again !!!
 }
 //
 //     } else {
 //
 //     robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 //     robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 //
 //     float mesure_mm_moy = robot.actions().sensors().multipleLeftSide(10);
 //     robot.logger().error() << "--------------moy left: " << mesure_mm_moy << logs::end;
 //     //bool change = robot.asserv().calculateDriftLeftSideAndSetPos(260, moy, 70, 450 + 13);
 //     //if (change) {
 //
 //     RobotPosition p = robot.asserv().pos_getPosition();
 //     float pos_x_start_mm = 70;
 //     float pos_y_start_mm = 450;
 //     float delta_j_mm = 100;
 //     float delta_k_mm = 100;
 //     float robot_size_l_mm = 150;
 //     int succeed = robot.asserv().adjustRealPosition(pos_x_start_mm, pos_y_start_mm, p, delta_j_mm, delta_k_mm,
 //     mesure_mm_moy, robot_size_l_mm);
 //
 //     robot.svgPrintPosition();
 //     robot.logger().error() << "POS REAL : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm()
 //     << " " << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;
 //
 //     if (succeed > 0) {
 //     if (mesure_mm_moy > 260) {
 //     ts = robot.asserv().doMoveBackwardAndRotateTo(zonex, 245.0, zone.theta);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error()
 //     << " go to doMoveBackwardAndRotateTo dest_blue - 30  ===== PB COLLISION FINALE - Que fait-on? ts="
 //     << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     } else {
 //     ts = robot.asserv().doMoveForwardAndRotateTo(zonex, 245.0, zone.theta);
 //     if (ts != TRAJ_FINISHED) {
 //     robot.logger().error()
 //     << " go to doMoveForwardAndRotateTo dest_blue - 30  ===== PB COLLISION FINALE - Que fait-on? ts="
 //     << ts << logs::end;
 //     robot.asserv().resetEmergencyOnTraj();
 //     }
 //     }
 //
 //     } else {
 //     robot.logger().error() << "succeed=" << succeed << " TODO try AGAIN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
 //     << logs::end;
 //     //TODO try again !!!
 //     }
 //     }
 robot.svgPrintPosition();
 robot.logger().error() << "POS2 : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 robot.logger().info() << "on leve le bras" << logs::end;
 if (robot.getMyColor() == PMXVIOLET) {
 robot.actions().ax12_rightHand(-1);
 } else {
 robot.actions().ax12_leftHand(-1);
 }

 robot.logger().info() << "on pousse le bleu" << logs::end;
 ts = robot.asserv().doLineAbs(180);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " on pousse le bleu  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.svgPrintPosition();

 robot.logger().info() << "on retracte le bras" << logs::end;
 if (robot.getMyColor() == PMXVIOLET) {
 robot.actions().ax12_rightHand_retract();
 } else {
 robot.actions().ax12_leftHand_retract();
 }

 robot.points += 20;
 robot.displayPoints();

 robot.blue_done = true;
 return false; //return true si ok sinon false si interruption

 }

 bool O_take_gold()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 if (robot.blue_done == false)
 return false;
 robot.logger().info() << "start O_take_gold." << logs::end;

 robot.svgPrintPosition();

 robot.logger().info() << "on go au goldenium" << logs::end;
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 robot.ia().iAbyPath().goToZone("zone_gold", &zone);
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(zone.x, zone.y, zone.theta, false, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_gold  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }

 robot.svgPrintPosition();
 robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;
 //
 //     //test si porte goldenium
 //     robot.logger().info() << "on teste la porte du goldenium" << logs::end;
 //     int dist_porte1 = robot.actions().sensors().sensorDist("fC");
 //     int dist_porte2 = robot.actions().sensors().sensorDist("fC");
 //     int dist_porte3 = robot.actions().sensors().sensorDist("fC");
 //
 //     int corr = robot.asserv().pos_getY_mm() - 333.0;
 //     robot.logger().info() << "dist_porte1= " << dist_porte1 << " dist_porte2= " << dist_porte2 << " dist_porte3= "
 //     << dist_porte3 << " corr= " << corr << logs::end;
 //     if ((dist_porte1 + corr) <= 290) //255 et 290
 //     {
 //     robot.logger().info() << "PORTE FERMEE !!!!" << logs::end;
 //     robot.gold_door_opened = false;
 //     return false; // ne marche pas en simu !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 //     } else {
 //     robot.gold_door_opened = true;
 //     }
 robot.gold_door_opened = true;

 robot.actions().ax12_left_cil_retract_more(0);
 robot.actions().ax12_right_cil_retract_more();

 robot.logger().info() << "on avance pour prendre le goldenium" << logs::end;
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 ts = robot.asserv().doLineAbs(60);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " doLineAbs(60)  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }
 robot.asserv().doCalage(80, 35);    //80
 ts = robot.asserv().doLineAbs(-10);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " doLineAbs(-10)  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.svgPrintPosition();

 robot.logger().info() << "prise du goldenium" << logs::end;
 robot.actions().ax12_left_cil(0);
 robot.actions().ax12_right_cil();
 sleep(1);

 if (!robot.actions().ax12_goldenium_in_cil()) {
 robot.logger().info() << "louuuluuupé !!!! Que fait-on ? on essaye de le reprendre" << logs::end; //TODO a refaire
 robot.logger().info() << "prise du goldenium" << logs::end;

 robot.actions().ax12_left_cil_retract_more(0);
 robot.actions().ax12_right_cil_retract_more();
 sleep(1);
 robot.actions().ax12_left_cil(0);
 robot.actions().ax12_right_cil();
 sleep(1);
 }

 robot.logger().info() << "on recule un peu" << logs::end;
 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
 ts = robot.asserv().doLineAbs(-60);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " doLineAbs(-60)  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 float x_temp = robot.asserv().pos_getX_mm();
 float y_temp = robot.asserv().pos_getY_mm() + 130.0;
 ts = robot.ia().iAbyPath().whileMoveBackwardTo(robot.asserv().getRelativeX(x_temp), y_temp, false, 1000000, 5, 5);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_gold  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.points += 20;
 robot.displayPoints();

 robot.gold_taken = true;

 return true;
 }

 bool O_drop_gold()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_drop_gold." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 if (robot.gold_taken == false)
 return false;

 robot.svgPrintPosition();
 robot.logger().info() << "on va deposer" << logs::end;
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 robot.ia().iAbyPath().goToZone("zone_depose", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, false, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_depose  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }

 robot.logger().info() << "arrivé..." << logs::end;
 robot.svgPrintPosition();
 robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

 //le gros a besoin de 14sec
 //pause jusqu'à 50 sec
 int sec_min = 45;
 int sec_min2 = 76;
 int time = robot.chrono().getElapsedTimeInSec();
 if (time < sec_min) {
 sleep(sec_min - time);
 } else if (time < sec_min2) {
 //time = robot.chrono().getElapsedTimeInSec();
 if (time < sec_min2) {
 sleep(sec_min2 - time);
 }
 }
 robot.logger().info() << "go...doMoveForwardAndRotateTo(1320, 1230, 77);" << logs::end;
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1320, 1230, 77, false, 1000000, 5, 5, false);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_depose  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }

 robot.svgPrintPosition();
 robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

 robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
 robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

 robot.asserv().setLowSpeedForward(false, 0); //au cas où par les sensors (si pas de ARU) //a voir si on ne peut pas le mettre ailleurs à l'init

 robot.logger().info() << "doCalage..." << logs::end;
 robot.asserv().doCalage(350, 45);
 robot.svgPrintPosition();

 robot.logger().info() << "on lache le goldenium..." << logs::end;

 robot.actions().ax12_left_cil_release(-1);
 robot.actions().ax12_left_cil_release(-1);
 robot.actions().ax12_left_cil_release(-1);

 //usleep(500000); //3

 if (robot.getMyColor() == PMXVIOLET) {
 robot.actions().ax12_right_cil_retract_less(-1, 50);
 } else {
 robot.actions().ax12_left_cil_retract_less(-1, 50);
 }
 //sleep(1);
 //usleep(500000);

 robot.actions().ax12_left_cil(0);
 robot.actions().ax12_right_cil();

 robot.points += 24;
 robot.displayPoints();

 //sleep(1);
 //on recule
 ts = robot.asserv().doLineAbs(-120);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " go to doLineAbs(-120)  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.gold_dropped = true;

 return true;

 }

 bool O_fake_balance()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_fake_balance." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 if (robot.gold_dropped == true)
 return true;

 if (robot.gold_taken == false)
 return false;

 robot.svgPrintPosition();

 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);

 robot.ia().iAbyPath().goToZone("zone_fake_balance", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, false, 1000000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_fake_balance  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }

 //
 //     while ((ts = robot.ia().iAbyPath().doMoveForwardTo(zone.x, zone.y)) != TRAJ_FINISHED) {
 //     robot.svgPrintPosition();
 //     //robot.asserv().displayTS(ts);
 //     if (ts == TRAJ_NEAR_OBSTACLE) {
 //     robot.logger().error() << " O_fake_balance ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
 //     if (f > 2)
 //     return false;
 //     f++;
 //     usleep(200000);
 //     }
 //     if (ts == TRAJ_COLLISION) {
 //     robot.logger().error() << " O_fake_balance ===== COLLISION essai n°" << f << logs::end;
 //     if (f >= 1)
 //     return false;
 //     f++;
 //     }
 //     //robot.asserv().resetDisplayTS();
 //
 //     }
 robot.svgPrintPosition();
 robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;
 return false; //return true si ok sinon false si interruption
 }

 bool O_push_alea()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start O_push_alea." << logs::end;
 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition zone;

 if (robot.gold_dropped == false)
 return false;

 if (robot.getMyColor() == PMXVIOLET) {
 robot.ia().iAbyPath().enable(robot.ia().area_alea_violet, 0);
 robot.ia().iAbyPath().enable(robot.ia().area_palet_start2_violet, 0);
 robot.ia().iAbyPath().enable(robot.ia().area_palet_start_violet, 0);
 } else {
 robot.ia().iAbyPath().enable(robot.ia().area_alea_yellow, 0);
 robot.ia().iAbyPath().enable(robot.ia().area_palet_start2_yellow, 0);
 robot.ia().iAbyPath().enable(robot.ia().area_palet_start_yellow, 0);
 }


 //    int time = robot.chrono().getElapsedTimeInSec();
 //    if (time < 77) {
 //        ts = robot.ia().iAbyPath().whileMoveForwardTo(1300, 1000, true, 500000, 5, 5, true);
 //        if (ts != TRAJ_FINISHED) {
 //            robot.logger().error() << " zone fake 1300,1000  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 //                    << logs::end;
 //            robot.asserv().resetEmergencyOnTraj();
 //            return false;
 //        }
 //    }

 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 robot.ia().iAbyPath().goToZone("zone_alea_violet", &zone);

 ts = robot.ia().iAbyPath().whileMoveForwardTo(zone.x, zone.y, true, 500000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " zone_alea_violet  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }


 //     while ((ts = robot.ia().iAbyPath().doMoveForwardTo(zone.x, zone.y)) != TRAJ_FINISHED) {
 //     robot.svgPrintPosition();
 //     //robot.asserv().displayTS(ts);
 //     if (ts == TRAJ_NEAR_OBSTACLE) {
 //     robot.logger().error() << " O_push_alea ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
 //     if (f > 2)
 //     return false;
 //     f++;
 //     usleep(200000);
 //     }
 //     if (ts == TRAJ_COLLISION) {
 //     robot.logger().error() << " O_push_alea ===== COLLISION essai n°" << f << logs::end;
 //     if (f >= 1)
 //     return false;
 //     f++;
 //     }
 //     //robot.asserv().resetDisplayTS();

 }
 robot.svgPrintPosition();
 robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 robot.logger().info() << "pousse zone depart" << logs::end;

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(620, 650, 180, true, 500000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " pousse zone depart  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 return false;
 }

 //     while ((ts = robot.asserv().doMoveForwardAndRotateTo(620, 650, 180)) != TRAJ_FINISHED) {
 //     robot.svgPrintPosition();
 //     //robot.asserv().displayTS(ts);
 //     if (ts == TRAJ_NEAR_OBSTACLE) {
 //     robot.logger().error() << " O_push_alea pushed ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
 //     if (f > 2)
 //     return false;
 //     f++;
 //     usleep(200000);
 //     }
 //     if (ts == TRAJ_COLLISION) {
 //     robot.logger().error() << " O_push_alea pushed ===== COLLISION essai n°" << f << logs::end;
 //     if (f >= 1)
 //     return false;
 //     f++;
 //     }
 //     //robot.asserv().resetDisplayTS();
 //
 //     }
 robot.svgPrintPosition();
 robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
 << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

 robot.points += 14;
 robot.displayPoints();
 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
 ts = robot.asserv().doLineAbs(-120);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " doLineAbs(-120) ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }

 robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);

 ts = robot.ia().iAbyPath().whileMoveForwardAndRotateTo(1000, 1005, 60, false, 300000, 5, 5, true);
 if (ts != TRAJ_FINISHED) {
 robot.logger().error() << " pousse zone depart  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts
 << logs::end;
 robot.asserv().resetEmergencyOnTraj();
 }


 //     while ((ts = robot.asserv().doMoveForwardAndRotateTo(1000, 1005, 60)) != TRAJ_FINISHED) {
 //     robot.svgPrintPosition();
 //     //robot.asserv().displayTS(ts);
 //     if (ts == TRAJ_NEAR_OBSTACLE) {
 //     robot.logger().error() << " O_push_alea pushed ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
 //     if (f > 2)
 //     return false;
 //     f++;
 //     usleep(200000);
 //     }
 //     if (ts == TRAJ_COLLISION) {
 //     robot.logger().error() << " O_push_alea pushed ===== COLLISION essai n°" << f << logs::end;
 //     if (f >= 1)
 //     return false;
 //     f++;
 //     }
 //     //robot.asserv().resetDisplayTS();
 //
 //     }
 robot.svgPrintPosition();
 return true;
 }
 */

/*
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
 //p_->add_rectangle(robot.ia().area_palet_start_violet, 500.0, 750.0, 300.0, 950.0, 0.0);
 //p_->add_rectangle(robot.ia().area_palet_start_yellow, 2500.0, 750.0, 300.0, 950.0, 0.0);

 p_->compute_edges();

 robot.ia().iAbyPath().addPlayground(p_);
 robot.ia().iAbyPath().toSVG();
 }*/
