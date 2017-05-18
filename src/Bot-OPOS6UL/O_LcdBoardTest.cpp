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
	logger().info() << "Executing - " << this->desc() << logs::end;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	robot.actions().lcd2x16().clear();
	robot.actions().lcd2x16().home();
	robot.actions().lcd2x16().print("Start...");
	robot.actions().lcd2x16().setCursor(0, 1);
	robot.actions().lcd2x16().print("PM-ROBOTIX");

	sleep(2);

	robot.actions().stop();

	robot.stop();
	logger().info() << "OPOS6UL_RobotExtended Happy End." << logs::end;
}

