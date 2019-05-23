#include "O_ServoObjectsTest.hpp"

#include <unistd.h>
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

    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;

    robot.actions().servosAx12().detect();
    robot.actions().ax12_left_cil_retract(0);
    robot.actions().ax12_right_cil_retract();

    robot.actions().ax12_left_cil(0);
    robot.actions().ax12_right_cil();

    logger().info() << "goldenium = " <<robot.actions().ax12_goldenium_in_cil() << logs::end;

    sleep(3);
    logger().info() << "goldenium = " <<robot.actions().ax12_goldenium_in_cil() << logs::end;



    robot.actions().ax12_left_cil_retract(0);
    robot.actions().ax12_right_cil_retract();


    robot.stopExtraActions();
    logger().info() << "Happy End." << logs::end;
}

