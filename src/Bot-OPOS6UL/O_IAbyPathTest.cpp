/*!
 * \file
 * \brief Implémentation de la classe O_IAByPathTest.
 */

#include "O_IAbyPathTest.hpp"

#include <src/pmr_node.h>
#include <src/pmr_path_result.h>
#include <src/pmr_point.h>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

bool O_IAbyPathTest_action1()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start action1." << logs::end;

    /*
     TRAJ_STATE ts = TRAJ_OK;
     RobotPosition path, zone;

     robot.ia().iAbyZone().goToZone("zone1", &path, &zone);
     ts = robot.asserv().doMoveForwardTo(path.x, path.y);
     if (ts != TRAJ_OK)
     return false;
     robot.svgPrintPosition();
     ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
     if (ts != TRAJ_OK)
     return false;
     */
    robot.svgPrintPosition();

    robot.logger().info() << "action1 done." << logs::end;
    return true; //return true si ok sinon false si interruption
}

bool O_IAByPathTest_action2()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start action2." << logs::end;
    TRAJ_STATE ts = TRAJ_OK;

    RobotPosition zone;
    robot.ia().iAbyPath().goToZone("zone_1", &zone); //recuperation de la zone //TODO remplacer par getzone

    //robot.ia().iAbyPath().enable(robot.ia().oarea_cube1, 0);
    //opponent p->move(obj, 0.0, 300.0)->synchronize();

    robot.logger().info() << "path1" << logs::end;
    //robot.ia().iAbyPath().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    robot.ia().iAbyPath().doMoveForwardAndRotateTo(1600, 1600, 90);

    robot.logger().info() << "path2" << logs::end;
    robot.ia().iAbyPath().doMoveForwardAndRotateTo(200, 1600, 90);

    //robot.logger().info() << "path2 without iA" << logs::end;
    //ts = robot.asserv().doMoveForwardTo(100, 1600);



    //toSVG(p, path, "testRobot3.svg"); //TODO afficher le path

    //TRAJ_STATE ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);

    /*
     TRAJ_STATE ts = TRAJ_OK;
     RobotPosition path, zone;

     robot.ia().iAbyZone().goToZone("zone2", &path, &zone);
     ts = robot.asserv().doMoveForwardTo(path.x, path.y);
     if (ts != TRAJ_OK)
     return false;
     robot.svgPrintPosition();
     ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
     if (ts != TRAJ_OK)
     return false;
     */
    robot.svgPrintPosition();

    robot.logger().info() << "action2 done." << logs::end;
    return true; //return true si ok sinon false si interruption
}
/*
 bool O_IAByPathTest_action3()
 {
 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
 robot.logger().info() << "start action3." << logs::end;

 TRAJ_STATE ts = TRAJ_OK;
 RobotPosition path, zone;

 robot.ia().iAbyZone().goToZone("zone3", &path, &zone);
 ts = robot.asserv().doMoveForwardTo(path.x, path.y);
 if (ts != TRAJ_OK)
 return false;
 robot.svgPrintPosition();
 ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
 if (ts != TRAJ_OK)
 return false;

 robot.svgPrintPosition();

 robot.logger().info() << "action3 done." << logs::end;
 return true;//return true si ok sinon false si interruption
 }*/

void O_IAByPathTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.asserv().startMotionTimerAndOdo(true);
    robot.asserv().setPositionAndColor(100, 500, 0.0, (robot.getMyColor() != PMXORANGE));
    robot.svgPrintPosition();

    IASetup();
    initPlayground();

    robot.chrono().start();
    robot.ia().iAbyPath().ia_start(); //launch IA

    robot.stopAll();

    logger().info() << "Happy End - " << robot.chrono().getElapsedTimeInSec() << " sec" << logs::end;
}

void O_IAByPathTest::IASetup()
{
    logger().debug() << "IASetup" << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 500, 1000, 100, 400, 0);
    robot.ia().iAbyPath().ia_createZone("zone_1", 900, 500, 200, 100, 1400, 600, 90);
    robot.ia().iAbyPath().ia_createZone("zone_2", 0, 1500, 500, 500, 400, 1600, -135);

    //robot.ia().iAbyZone().ia_createZone("zone_3", 700, 1700, 200, 100, 700, 1500, 90);

    robot.ia().iAbyPath().ia_addAction("action1", &O_IAbyPathTest_action1);
    robot.ia().iAbyPath().ia_addAction("action2", &O_IAByPathTest_action2);
    // robot.ia().iAbyPath().ia_addAction("action3", &O_IAByPathTest_action3);
}

void O_IAByPathTest::initPlayground()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
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
    p_->add_circle(robot.ia().oarea_cube3left, 300.0, 1200.0, 300.0, 6);


    //distributeurs
    p_->add_rectangle(robot.ia().oarea_distribadverse, 600, 1900, 200.0, 200.0, 0);

    //Green parts
    //cubes
    p_->add_circle(robot.ia().garea_cube1, 2150.0, 530.0, 300.0, 6);
    p_->add_circle(robot.ia().garea_cube2high, 1900.0, 1500.0, 300.0, 6);
    p_->add_circle(robot.ia().garea_cube3left, 2700.0, 1200.0, 300.0, 6);

    //distributeurs
    p_->add_rectangle(robot.ia().garea_distribadverse, 2400, 1900, 200.0, 200.0, 0);

    //stations d'épuration
    p_->add_rectangle(robot.ia().area_stations, 1500.0, 1850.0, 1400.0, 300.0, 0);
    //p_->add_circle(robot.ia().opponent_1, 1200.0, 1200.0, 100.0, 6);

    p_->compute_edges();

    robot.ia().iAbyPath().addPlayground(p_);
    robot.ia().iAbyPath().toSVG();
}
