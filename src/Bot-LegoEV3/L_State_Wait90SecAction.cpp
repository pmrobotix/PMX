#include "L_State_Wait90SecAction.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

class Robot;

L_State_Wait90SecAction::L_State_Wait90SecAction(Robot& robot, void *data) :
		robot_(robot), data_(data)
{
}

void L_State_Wait90SecAction::execute()
{
	this->logger().debug() << "start" << logs::end;

	Data* sharedData = (Data*) data_;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//démarrage du chrono
	logger().info() << "Start Chronometer" << logs::end;
	robot.chrono().start();

	//ARU and adversary
	while (robot.chrono().getElapsedTimeInSec() <= 90)
	{
		//test ARU
		if (robot.actions().tirette().pressed())
		{
			logger().error() << "ARU pressed !!!!!!" << logs::end;
			//stop all robot
			robot.stop();

			sharedData->end90s(true);
			usleep(200000);
			exit(0);
		}
		//robot.actions().ledBar().set(1, LED_OFF); //WARNING BUG DE LED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//robot.actions().ledBar().set(0, LED_OFF);
		usleep(300000);

		//robot.actions().ledBar().set(1, LED_RED);
		//robot.actions().ledBar().set(0, LED_RED);

		this->logger().debug() << "chrono " << robot.chrono().getElapsedTimeInSec() << logs::end;
	}

	this->logger().info() << "end90s...stop... " << robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.asserv_default->freeMotion();
	robot.actions().servoObjects().releaseAll();


	sharedData->end90s(true); //indique que l'action est effectuée au prog princ
	//FUNNY ACTION
	 this->logger().info() << "FUNNY ACTION...start... "
	 << robot.chrono().getElapsedTimeInSec()
	 << logs::end;

	 robot.actions().funnyAction_Activate();
	 sleep(4);
	 //robot.actions().parasol().activate(-150);
	 //robot.actions().funnyAction().release();
	 //sleep(1);

	 robot.actions().servoObjects().releaseAll();

	 this->logger().info() << "FUNNY ACTION...stop... "
	 << robot.chrono().getElapsedTimeInSec()
	 << logs::end;


	robot.baseStop();
	usleep(1000000); //on laisse la funnyaction se faire.
	robot.stop(); //arret si le prog principal n'a pas fini
	exit(0);
}
