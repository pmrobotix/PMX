#include "O_State_DecisionMakerIA.hpp"

#include <unistd.h>

#include "../Common/Action/LedBar.hpp"
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

bool O_push_button()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_button." << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    RobotPosition zone;

    robot.ia().iAbyPath().goToZone("zone_push_button", &zone);
    ts = robot.asserv().doMoveForwardAndRotateTo(zone.x, zone.y, zone.theta);
    if (ts != TRAJ_OK)
        return false;
    robot.svgPrintPosition();

    robot.logger().info() << "O_push_button done." << logs::end;
    return true; //return true si ok sinon false si interruption
}

bool O_push_cube()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_cube." << logs::end;

    robot.logger().info() << "O_push_cube done." << logs::end;
    return true; //return true si ok sinon false si interruption
}

bool O_push_bee()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.logger().info() << "start O_push_bee." << logs::end;

    robot.logger().info() << "O_push_bee done." << logs::end;
    return true; //return true si ok sinon false si interruption
}

void O_State_DecisionMakerIA::IASetupActivitiesZone()
{
    logger().info() << "IASetupActivitiesZone" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.ia().iAbyPath().ia_createZone("depart", 0, 0, 400, 650, 200, 500, 0);
    robot.ia().iAbyPath().ia_createZone("zone_push_cube", 800, 500, 100, 300, 850, 950, -90);
    robot.ia().iAbyPath().ia_createZone("zone_push_button", 1000, 0, 300, 400, 1150, 300, 90);
    robot.ia().iAbyPath().ia_createZone("zone_push_bee", 0, 1700, 300, 300, 300, 1750, -10);

    robot.ia().iAbyPath().ia_addAction("push_button", &O_push_button);
    robot.ia().iAbyPath().ia_addAction("push_cube", &O_push_cube);
    robot.ia().iAbyPath().ia_addAction("push_bee", &O_push_bee);

    logger().info() << " END IASetupActivitiesZone" << logs::end;
}

void O_State_DecisionMakerIA::initPlayground()
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
    p_->add_circle(robot.ia().opponent_1, 1200.0, 1200.0, 100.0, 6);

    p_->compute_edges();

    robot.ia().iAbyPath().addPlayground(p_);
    robot.ia().iAbyPath().toSVG();
}

void O_State_DecisionMakerIA::execute()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    IASetupActivitiesZone(); //definit les activities

    initPlayground(); //definit les zones de non droit

    //wait for the start of the chrono !
    while (!robot.chrono().started()) {
        usleep(10000);
    }

    logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;
    robot.svgPrintPosition();

    //robot.actions().sensors().startSensors();
    robot.ia().iAbyPath().ia_start(); //launch IA

    //robot.actions().ledBar().k2mil(100, 100000, LED_GREEN);

    robot.actions().ledBar().startK2mil(100, 100000, LED_GREEN);

    logger().info() << "O_State_DecisionMakerIA executed" << logs::end;
    robot.actions().stop();
    robot.freeMotion();

}
