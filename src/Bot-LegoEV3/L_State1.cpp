#include "L_State1.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/FunnyAction.hpp"
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

	begin: if (!sharedData->skipSetup())
	{

		//robot.actions().ledBar().startBlink(100000, 500000, LED_RED, false);
		robot.actions().ledBar().set(1, LED_RED);
		robot.actions().ledBar().set(0, LED_RED);

		robot.actions().tirette().waitPressed();

		//robot.actions().ledBar().stopAndWait(true);
		//robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);
		robot.actions().ledBar().set(1, LED_OFF);
		robot.actions().ledBar().set(0, LED_OFF);

		robot.actions().servoObjects().leftDeploy(100);
		robot.actions().servoObjects().rightDeploy(100);

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
				robot.actions().parasol().reset();
				robot.actions().parasol().activate(-20);
				//sleep(1);
				if (robot.getMyColor() == PMXGREEN)
				{
					robot.actions().servoObjects().centreDeploy(10);
					robot.actions().servoObjects().centreDeploy(-65); //on abaisse la canne
					robot.actions().servoObjects().leftDeploy(-100); //on lache les poissons
					robot.actions().servoObjects().leftDeploy(100);
					robot.actions().servoObjects().centreDeploy(10);//on releve la canne
				}
				if (robot.getMyColor() == PMXVIOLET)
				{
					robot.actions().servoObjects().centreDeploy(10);
					robot.actions().servoObjects().centreDeploy(85); //on abaisse la canne
					robot.actions().servoObjects().rightDeploy(-100); //on lache les poissons
					robot.actions().servoObjects().rightDeploy(100);
					robot.actions().servoObjects().centreDeploy(10);//on releve la canne
				}

				sleep(1);
				robot.actions().servoObjects().releaseAll();
			}
			usleep(10000);
		}
		//robot.actions().ledBar().stopAndWait(true);

		//tirette
		//robot.actions().ledBar().startAlternate(100000, 100000, 0x00, 0x03, LED_YELLOW, false);
		robot.actions().ledBar().set(1, LED_YELLOW);
		robot.actions().ledBar().set(0, LED_YELLOW);
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
		//robot.actions().ledBar().stopAndWait(true);

		setPos(); //initialise color before!
	}
	else
	{
		logger().error() << "robot.getMyColor()=" << robot.getMyColor() << logs::end;
		if (robot.getMyColor() == PMXNOCOLOR)
		{
			exit(0);
		}
		robot.actions().parasol().reset();
		robot.actions().parasol().activate(-20);
		robot.actions().servoObjects().leftDeploy(100);
		robot.actions().servoObjects().rightDeploy(100);
		if (robot.getMyColor() == PMXGREEN)
			robot.actions().servoObjects().centreDeploy(0);
		if (robot.getMyColor() == PMXVIOLET)
			robot.actions().servoObjects().centreDeploy(35);

		//usleep(500000);
		setPos(); //initialise color before!
	}

	//robot.actions().ledBar().stopAndWait(true);

	//robot.actions().ledBar().startReset();
	//robot.actions().ledBar().stop(true);
	//robot.actions().ledBar().stopAndWait(true);

	L_State_Wait90SecAction* action = new L_State_Wait90SecAction(robot, (void *) sharedData);
	action->start("L_State_Wait90SecAction");

	return this->getState("decisionMaker");
	//return NULL; //finish all state
}

void L_State1::setPos()
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().setPositionAndColor(85, 1007, 0.0, (robot.getMyColor() == PMXGREEN));
	robot.svgPrintPosition();

}
