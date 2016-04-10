/*!
 * \file
 * \brief Implémentation de la classe APF9328LcdBoardTest.
 */

#include "APF9328LcdBoardTest.hpp"

#include <unistd.h>
#include <string>

#include "../Driver.ARM/Adafruit_RGBLCDShield.hpp"
#include "../Log/Logger.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void APF9328LcdBoardTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.actions().lcd2x16().clear();
	robot.actions().lcd2x16().home();
	robot.actions().lcd2x16().print("Start...");
	robot.actions().lcd2x16().setCursor(0, 1);
	robot.actions().lcd2x16().print("PM-ROBOTIX");

	sleep(2);
//	robot.actions().lcd2x16().clear();
//	robot.actions().lcd2x16().setBacklightOff();
	robot.actions().stop();

	robot.stop();
	logger().info() << "APF9328LcdBoardTest Happy End." << logs::end;
}

