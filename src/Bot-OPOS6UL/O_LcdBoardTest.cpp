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
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_LcdBoardTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().home();
    robot.actions().lcd2x16().print("PM-ROBOTIX start");
    sleep(1);
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().setCursor(0, 0);
    //robot.actions().lcd2x16().print("Hi, I'm PMX.");
    robot.actions().lcd2x16().print("Inna...");
    sleep(1);
    robot.actions().lcd2x16().setCursor(0, 1);

    robot.actions().lcd2x16().print("I happy with you !!");

    sleep(2);
    robot.actions().lcd2x16().setCursor(7, 0);
    robot.actions().lcd2x16().print("ooooxxyyyy !");
    sleep(2);

    robot.stopExtraActions();

    logger().info() << "OPOS6UL_RobotExtended Happy End." << logs::end;
}

