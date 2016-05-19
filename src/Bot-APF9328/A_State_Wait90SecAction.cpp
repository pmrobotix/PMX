
#include "A_State_Wait90SecAction.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "APF9328RobotExtended.hpp"

class Robot;

A_State_Wait90SecAction::A_State_Wait90SecAction(Robot& robot, void *data)
		: robot_(robot), data_(data)
{
}

void A_State_Wait90SecAction::execute()
{
	this->logger().debug() << "start" << logs::end;

	Data* sharedData = (Data*) data_;
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();


	//démarrage du chrono
	logger().info() << "Start Chronometer" << logs::end;
	robot.chrono().start();

	//ARU and adversary
	while (robot.chrono().getElapsedTimeInSec() <= 90) //todo mettre en parametre
	{

		usleep(2000000);
		this->logger().debug() << "chrono " << robot.chrono().getElapsedTimeInSec() << logs::end;
	}

	this->logger().info() << "end90s...stop... "
			<< robot.chrono().getElapsedTimeInSec()
			<< logs::end;

	sharedData->end90s(true); //indique que l'action est effectuée au prog princ
	robot.asserv_default->freeMotion();
	robot.stop();
	usleep(500000);
	exit(0);
}
