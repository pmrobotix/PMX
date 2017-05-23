#include "O_ServoObjectsTest.hpp"

#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_ServoObjectsTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void O_ServoObjectsTest::run(int argc, char** argv)
{

	configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	Arguments args = robot.getArgs();

	logger().info() << "Executing - " << this->desc() << logs::end;


	//robot.actions().servo_init();

	robot.actions().servo_lowspeed();
	robot.actions().arm_retract();
	robot.actions().funnyAction_Init();
	robot.actions().nose_init();
	robot.actions().arm_center();
	robot.actions().turn_nene_center();


	sleep(5);
	robot.actions().servo_mediumspeed();
	logger().info() << "execute Funny action "<< logs::end;
	robot.actions().funnyAction_Activate();

	logger().info() << "prepare blue match"<< logs::end;
	robot.actions().turn_nene_left();
	sleep(2);

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

