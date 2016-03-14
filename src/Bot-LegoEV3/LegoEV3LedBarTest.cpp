#include "LegoEV3LedBarTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3LedBarTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.actions().ledBar().resetAll();
	/*
	 robot.actions().ledBar().setOff(0);
	 robot.actions().ledBar().setOff(1);
	 //usleep(500000);
	 robot.actions().ledBar().setOn(0);
	 robot.actions().ledBar().setOn(1);
	 //usleep(500000);
	 robot.actions().ledBar().setOff(0);
	 robot.actions().ledBar().setOff(1);
	 //usleep(500000);
	 robot.actions().ledBar().set(0, LED_RED);
	 //usleep(1000000);
	 robot.actions().ledBar().set(0, LED_ORANGE);
	 //usleep(1000000);
	 robot.actions().ledBar().set(0, LED_GREEN);
	 //usleep(1000000);
	 robot.actions().ledBar().setOff(0);
	 //usleep(1000000);

	 robot.actions().ledBar().blink(5, 200000, LED_ORANGE);

	 robot.actions().ledBar().k2mil(4, 100000, LED_RED);

	 */

	robot.actions().start();

	//robot.actions().ledBar().startSet(0, LED_ORANGE);

	//robot.actions().ledBar().startAlternate(10, 200000, 0x03, 0x01, LED_GREEN, true);
	//robot.actions().ledBar().startK2mil(5, 400000, LED_RED, true);

	robot.actions().ledBar().startK2mil(20, 100000, LED_ORANGE, false);
	sleep(2);
	robot.actions().ledBar().startBlinkPin(15, 100000, 0, LED_RED, false);
	sleep(2);



	robot.actions().stop();

	//robot.actions().ledBar().k2mil(4, 70000, LED_RED);
	robot.actions().ledBar().resetAll();

	logger().info() << "LegoEV3LedBarTest Happy End." << logs::end;
}

