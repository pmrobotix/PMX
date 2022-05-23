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

void L_ServoObjectTest::configureConsoleArgs(int argc, char **argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.getArgs().addArgument("action", "action a faire");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_ServoObjectTest::run(int argc, char **argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    Arguments args = robot.getArgs();

    string action = "";
    if (args["action"] != "0") {
        action = args["action"].c_str();
        logger().info() << "Arg action set " << args["action"] << ", action = " << action << logs::end;
    }

    if (action == "RR") {
        robot.actions().releaseAll();
    }

    if (action == "I") {
        robot.actions().init_servos();
    }

    if (action == "SQ") {
        robot.actions().square_push_right(1500);
        robot.actions().square_middle_init(1500);
        robot.actions().square_push_right(1500);
        robot.actions().square_middle_init(1500);
    }
    if (action == "CU") {
        logger().info() << "init_mettre_cube" << logs::end;
        robot.actions().init_mettre_cube();

        sleep(4);

        logger().info() << "cube_push_and_take_trophy" << logs::end;
        robot.actions().cube_push_and_take_trophy(0);

        sleep(2);
        robot.actions().cube_normal_pos(4000); //depose
    }

    if (action == "AL") {
        robot.actions().arm_left_deploy(2000);
    }

    if (action == "AR") {
        robot.actions().arm_right_deploy(2000);
    }

    /*
     logger().info() << "init_servo" << logs::end;

     sleep(1);

     //TEST SQUARE
     robot.actions().square_push_right(1500);
     robot.actions().square_middle_init(1500);
     robot.actions().square_push_right(1500);
     robot.actions().square_middle_init(1500);

     //TEST CUBE
     logger().info() << "init_mettre_cube" << logs::end;
     robot.actions().init_mettre_cube();

     sleep(4);

     logger().info() << "cube_push_and_take_trophy" << logs::end;
     robot.actions().cube_push_and_take_trophy(0);

     sleep(2);
     robot.actions().cube_normal_pos(4000);//depose
     */
//
//
//    robot.actions().init_servos();
//    robot.actions().releaseAll();
    /*
     logger().info() << "conveyorBelt_Left_center" << logs::end;
     robot.actions().conveyorBelt_Left_center();
     logger().info() << "left_arm_retract" << logs::end;
     robot.actions().left_arm_retract();
     logger().info() << "right_arm_retract" << logs::end;
     robot.actions().right_arm_retract();

     int nb = 1;
     if (m == "ALLR" || m == "ALLL")
     nb = 3;

     for (int i = 1; i <= nb; i++) {
     if (m == "L" || m == "ALLL") {

     logger().info() << "conveyorBelt_Left_low" << logs::end;
     robot.actions().conveyorBelt_Left_low();

     robot.actions().left_prendre_palet(2100, 1, true);


     //            logger().info() << "left_arm_take" << logs::end;
     //            robot.actions().left_arm_take();
     //
     //            logger().info() << "left_arm_retract" << logs::end;
     //            robot.actions().left_arm_retract();
     //
     //            //2nd time
     //            logger().info() << "left_arm_take" << logs::end;
     //            robot.actions().left_arm_take();
     //
     //            logger().info() << "left_arm_retract" << logs::end;
     //            robot.actions().left_arm_retract();
     //
     //            logger().info() << "conveyorBelt_PushRight" << logs::end;
     //
     //            if (i == 3)
     //                robot.actions().conveyorBelt_PushRight(200);
     //            else
     //                robot.actions().conveyorBelt_PushRight(1200);

     } else if (m == "R" || m == "ALLR") {

     logger().info() << "conveyorBelt_Right_low" << logs::end;
     robot.actions().conveyorBelt_Right_low();

     logger().info() << "right_arm_take" << logs::end;
     robot.actions().right_arm_take();

     logger().info() << "right_arm_retract" << logs::end;
     robot.actions().right_arm_retract();

     //2nd time
     logger().info() << "right_arm_take" << logs::end;
     robot.actions().right_arm_take();

     logger().info() << "right_arm_retract" << logs::end;
     robot.actions().right_arm_retract();

     logger().info() << "conveyorBelt_PushLeft" << logs::end;
     if (i == 3)
     robot.actions().conveyorBelt_PushLeft(200);
     else
     robot.actions().conveyorBelt_PushLeft(1200);
     }
     sleep(2);
     }
     if (m == "ER" || m == "ALLR") {

     robot.actions().right_eject_all(3000);

     //         logger().info() << "conveyorBelt_Left_low" << logs::end;
     //         robot.actions().conveyorBelt_Left_low();
     //         logger().info() << "conveyorBelt_EjectRight" << logs::end;
     //         robot.actions().conveyorBelt_EjectRight(3);
     } else if (m == "EL" || m == "ALLL") {

     robot.actions().left_eject_all(3000);

     //         logger().info() << "conveyorBelt_Right_low" << logs::end;
     //         robot.actions().conveyorBelt_Right_low();
     //         logger().info() << "conveyorBelt_EjectLeft" << logs::end;
     //         robot.actions().conveyorBelt_EjectLeft(3);
     }

     if (m == "DR") {
     logger().info() << "conveyorBelt_Left_low" << logs::end;
     robot.actions().conveyorBelt_Right_low();
     logger().info() << "conveyorBelt_EjectRight" << logs::end;
     robot.actions().conveyorBelt_EjectRight(3);
     }

     if (m == "DL") {
     logger().info() << "conveyorBelt_Left_low" << logs::end;
     robot.actions().conveyorBelt_Left_low();
     logger().info() << "conveyorBelt_EjectLeft" << logs::end;
     robot.actions().conveyorBelt_EjectLeft(3);
     }

     logger().info() << "conveyorBelt_Left_center" << logs::end;
     robot.actions().conveyorBelt_Left_center();

     robot.actions().releaseAll();
     */

    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;

}

