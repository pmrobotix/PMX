/*!
 * \file
 * \brief Implémentation de la classe LegoEV3SoundBarTest.
 */

#include "LegoEV3SoundBarTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/SoundBar.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3SoundBarTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	logger().info() << "BEEP" << logs::end;
	robot.actions().soundBar().beep();

	logger().info() << "speakPMX" << logs::end;
	robot.actions().soundBar().speakPMX();


	robot.actions().start();

	logger().info() << "BEEP BEEP" << logs::end;
	robot.actions().soundBar().startBeep();
	robot.actions().soundBar().startBeep();

	sleep(1);

	robot.actions().stop();

	logger().info() << "ImperialMarchDarthVader" << logs::end;
	robot.actions().soundBar().toneImperialMarchDarthVader();

	logger().info() << "BEEP Happy End." << logs::end;
	robot.actions().soundBar().beep();
	robot.stop();

}

