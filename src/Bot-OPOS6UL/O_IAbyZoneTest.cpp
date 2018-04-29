/*!
 * \file
 * \brief Implémentation de la classe O_IAByZoneTest.
 */

#include "O_IAbyZoneTest.hpp"

#include <string>

#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_IAByZoneTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    logger().info() << "Happy End - " << robot.chrono().getElapsedTimeInSec() << " sec" << logs::end;
}
