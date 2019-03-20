#include "A_State1.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/AAutomateState.hpp"
//#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "A_State_Wait90SecAction.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328AsservExtended.hpp"
#include "APF9328RobotExtended.hpp"

IAutomateState*
A_State1::execute(Robot&)//, void *data)
{
	logger().info() << "A_State1" << logs::end;

	//Data* sharedData = (Data*) data;
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.actions().start();
	begin:
	robot.actions().lcd2x16().setBacklightOn();
	robot.actions().lcd2x16().clear();

	//sharedData->strategy("all");

	if (1)//!sharedData->skipSetup())
	{
		logger().info() << "METTRE LA TIRETTE ! " << logs::end;
		robot.actions().lcd2x16().home();
		robot.actions().lcd2x16().print("METTRE LA TIRETTE ! ");
		robot.actions().tirette().waitPressed();
		robot.actions().lcd2x16().clear();

		robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);

		robot.actions().lcd2x16().clear();
		robot.actions().lcd2x16().home();
		robot.actions().lcd2x16().print("CHOISIR COULEUR + IA...");

		ButtonTouch b = BUTTON_NONE;
		while (b != BUTTON_BACK_KEY || robot.getMyColor() == PMXNOCOLOR)
		{
			b = robot.actions().buttonBar().waitOneOfAllPressed();
			if (b == BUTTON_LEFT_KEY)
			{
				logger().info() << "BUTTON_LEFT_KEY - BLUE" << logs::end;
				robot.actions().lcd2x16().clear();
				robot.actions().lcd2x16().home();
				robot.actions().lcd2x16().print("BLUE  ");
				robot.setMyColor(PMXVIOLET);
			}
			if (b == BUTTON_RIGHT_KEY)
			{
				logger().info() << "BUTTON_RIGHT_KEY - YELLOW" << logs::end;
				robot.actions().lcd2x16().clear();
				robot.actions().lcd2x16().home();
				robot.actions().lcd2x16().print("YELLOW");
				robot.setMyColor(PMXYELLOW);
			}
			if (b == BUTTON_UP_KEY)
			{
				logger().info() << "BUTTON_UP_KEY - IA" << logs::end;
				//sharedData->strategy("strat5");
			}
			if (b == BUTTON_DOWN_KEY)
			{
				logger().info() << "BUTTON_DOWN_KEY - MECA" << logs::end;
				/*robot.actions().servoObjects().leftDeploy(15, false); //deploy
				robot.actions().servoObjects().rightDeploy(15, false); //deploy
				robot.actions().servoObjects().leftDeploy(-85, false); //retract
				robot.actions().servoObjects().rightDeploy(-75, false); //retract
*/
			}
		}

		robot.actions().lcd2x16().clear();
		robot.actions().lcd2x16().home();
		robot.actions().lcd2x16().print("SET POSITION : OK");
		robot.actions().ledBar().stopAndWait(true);

		setPos();

		//tirette
		robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_GREEN, false);
		robot.actions().lcd2x16().clear();
		robot.actions().lcd2x16().print("PMX...WAIT GO !");

		bool bb = false;
		//robot.actions().tirette().waitUnpressed();
		while (robot.actions().tirette().pressed())
		{
			bb = robot.actions().buttonBar().pressed(BUTTON_DOWN_KEY);
			if (bb)
			{
				robot.actions().ledBar().stopAndWait(true);
				robot.actions().lcd2x16().clear();
				goto begin;
			}
			usleep(100000);
		}
	}
	else
	{
		if (robot.getMyColor() == PMXNOCOLOR)
		{
			robot.actions().lcd2x16().home();
			robot.actions().lcd2x16().print("NO COLOR... => EXIT !!");
			exit(0);
		}

		robot.actions().lcd2x16().home();
		robot.actions().lcd2x16().print("Skip setup...");

		//sharedData->strategy("all");

		setPos();
	}

	robot.actions().lcd2x16().clear();
	robot.actions().lcd2x16().setBacklightOff();

	robot.actions().ledBar().stopAndWait(true);

	robot.actions().ledBar().startReset();
	robot.actions().ledBar().stop(true);

	//start waitFor90s
	//A_State_Wait90SecAction* action = new A_State_Wait90SecAction(robot);//, (void *) sharedData);
	//action->start("A_Wait90SecAction");

	return this->getState("decisionMaker");
	//return NULL; //finish all state
}

void A_State1::setPos()
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().setPositionAndColor(105, 1250, 0.0, (robot.getMyColor() == PMXVIOLET));
	robot.svgPrintPosition();

}
