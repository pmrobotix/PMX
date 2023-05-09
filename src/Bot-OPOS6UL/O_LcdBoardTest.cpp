/*!
 * \file
 * \brief Implémentation de la classe O_LcdBoardTest.
 */

#include "O_LcdBoardTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/LcdShield.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_LcdBoardTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //robot.asserv().startMotionTimerAndOdo(true);

    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().home();
    robot.actions().lcd2x16().print("PM-ROBOTIX start");
    sleep(1);
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().setCursor(0, 0);
    robot.actions().lcd2x16().print("Hi, I'm PMX.");

    sleep(1);
    robot.actions().lcd2x16().setCursor(0, 1);

    robot.actions().lcd2x16().print("Yeahhh !");

    sleep(1);
    robot.actions().lcd2x16().setCursor(0, 0);
    robot.actions().lcd2x16().print("ooooxxyyyy  !");
    sleep(1);
    robot.actions().lcd2x16().setBacklightOff();


    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;
}

