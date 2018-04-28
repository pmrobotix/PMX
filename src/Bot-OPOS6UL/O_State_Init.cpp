#include "O_State_Init.hpp"

#include "../Common/Action/LcdShield.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

IAutomateState*
O_State_Init::execute(Robot&)
{
    logger().info() << "O_StateInit executing..." << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();





    logger().info() << "O_StateInit executed" << logs::end;
    return this->getState("WaitEndOfMatch"); //return NULL; //finish all state
}

void O_State_Init::setPos()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().print("SET POSITION : OK");

    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(70, 500, 0.0, (robot.getMyColor() != PMXORANGE));
    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().assistedHandling();
    robot.asserv().doLineAbs(50);
    robot.asserv().doLineAbs(-50);
    robot.asserv().freeMotion();

}
