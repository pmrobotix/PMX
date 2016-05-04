#include "L_State1.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/AAutomateState.hpp"
#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "L_State_Wait90SecAction.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

IAutomateState*
L_State1::execute(Robot& r, void *data)
{
	logger().info() << "L_State1" << logs::end;
	Data* sharedData = (Data*) data;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.actions().start();

	begin:
	if (!sharedData->skipSetup())
	{
		robot.actions().ledBar().startBlink(100000, 500000, LED_RED, false);

		robot.actions().tirette().waitPressed();

		robot.actions().ledBar().stopAndWait(true);
		robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);

		ButtonTouch b = BUTTON_NONE;
		while (b != BUTTON_BACK_KEY || robot.getMyColor() == PMXNOCOLOR)
		{
			b = robot.actions().buttonBar().waitOneOfAllPressed();
			if (b == BUTTON_LEFT_KEY)
			{
				logger().info() << "BUTTON_LEFT_KEY - VIOLET" << logs::end;
				robot.actions().ledBar().stopAndWait(true);
				robot.actions().ledBar().set(1, LED_RED);
				robot.actions().ledBar().set(0, LED_OFF);
				robot.setMyColor(PMXVIOLET);
			}
			if (b == BUTTON_RIGHT_KEY)
			{
				logger().info() << "BUTTON_RIGHT_KEY - GREEN" << logs::end;
				robot.actions().ledBar().stopAndWait(true);
				robot.actions().ledBar().set(1, LED_OFF);
				robot.actions().ledBar().set(0, LED_GREEN);
				robot.setMyColor(PMXGREEN);
			}
			if (b == BUTTON_UP_KEY)
			{
				logger().info() << "BUTTON_UP_KEY - IA" << logs::end;
			}
			if (b == BUTTON_DOWN_KEY)
			{
				logger().info() << "BUTTON_DOWN_KEY - MECA" << logs::end;

			}
		}
		robot.actions().ledBar().stopAndWait(true);

		setPos();

		//tirette
		robot.actions().ledBar().startAlternate(100000, 100000, 0x00, 0x03, LED_YELLOW, false);
		bool bb = false;
		//robot.actions().tirette().waitUnpressed();
		while (robot.actions().tirette().pressed())
		{
			bb = robot.actions().buttonBar().pressed(BUTTON_DOWN_KEY);
			if (bb)
			{
				robot.actions().ledBar().stopAndWait(true);

				goto begin;
			}
			usleep(100000);
		}
	}
	else
	{
		if (robot.getMyColor() == PMXNOCOLOR)
		{
			logger().error() << "NO COLOR SELECTED !!" << logs::end;
			exit(0);
		}

		usleep(500000);
		setPos();
	}

	robot.actions().ledBar().stopAndWait(true);

	robot.actions().ledBar().startReset();
	robot.actions().ledBar().stop(true);

	L_State_Wait90SecAction* action = new L_State_Wait90SecAction(robot, (void *) sharedData);
	action->start("L_State_Wait90SecAction");

	return this->getState("decisionMaker");
	//return NULL; //finish all state
}

void L_State1::setPos()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().setPositionAndColor(85, 1000, 0.0, (robot.getMyColor() == PMXGREEN));
	robot.svgPrintPosition();

}
