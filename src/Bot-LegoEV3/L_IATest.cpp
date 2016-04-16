/*!
 * \file
 * \brief Implémentation de la classe L_IATest.
 */

#include "L_IATest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_IATest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//robot.ia_->iaz_.ia_start();//launch IA


	robot.stop();

	logger().info() << "Happy End." << logs::end;
}

