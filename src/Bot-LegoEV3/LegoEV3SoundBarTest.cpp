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

void LegoEV3SoundBarTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.actions().soundBar().beep(5);

	robot.actions().soundBar().speakPMX(50);


	robot.actions().start();

	robot.actions().soundBar().startBeep(25);
	robot.actions().soundBar().startBeep(50);

	sleep(1);

	robot.actions().stop();

	logger().info() << this->name() << " - Happy End." << logs::end;
	robot.actions().soundBar().beep(5);
	robot.stop();

}

