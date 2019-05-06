/*!
 * \file
 * \brief Implémentation de la classe LegoEV3SoundBarTest.
 */

#include "L_ServoObjectTest.hpp"

#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoObjectTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //LEFT or RIGHT
    robot.getArgs().addArgument("m", "move Left or Right");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_ServoObjectTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    Arguments args = robot.getArgs();

    std::string m;

    if (args["m"] != "0") {
        //step = atoi(args["step"].c_str());
        m = args["m"];
        logger().info() << "Arg m set " << args["m"] << ", m = " << m << logs::end;
    }

    logger().info() << "conveyorBelt_Left_low" << logs::end;
    robot.actions().conveyorBelt_Left_low();

    logger().info() << "left_arm_take" << logs::end;
    robot.actions().left_arm_take();

    logger().info() << "left_arm_retract" << logs::end;
    robot.actions().left_arm_retract();

    //2nd time
    logger().info() << "left_arm_take" << logs::end;
    robot.actions().left_arm_take();

    logger().info() << "left_arm_retract" << logs::end;
    robot.actions().left_arm_retract();

    logger().info() << "conveyorBelt_PushRight" << logs::end;
    robot.actions().conveyorBelt_PushRight(1);

    /*
     logger().info() << "conveyorBelt_Left_center" << logs::end;
     robot.actions().conveyorBelt_Left_center();
     */
    robot.actions().releaseAll();

    logger().info() << "Happy End." << logs::end;
    robot.stopAll();

}

