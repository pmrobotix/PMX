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

		logger().info() << "ATTENTE MISE EN PLACE TIRETTE..." << logs::end;
		robot.actions().tirette().waitPressed();

		//robot.actions().ledBar().stopAndWait(true);
		//robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);
		robot.actions().ledBar().set(1, LED_OFF);
		robot.actions().ledBar().set(0, LED_OFF);

		robot.actions().pince_Open();
		robot.actions().pince_HerculeUp();
		robot.actions().funnyAction_Init();
		robot.actions().pince_InitRotation();

		logger().info() << "ATTENTE CHOIX COULEUR..." << logs::end;
		ButtonTouch b = BUTTON_NONE;
		while (b != BUTTON_ENTER_KEY || robot.getMyColor() == PMXNOCOLOR)
		{
			b = robot.actions().buttonBar().waitOneOfAllPressed();
			if (b == BUTTON_LEFT_KEY)
			{
				logger().info() << "BUTTON_LEFT_KEY - BLUE" << logs::end;
				robot.actions().ledBar().stopAndWait(true);
				robot.actions().ledBar().set(1, LED_GREEN);
				robot.actions().ledBar().set(0, LED_OFF);
				robot.setMyColor(PMXBLUE);
			}
			if (b == BUTTON_RIGHT_KEY)
			{
				logger().info() << "BUTTON_RIGHT_KEY - YELLOW" << logs::end;
				robot.actions().ledBar().stopAndWait(true);
				robot.actions().ledBar().set(1, LED_OFF);
				robot.actions().ledBar().set(0, LED_YELLOW);
				robot.setMyColor(PMXYELLOW);
			}
			if (b == BUTTON_UP_KEY)
			{
				//logger().info() << "BUTTON_UP_KEY - IA" << logs::end;
			}
			if (b == BUTTON_DOWN_KEY)
			{
				logger().info() << "BUTTON_DOWN_KEY - MECA" << logs::end;

				robot.actions().pince_Open();
				robot.actions().pince_HerculeDown();
				robot.actions().pince_InitRotation();
				robot.actions().pince_Rotate();
				robot.actions().pince_Close(1);
				robot.actions().pince_Open();
				robot.actions().pince_InitRotation();
				robot.actions().pince_HerculeUp();

				usleep(1);
				robot.actions().servoObjects().releaseAll();
			}
			//usleep(1000);
		}
		//robot.actions().ledBar().stopAndWait(true);

		//tirette
		//robot.actions().ledBar().startAlternate(100000, 100000, 0x00, 0x03, LED_YELLOW, false);

		//on affiche la couleur sur les 2 leds
		if (robot.getMyColor() == PMXBLUE)
		{
			robot.actions().ledBar().set(1, LED_GREEN);
			robot.actions().ledBar().set(0, LED_GREEN);
		}
		else if (robot.getMyColor() == PMXYELLOW)
		{
			robot.actions().ledBar().set(1, LED_YELLOW);
			robot.actions().ledBar().set(0, LED_YELLOW);
		}

		bool bb = false;

		logger().info() << "ATTENTE  TIRETTE..." << logs::end;
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
		logger().info() << "robot.getMyColor()=" << robot.getMyColor() << logs::end;
		if (robot.getMyColor() == PMXNOCOLOR)
		{
			exit(0);
		}
		robot.actions().pince_Open();
		robot.actions().pince_HerculeUp();
		robot.actions().funnyAction_Init();
		robot.actions().pince_InitRotation();

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
	//init avant bascule
	//robot.asserv().setPositionAndColor(60.05, 223.0, 180.0, (robot.getMyColor() != PMXYELLOW));
	robot.asserv().setPositionAndColor(114, 223.0, 180.0, (robot.getMyColor() != PMXYELLOW));

	//init après bascule
	//robot.asserv().setPositionAndColor(978.5, 110.0, 90.0, (robot.getMyColor() != PMXYELLOW));
	robot.svgPrintPosition();

}
