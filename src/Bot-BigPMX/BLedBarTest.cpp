/*!
 * \file
 * \brief Implémentation de la classe LedBarTest.
 */

#include "BLedBarTest.hpp"

#include <string>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/Logger.hpp"
#include "BActionsExtended.hpp"
#include "BRobotExtended.hpp"


using namespace std;

void BLedBarTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	//SRobotExtended robot;
	BRobotExtended &robot = BRobotExtended::instance();

	robot.actions().ledBar().resetAll();


	robot.actions().ledBar().setOff(0);
	robot.actions().ledBar().setOff(1);
	usleep(500000);
	robot.actions().ledBar().setOn(0);
	robot.actions().ledBar().setOn(1);
	usleep(500000);
	robot.actions().ledBar().setOff(0);
	robot.actions().ledBar().setOff(1);
	usleep(500000);
	robot.actions().ledBar().set(0, LED_RED);
	usleep(1000000);
	robot.actions().ledBar().set(0, LED_ORANGE);
	usleep(1000000);
	robot.actions().ledBar().set(0, LED_GREEN);
	usleep(1000000);
	robot.actions().ledBar().setOff(0);
	usleep(1000000);

	robot.actions().ledBar().blink(5, 200000, LED_ORANGE);

	robot.actions().ledBar().k2mil(4, 100000, LED_RED);

	logger().info() << "start ActionManager..." << logs::end;
	robot.actions().start();

	//robot.actions.ledbar().startSet(0,1);
	//robot.actions.ledBar().startAlternate(10, 500000, 0x03, 0x01, LED_GREEN, true);
/*
	robot.actions.ledBar().startK2mil(2, 100000, LED_ORANGE, false);
	sleep(3);
	robot.actions.ledBar().startBlinkPin(10, 100000, 0, LED_ORANGE, false);
	sleep(3);

	*/
	robot.actions().ledBar().startK2mil(5, 100000, LED_ORANGE, true);


	robot.actions().stop();

	robot.actions().ledBar().resetAll();

	logger().info() << "Happy End." << logs::end;
}

