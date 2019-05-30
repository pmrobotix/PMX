#include "O_State_DecisionMakerIA.hpp"

#include <bits/basic_string.h>
#include <unistd.h>
#include <cmath>

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
bool O_fake_blue()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_fake." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;
    int f = 0;
    if (robot.gold_taken == true)
        return true;

    robot.svgPrintPosition();

//    robot.asserv().ignoreFrontCollision(false);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_fake_blue", &zone);

    while ((ts = robot.ia().iAbyPath().doMoveForwardTo(zone.x, zone.y)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_fake_blue ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_fake_blue ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }

        robot.asserv().resetDisplayTS();
    }
    return false; //return true si ok sinon false si interruption
}

bool O_push_blue()
{
    int f = 0;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_blue." << logs::end;
    if (robot.gold_door_opened == true) {
        robot.logger().info() << "gold_door_opened, then skip blue..." << logs::end;
        return true;
    }

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.svgPrintPosition();

//    robot.asserv().ignoreFrontCollision(false);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_push_blue", &zone);
    int dest_blue = zone.y;
    while ((ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_push_blue ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;

        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_push_blue ===== COLLISION essai n°" << f << logs::end;
            if (f >= 2)
                return false;
            f++;

        }
        usleep(500000);
        robot.asserv().resetDisplayTS();
    }
    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

    //test du calage terrain
    //TODO cote gauche a faire!!!!!
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
        robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
        int dist_1 = robot.actions().sensors().rightSide();
        int dist_2 = robot.actions().sensors().rightSide();
        int dist_3 = robot.actions().sensors().rightSide();
        float moy = (dist_1 + dist_2 + dist_3) / 3.0;
        bool change = robot.asserv().calculateDriftRightSideAndSetPos(dest_blue - 30, moy, 70, 450 + 13);
        if (change) {
            if (moy > dest_blue - 30)
                robot.asserv().doMoveBackwardAndRotateTo(zone.x, zone.y, zone.theta);
            else
                robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
        }
    } else {
        /*
        robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
        robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
        int dist_1 = robot.actions().sensors().leftSide();
        int dist_2 = robot.actions().sensors().leftSide();
        int dist_3 = robot.actions().sensors().leftSide();
        float moy = (dist_1 + dist_2 + dist_3) / 3.0;
        bool change = robot.asserv().calculateDriftLeftSideAndSetPos(dest_blue - 30, moy, 70, 450 + 13);
        if (change) {
            if (moy > dest_blue - 30)
                robot.asserv().doMoveBackwardAndRotateTo(zone.x, zone.y, zone.theta);
            else
                robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
        }*/
    }
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.logger().info() << "on leve le bras" << logs::end;
    if (robot.getMyColor() == PMXVIOLET) {
        robot.actions().ax12_rightHand(-1);
    } else {
        robot.actions().ax12_leftHand(-1);
    }

    robot.logger().info() << "on pousse le bleu" << logs::end;
//    robot.asserv().ignoreFrontCollision(true);
//    robot.asserv().ignoreRearCollision(true);
    robot.asserv().doLineAbs(150);
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
    //return true;
}

bool O_take_gold()
{
    int f = 0;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_take_gold." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    if (robot.blue_done == false)
        return false;

    robot.svgPrintPosition();

    robot.logger().info() << "on go au goldenium" << logs::end;
//    robot.asserv().ignoreFrontCollision(false);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().resetDisplayTS();

    robot.ia().iAbyPath().goToZone("zone_gold", &zone);
    while ((ts = robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_take_gold ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;

        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_take_gold ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }
        usleep(500000);
        robot.asserv().resetDisplayTS();
    }

    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

//test si porte goldenium
    robot.logger().info() << "on teste la porte du goldenium" << logs::end;
    int dist_porte1 = robot.actions().sensors().sensorDist("fC");
    int dist_porte2 = robot.actions().sensors().sensorDist("fC");
    int dist_porte3 = robot.actions().sensors().sensorDist("fC");

    int corr = robot.asserv().pos_getY_mm() - 333.0;
    robot.logger().info() << "dist_porte1= " << dist_porte1 << " dist_porte2= " << dist_porte2 << " dist_porte3= "
            << dist_porte3 << " corr= " << corr << logs::end;
    if ((dist_porte1 + corr) <= 290) //255 et 290
            {
        robot.logger().info() << "PORTE FERMEE !!!!" << logs::end;
        robot.gold_door_opened = false;
        return false; // ne marche pas en simu !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    } else {
        robot.gold_door_opened = true;
    }
    robot.actions().ax12_left_cil_retract_more(0);
    robot.actions().ax12_right_cil_retract_more();

    robot.logger().info() << "on avance pour prendre le goldenium" << logs::end;
//    robot.asserv().ignoreFrontCollision(true);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.asserv().resetDisplayTS();
    ts = robot.asserv().doLineAbs(60);
    robot.asserv().doCalage(80, 35);//80
    ts = robot.asserv().doLineAbs(-10);

    robot.asserv().displayTS(ts);
    robot.svgPrintPosition();

    robot.logger().info() << "prise du goldenium" << logs::end;
    robot.actions().ax12_left_cil(0);
    robot.actions().ax12_right_cil();
    sleep(1);

    if (!robot.actions().ax12_goldenium_in_cil()) {
        robot.logger().info() << "louuuuuupé !!!! on recommence" << logs::end; //TODO a refaire
    }
    robot.logger().info() << "on recule un peu" << logs::end;
//    robot.asserv().ignoreFrontCollision(true);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.asserv().doLineAbs(-130);
    robot.svgPrintPosition();

    robot.points += 20;
    robot.displayPoints();

    robot.gold_taken = true;

    return true;
}

