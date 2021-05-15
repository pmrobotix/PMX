/*!
 * \file
 * \brief Implémentation de la classe O_TiretteTest.
 */

#include "O_TiretteTest.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"


using namespace std;

void O_TiretteTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    logger().info() << "O_TiretteTest" << logs::end;
    while(true)
    {
        logger().info() << "pressed : " << robot.actions().tirette().pressed() << logs::end;
    }


    logger().info() << "Happy End." << logs::end;
}

