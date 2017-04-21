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
	 robot.actions().servoObjects().deploy(SERVO_FUNNY_ACTION, -10);
	 robot.actions().servoObjects().deploy(SERVO_FUNNY_ACTION, -70);
	 robot.actions().servoObjects().deploy(SERVO_FUNNY_ACTION, -10);

	 */
	/*
	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 0);
	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 60);
	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 0);
	 */

	/*
	 robot.actions().servoObjects().deploy(SERVO_BASCULE, 70); // vertical
	 sleep(1);
	 robot.actions().servoObjects().deploy(SERVO_BASCULE, -100, 3); //90 degree //horizontal
	 sleep(1);
	 robot.actions().servoObjects().deploy(SERVO_BASCULE, 70);
	 sleep(1);
	 */
	/*
	 robot.actions().servoObjects().deploy(SERVO_PINCE, -15);//ouvert
	 sleep(1);
	 robot.actions().servoObjects().deploy(SERVO_PINCE, 50); //fermé
	 sleep(4);
	 robot.actions().servoObjects().deploy(SERVO_PINCE, -15);
	 sleep(1);*/

	/*
	 * suite des actions

	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 0,2);
	 robot.actions().servoObjects().deploy(SERVO_BASCULE, 80,0); // vertical
	 robot.actions().servoObjects().deploy(SERVO_HERCULE, 85,3);
	 //robot.actions().servoObjects().deploy(SERVO_BASCULE, 90); // vertical
	 robot.actions().servoObjects().deploy(SERVO_PINCE, -15); //ouvert

	 robot.actions().servoObjects().deploy(SERVO_PINCE, 50); //fermé

	 robot.actions().servoObjects().deploy(SERVO_BASCULE, -100, 3); //90 degree //horizontal

	 robot.actions().servoObjects().deploy(SERVO_PINCE, -15); //ouvert


	 robot.actions().servoObjects().deploy(SERVO_BASCULE, 90,2); // vertical
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
	robot.stop();

}