bool O_drop_gold()
{

    int f = 0;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_drop_gold." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    if (robot.gold_taken == false)
        return false;

    robot.svgPrintPosition();
    robot.logger().info() << "on va deposer" << logs::end;
//    robot.asserv().ignoreFrontCollision(false);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_depose", &zone);
    while ((ts = robot.ia().iAbyPath().doMoveForwardTo(zone.x, zone.y)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_drop_gold zone_depose ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_drop_gold zone_depose ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }

        robot.asserv().resetDisplayTS();

    }
    robot.logger().info() << "arrivé..." << logs::end;
    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

    int sec = 45;
    int time = robot.chrono().getElapsedTimeInSec();
    if (time < sec) {
        sleep(45 - time);
    }

    robot.logger().info() << "go...doMoveForwardAndRotateTo(1320, 1230, 77);" << logs::end;
//    robot.asserv().ignoreFrontCollision(true);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);

    while ((ts = robot.asserv().doMoveForwardAndRotateTo(1320, 1230, 77)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_drop_gold 2 ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_drop_gold 2 ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }

        robot.asserv().resetDisplayTS();

    }

    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.logger().info() << "doCalage..." << logs::end;
    robot.asserv().doCalage(230, 45);
    robot.svgPrintPosition();

    robot.logger().info() << "on lache le goldenium..." << logs::end;

//robot.actions().ax12_left_cil_retract(0);
//robot.actions().ax12_right_cil_retract();
    robot.actions().ax12_left_cil_release(-1);



    robot.points += 24;
    robot.displayPoints();

    sleep(1);
//on recule
    robot.asserv().doLineAbs(-120);

    robot.gold_dropped = true;

    return true;

}

bool O_fake_balance()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_fake_balance." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;
    int f = 0;

    if (robot.gold_dropped == true)
        return true;

    if (robot.gold_taken == false)
        return false;

    robot.svgPrintPosition();

//    robot.asserv().ignoreFrontCollision(false);
//    robot.asserv().ignoreRearCollision(true);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_fake_balance", &zone);

    while ((ts = robot.ia().iAbyPath().doMoveForwardTo(zone.x, zone.y)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_fake_balance ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_fake_balance ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }
        robot.asserv().resetDisplayTS();

    }
    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;
    return false; //return true si ok sinon false si interruption
}

bool O_push_alea()
{
    int f = 0;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_alea." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    if (robot.gold_dropped == false)
        return false;

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().resetDisplayTS();
    robot.ia().iAbyPath().goToZone("zone_alea_violet", &zone);
    while ((ts = robot.ia().iAbyPath().doMoveForwardTo(zone.x, zone.y)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_push_alea ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_push_alea ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }
        robot.asserv().resetDisplayTS();

    }
    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.logger().info() << "pousse zone depart" << logs::end;
    while ((ts = robot.asserv().doMoveForwardAndRotateTo(620, 650, 180)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_push_alea pushed ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_push_alea pushed ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }
        robot.asserv().resetDisplayTS();

    }
    robot.svgPrintPosition();
    robot.logger().error() << "POS : " << robot.asserv().pos_getX_mm() << " " << robot.asserv().pos_getY_mm() << " "
            << robot.asserv().pos_getTheta() * 180.0 / M_PI << logs::end;

    robot.points += 14;
    robot.displayPoints();
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.asserv().doLineAbs(-120);
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    while ((ts = robot.asserv().doMoveForwardAndRotateTo(1000, 1005, 60)) != TRAJ_FINISHED) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " O_push_alea pushed ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            if (f > 2)
                return false;
            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << " O_push_alea pushed ===== COLLISION essai n°" << f << logs::end;
            if (f >= 1)
                return false;
            f++;
        }
        robot.asserv().resetDisplayTS();

    }
    robot.svgPrintPosition();
    return true;
}

