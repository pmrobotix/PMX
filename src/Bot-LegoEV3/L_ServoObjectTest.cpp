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
/*
	 robot.actions().servoObjects().deploy(SERVO_FUNNY_ACTION, 0);
	 robot.actions().servoObjects().deploy(SERVO_FUNNY_ACTION, -45);
	 robot.actions().servoObjects().deploy(SERVO_FUNNY_ACTION, 0);


	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 0);
	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 75);
	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 0);


	 robot.actions().servoObjects().deploy(SERVO_BASCULE, -100);  // cylindre vertical
	 robot.actions().servoObjects().deploy(SERVO_BASCULE, 80, 2); //cylindre horizontal
	 robot.actions().servoObjects().deploy(SERVO_BASCULE, -100);  // cylindre vertical


	 robot.actions().servoObjects().deploy(SERVO_PINCE, 0);//ouvert
	 robot.actions().servoObjects().deploy(SERVO_PINCE, 45, 2); //fermé
	 robot.actions().servoObjects().deploy(SERVO_PINCE, 0);
*/





	robot.actions().funnyAction_Init();
	robot.actions().pince_Open();
	robot.actions().pince_InitRotation();
	robot.actions().pince_HerculeUp();
	robot.actions().pince_InitRotation();
	robot.actions().pince_HerculeDown();

	robot.actions().pince_Close(0);
	robot.actions().pince_Rotate();
	//sleep(4);
	robot.actions().pince_Open();
	robot.actions().pince_InitRotation();
	robot.actions().funnyAction_Activate();
	robot.actions().funnyAction_Init();
	robot.actions().servoObjects().releaseAll();

	logger().info() << "Happy End." << logs::end;
	robot.stopAll();

}

