/*!
 * \file
 * \brief Implémentation de la classe LedBarTest.
 */

#include "A_ButtonBarTest.hpp"

#include <string>

#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/Logger.hpp"
#include "APF9328RobotExtended.hpp"


using namespace std;

void A_ButtonBarTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	logger().info() << "Please press ENTER" << logs::end;
	robot.actions().buttonBar().waitPressed(BUTTON_ENTER_KEY);


	logger().info() << "Please press UP" << logs::end;
	robot.actions().buttonBar().waitPressed(BUTTON_UP_KEY);

	logger().info() << "Please press DOWN" << logs::end;
	robot.actions().buttonBar().waitPressed(BUTTON_DOWN_KEY);

	logger().info() << "Please press LEFT" << logs::end;
	robot.actions().buttonBar().waitPressed(BUTTON_LEFT_KEY);

	logger().info() << "Please press RIGHT" << logs::end;
	robot.actions().buttonBar().waitPressed(BUTTON_RIGHT_KEY);

	//logger().info() << "Please press BACK" << logs::end;
	//robot.actions().buttonBar().waitPressed(BUTTON_BACK_KEY);

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