void O_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().info() << "IASetupActivitiesZone definition" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 700, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_blue", 1680, 0, 200, 200, 1500 + 152, 230 + 30, 0);
    robot.ia().iAbyPath().ia_createZone("zone_fake_blue", 1680, 900, 200, 200, 1680, 900, 0);
    robot.ia().iAbyPath().ia_createZone("zone_fake_balance", 1680, 300, 200, 200, 1680, 300, 0);
    robot.ia().iAbyPath().ia_createZone("zone_gold", 2100, 0, 200, 200, 2225.0, 333.0, -90.0);
    robot.ia().iAbyPath().ia_createZone("zone_depose", 1300, 1400, 200, 200, 1500, 1000, 0);
    robot.ia().iAbyPath().ia_createZone("zone_alea_violet", 1000, 1100, 200, 200, 1275, 1320, 0);

    robot.ia().iAbyPath().ia_addAction("push_blue", &O_push_blue);
    robot.ia().iAbyPath().ia_addAction("take_gold", &O_take_gold);
    robot.ia().iAbyPath().ia_addAction("fake_blue", &O_fake_blue);
    robot.ia().iAbyPath().ia_addAction("drop_gold", &O_drop_gold);
    robot.ia().iAbyPath().ia_addAction("fake_balance", &O_fake_balance);
    robot.ia().iAbyPath().ia_addAction("push_alea", &O_push_alea);

    logger().debug() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::IASetupActivitiesZoneTableTest()
{
    logger().error() << "IASetupActivitiesZoneTableTest !!!!!!!!!!!!!!!!!!!!!!" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    logger().debug() << "color = " << robot.getMyColor() << logs::end;

    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 450, 650, 200, 700, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_blue", 810, 0, 200, 200, 740, 230 + 30, 0);
    robot.ia().iAbyPath().ia_createZone("zone_fake_blue", 810, 800, 200, 200, 810, 800, 0);
    robot.ia().iAbyPath().ia_createZone("zone_gold", 1300, 0, 200, 200, 1330.0, 333.0, -90.0);

    robot.ia().iAbyPath().ia_addAction("push_blue", &O_push_blue);
    robot.ia().iAbyPath().ia_addAction("take_gold", &O_take_gold);
    robot.ia().iAbyPath().ia_addAction("fake_blue", &O_fake_blue);

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
//p_->add_rectangle(robot.ia().area_palet_start_violet, 500.0, 750.0, 300.0, 950.0, 0.0);
//p_->add_rectangle(robot.ia().area_palet_start_yellow, 2500.0, 750.0, 300.0, 950.0, 0.0);

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
//logger().debug() << "waitForInit passed !!!!!!!" << logs::end;

    logger().info() << "Strategy to be applied = " << robot.strategy() << logs::end;

    if (robot.strategy() == "tabletest") {
        IASetupActivitiesZoneTableTest();
        initPlayground(); //definit les zones de non droit
        //enable zone
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);

    } else if (robot.strategy() == "all") {
        IASetupActivitiesZone(); //definit les activities
        initPlayground(); //definit les zones de non droit
        //enable zone
        //robot.ia().iAbyPath().enable(robot.ia().oarea_cube3left, 0);
        //robot.ia().iAbyPath().enable(robot.ia().garea_cube3left, 0);
    } else {
        logger().error() << "NO STRATEGY " << robot.strategy() << " FOUND !!! " << logs::end;
    }

//wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(10000);
    }

    logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;

//init rouge
    robot.asserv().doMoveForwardTo(330, 463);
    robot.asserv().doRotateTo(150);

    robot.points += 6;
    robot.displayPoints();
    robot.asserv().doLineAbs(-100);
    robot.points += 35;
    robot.displayPoints();

//On ajoute le timer de detection
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().addTimerSensors(50);

    robot.ia().iAbyPath().ia_start();        //launch IA

    robot.actions().ledBar().startK2mil(100, 100000, LED_GREEN);
//TODO danse de fin ?
//robot.actions().ledBar().k2mil(1, 50000, LED_GREEN); //Ne doit pas trop long...

//logger().info() << "O_State_DecisionMakerIA executed" << logs::end;
    robot.freeMotion();
//logger().info() << "O_State_DecisionMakerIA freeMotion" << logs::end;
//robot.actions().stop(); //TODO a supprimer ? sera fait par le Wait
//logger().info() << "O_State_DecisionMakerIA actions().stop" << logs::end;

    robot.svgPrintEndOfFile();
    logger().info() << "O_State_DecisionMakerIA svgPrintEndOfFile" << logs::end;

}

