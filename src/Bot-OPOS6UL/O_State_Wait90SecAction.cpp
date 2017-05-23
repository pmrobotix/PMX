#include "O_State_Wait90SecAction.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_RobotExtended.hpp"

class Robot;

O_State_Wait90SecAction::O_State_Wait90SecAction(Robot& robot, void *data) :
		robot_(robot), data_(data)
{
}

void O_State_Wait90SecAction::execute()
{
	this->logger().debug() << "start" << logs::end;

	Data* sharedData = (Data*) data_;
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	//démarrage du chrono
	logger().info() << "Start Chronometer" << logs::end;
	robot.chrono().start();

	//ARU and adversary ?
	while (robot.chrono().getElapsedTimeInSec() <= 10) //todo mettre en parametre
	{

		usleep(2000000);
		this->logger().debug() << "O_State_Wait90SecAction::execute chrono "
				<< robot.chrono().getElapsedTimeInSec() << logs::end;
	}

	this->logger().info() << "O_State_Wait90SecAction::execute end90s...stop... "
			<< robot.chrono().getElapsedTimeInSec() << logs::end;
	robot.asserv_default->freeMotion();

	sharedData->end90s(true); //indique que l'action est effectuée au prog princ

	//FUNNY ACTION
	this->logger().info() << "FUNNY ACTION...start... " << robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.actions().funnyAction_Activate();
	sleep(4);
	this->logger().info() << "FUNNY ACTION...stop... " << robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.actions().servoObjects().releaseAll();

	robot.baseStop();
	usleep(500000);
	robot.stop();
	exit(0);
}
