/*!
 * \file
 * \brief Implémentation de la classe LedBarTest.
 */

#include "APF9328LedBarTest.hpp"

#include <string>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/Logger.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void APF9328LedBarTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	//SRobotExtended robot;
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	//robot.svg().writePosition(500, 500, 0, APF9328_SVG_POS_ROBOT);

	robot.actions().ledBar().resetAll();

	robot.actions().ledBar().setOff(0);
	robot.actions().ledBar().setOff(1);
	usleep(200000);
	robot.actions().ledBar().setOn(0);
	robot.actions().ledBar().setOn(1);
	usleep(200000);
	robot.actions().ledBar().setOff(0);
	robot.actions().ledBar().setOff(1);
	usleep(200000);

	robot.actions().ledBar().set(0, LED_ORANGE);
	usleep(200000);
	robot.actions().ledBar().set(0, LED_GREEN);
	usleep(200000);
	robot.actions().ledBar().setOff(0);
	usleep(200000);

	robot.actions().ledBar().blink(5, 200000, LED_ORANGE);

	robot.actions().ledBar().k2mil(4, 100000, LED_GREEN);

	robot.actions().start();

	robot.actions().ledBar().startSet(0, LED_RED);
	usleep(200000);

	robot.actions().ledBar().startAlternate(20, 200000, 0x55, 0xAA, LED_GREEN, true);

	robot.actions().ledBar().startK2mil(10, 50000, LED_ORANGE, true);
	//sleep(2);
	robot.actions().ledBar().startBlinkPin(20, 100000, 0, LED_RED, true);
	//sleep(2);

	robot.actions().stop();

	robot.stop();
	logger().info() << "APF9328LedBarTest Happy End." << logs::end;
}

