/*!
 * \file
 * \brief Implémentation de la classe LegoEV3SoundBarTest.
 */

#include "L_ServoObjectTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoObjectTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//peche en green

	robot.actions().servoObjects().centreDeploy(0);
	robot.actions().servoObjects().leftDeploy(100);
	robot.actions().servoObjects().rightDeploy(-100);
	sleep(1);
	robot.actions().servoObjects().centreDeploy(-0);
	sleep(1);
	robot.actions().servoObjects().centreDeploy(-60);


	robot.actions().servoObjects().leftDeploy(-100);

	sleep(1);
	robot.actions().servoObjects().leftDeploy(100);
	robot.actions().servoObjects().centreDeploy(0);


	robot.actions().servoObjects().releaseAll();

	logger().info() << "Happy End." << logs::end;
	robot.stop();

}

