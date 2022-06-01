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
    robot.getArgs().addArgument("action", "action a faire");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_ServoObjectsTest::run(int argc, char** argv) {

    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    Arguments args = robot.getArgs();

    string action = "";
    if (args["action"] != "0") {
        action = args["action"].c_str();
        logger().info() << "Arg action set " << args["action"] << ", action = " << action << logs::end;
    }

    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    if (action == "ALL") {
        //TODO a faire en generique avec boucle for
        robot.actions().servos().release(robot.actions().AX12_SERVO_BRAS_D);
        robot.actions().servos().release(robot.actions().AX12_SERVO_BRAS_G);
        robot.actions().servos().release(robot.actions().AX12_SERVO_ELEVATOR_L);
        robot.actions().servos().release(robot.actions().AX12_SERVO_ELEVATOR_R);
        robot.actions().servos().release(robot.actions().AX12_SERVO_ARM_L_BOTTOM);
        robot.actions().servos().release(robot.actions().AX12_SERVO_ARM_R_BOTTOM);
        robot.actions().servos().release(robot.actions().AX12_SERVO_ARM_L_TOP);
        robot.actions().servos().release(robot.actions().AX12_SERVO_ARM_R_TOP);
        while (1) {
            logger().info() << "AX12_SERVO_BRAS_D       N° "
                    << robot.actions().AX12_SERVO_BRAS_D
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_BRAS_D)
                    << logs::end;
            logger().info() << "AX12_SERVO_BRAS_G       N° "
                    << robot.actions().AX12_SERVO_BRAS_G
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_BRAS_G)
                    << logs::end;
            logger().info() << logs::end;
            logger().info() << "AX12_SERVO_ELEVATOR_L   N° "
                    << robot.actions().AX12_SERVO_ELEVATOR_L
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_ELEVATOR_L)
                    << logs::end;
            logger().info() << "AX12_SERVO_ELEVATOR_R   N° "
                    << robot.actions().AX12_SERVO_ELEVATOR_R
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_ELEVATOR_R)
                    << logs::end;
            logger().info() << logs::end;
            logger().info() << "AX12_SERVO_ARM_L_TOP    N° "
                    << robot.actions().AX12_SERVO_ARM_L_TOP
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_ARM_L_TOP)
                    << logs::end;

            logger().info() << "AX12_SERVO_ARM_L_BOTTOM N° "
                    << robot.actions().AX12_SERVO_ARM_L_BOTTOM
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_ARM_L_BOTTOM)
                    << logs::end;
            logger().info() << logs::end;
            logger().info() << "AX12_SERVO_ARM_R_TOP    N° "
                    << robot.actions().AX12_SERVO_ARM_R_TOP
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_ARM_R_TOP)
                    << logs::end;
            logger().info() << "AX12_SERVO_ARM_R_BOTTOM N° "
                    << robot.actions().AX12_SERVO_ARM_R_BOTTOM
                    << " pos= "
                    << robot.actions().servos().getPulseWidth(robot.actions().AX12_SERVO_ARM_R_BOTTOM)
                    << logs::end;

            logger().info() << logs::end;
            logger().info() << logs::end;
            utils::sleep_for_millis(500);
        }
    }

    if (action == "BD") {
        robot.actions().ax12_bras_droit_init();
        usleep(1000000);
        robot.actions().ax12_bras_droit();
        usleep(1000000);
        robot.actions().ax12_bras_droit_init();
        usleep(1000000);
    }

    if (action == "BG") {
        robot.actions().ax12_bras_gauche_init();
        usleep(1000000);
        robot.actions().ax12_bras_gauche();
        usleep(1000000);
        robot.actions().ax12_bras_gauche_init();
        usleep(1000000);
    }

    if (action == "GOR1") {

        robot.actions().pump_R(false);
        robot.actions().pump_L(false);
        robot.actions().pump_R_electrov(false);
        robot.actions().pump_L_electrov(false);
        robot.actions().elevator2022_init(-1, 500);
        robot.actions().arm_L_side_init();
        robot.actions().arm_R_side_init(-1);

        robot.actions().elevator2022_deploy(-1, 1000);
        sleep(3);
        //prise des palets
        robot.actions().elevator2022_init(-1, 500);

        robot.actions().arm_R_take(-1, 300);

        //niv1
        robot.actions().elevator2022_niv1(-1);

        robot.actions().pump_R(true);

        robot.actions().elevator2022_init(0);

        robot.actions().arm_R_side_init(-1, 100);
        robot.actions().arm_R_deploy1(-1, 100);
        robot.actions().arm_R_deploy2(-1, 100);
        robot.actions().arm_R_deploy3(-1, 100);

        sleep(2);
        robot.actions().pump_R(false);
        robot.actions().pump_R_electrov(true);

    }

    if (action == "GOR2") {

        //prise des palets
        robot.actions().elevator2022_init(-1, 500);

        robot.actions().arm_R_take(-1, 300);

        //niv2
        robot.actions().elevator2022_niv2(-1);

        robot.actions().pump_R(true);

        robot.actions().elevator2022_init(0);

        robot.actions().arm_R_side_init(-1, 100);
        robot.actions().arm_R_deploy1(-1, 100);
        robot.actions().arm_R_deploy2(-1, 100);
        robot.actions().arm_R_deploy3(-1, 100);

        sleep(2);
        robot.actions().pump_R(false);
        robot.actions().pump_R_electrov(true);

    }

    if (action == "GOR3") {

            //prise des palets
            robot.actions().elevator2022_init(-1, 500);

            robot.actions().arm_R_take(-1, 300);

            //niv2
            robot.actions().elevator2022_niv3(-1);

            robot.actions().pump_R(true);

            robot.actions().elevator2022_init(0);

            robot.actions().arm_R_side_init(-1, 100);
            robot.actions().arm_R_deploy1(-1, 100);
            robot.actions().arm_R_deploy2(-1, 100);
            robot.actions().arm_R_deploy3(-1, 100);

            sleep(2);
            robot.actions().pump_R(false);
            robot.actions().pump_R_electrov(true);

        }


    if (action == "RNDW") {
        robot.actions().elevator2022_init(-1, 500);
        robot.actions().elevator2022_deploy(-1, 1000);
        sleep(3);
        robot.actions().elevator2022_init(-1, 500);

    }

    if (action == "NIV0") {
        robot.actions().elevator2022_init(-1);
    }
    //niveau du premier
    if (action == "NIV1") {
        robot.actions().elevator2022_niv1(-1);
    }
    if (action == "NIV2") {
        robot.actions().elevator2022_niv2(-1);
    }
    if (action == "NIV3") {

        robot.actions().elevator2022_niv3(-1);
    }

    if (action == "PUMPR") {

        robot.actions().pump_R(true);
    }

    if (action == "PUMPL") {

        robot.actions().pump_L(true);
    }

    if (action == "POFF") {

        robot.actions().pump_R(false);
        robot.actions().pump_L(false);
    }

    if (action == "ARMR") {
        robot.actions().arm_R_side_init(-1, 100);
        robot.actions().arm_R_take(-1, 300);
        robot.actions().arm_R_side_init(-1, 100);
        robot.actions().arm_R_deploy1(-1, 100);
        robot.actions().arm_R_deploy2(-1, 100);
        robot.actions().arm_R_deploy3(-1, 100);
    }

    /*
     if (action == "D") {
     robot.actions().ax12_drapeaux_init();
     //usleep(1000000);
     robot.actions().ax12_drapeaux();
     //usleep(1000000);
     }



     if (action == "UP") {
     robot.actions().ax12_elevator_up(false);


     }
     if (action == "DW") {
     robot.actions().ax12_elevator_down(false);

     }

     if (action == "RN") {
     robot.actions().ax12_elevator_up(true);
     robot.actions().ax12_rotation_black_side(true);
     robot.actions().ax12_elevator_up(false); //release elevator

     }
     if (action == "RNDW") {
     robot.actions().ax12_elevator_up(true);
     robot.actions().ax12_rotation_black_side(true);
     robot.actions().ax12_elevator_down(true);

     }

     if (action == "RB") {
     robot.actions().ax12_elevator_up(true);
     robot.actions().ax12_rotation_blue_side(true);

     }

     if (action == "P") {
     robot.actions().ax12_pince_au_centre(true);
     robot.actions().ax12_pince_a_droite(true);
     robot.actions().ax12_pince_a_gauche(true);
     robot.actions().ax12_pince_au_centre(false);
     //usleep(1000000);
     }
     */

    if (action == "RR") {
        robot.actions().releaseAll();
    }
    /*
     robot.actions().servosAx12().detect();
     robot.actions().ax12_left_cil_retract(0);
     robot.actions().ax12_right_cil_retract();

     robot.actions().ax12_rightHand_retract();
     robot.actions().ax12_leftHand_retract();

     robot.actions().ax12_left_cil(0);
     robot.actions().ax12_right_cil();

     robot.actions().ax12_rightHand();
     robot.actions().ax12_leftHand();

     logger().info() << "goldenium = " << robot.actions().ax12_goldenium_in_cil() << logs::end;

     sleep(3);
     logger().info() << "goldenium = " << robot.actions().ax12_goldenium_in_cil() << logs::end;

     robot.actions().ax12_left_cil_retract(0);
     robot.actions().ax12_right_cil_retract();

     robot.actions().ax12_rightHand_retract(0);
     robot.actions().ax12_leftHand_retract();
     */
    //robot.stopExtraActions();
    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;

}

