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

L_State_Wait90SecAction::L_State_Wait90SecAction(Robot& robot, void *data)
		: robot_(robot), data_(data)
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
	while (robot.chrono().getElapsedTimeInSec() <= 10)
	{
		//test ARU
		if (robot.actions().tirette().pressed())
		{
			//stop all robot
			robot.stop();

			sharedData->end90s(true);
			//usleep(200000);
			exit(0);
		}
		usleep(200000);
		this->logger().debug() << "chrono " << robot.chrono().getElapsedTimeInSec() << logs::end;
	}

	this->logger().info() << "end90s...stop... "
			<< robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	robot.asserv_default->freeMotion();

	//FUNNY ACTION
	this->logger().info() << "FUNNY ACTION...start... "
					<< robot.chrono().getElapsedTimeInSec()
					<< logs::end;
	sleep(5);

	this->logger().info() << "FUNNY ACTION...stop... "
				<< robot.chrono().getElapsedTimeInSec()
				<< logs::end;

	sharedData->end90s(true); //indique que l'action est effectuée au prog princ
	robot.stop();
	usleep(700000);
	exit(0);
}
