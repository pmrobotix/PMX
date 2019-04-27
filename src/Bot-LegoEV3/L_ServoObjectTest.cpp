/*!
 * \file
 * \brief Implémentation de la classe LegoEV3SoundBarTest.
 */

#include "L_ServoObjectTest.hpp"

#include <string>

#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoObjectTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    /*
     logger().info() << "conveyorBelt_Left_center" << logs::end;
     robot.actions().conveyorBelt_Left_center();

     logger().info() << "left_arm_retract" << logs::end;
     robot.actions().left_arm_retract();

     logger().info() << "right_arm_retract" << logs::end;
     robot.actions().right_arm_retract();



     logger().info() << "conveyorBelt_Right_low" << logs::end;
     robot.actions().conveyorBelt_Right_low();

     logger().info() << "right_arm_take" << logs::end;
     robot.actions().right_arm_take();

     logger().info() << "right_arm_retract" << logs::end;
     robot.actions().right_arm_retract();

     logger().info() << "conveyorBelt_Left_center" << logs::end;
     robot.actions().conveyorBelt_Left_center();


     */
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

