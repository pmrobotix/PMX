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

    /*
    if (action == "D") {
        robot.actions().ax12_drapeaux_init();
        //usleep(1000000);
        robot.actions().ax12_drapeaux();
        //usleep(1000000);
    }

    if (action == "BD") {
        robot.actions().ax12_bras_droit_init();
        usleep(1000000);
        robot.actions().ax12_bras_droit();
        usleep(1000000);
        robot.actions().ax12_bras_droit_init();
    }

    if (action == "BG") {
        robot.actions().ax12_bras_gauche_init();
        usleep(1000000);
        robot.actions().ax12_bras_gauche();
        usleep(1000000);
        robot.actions().ax12_bras_gauche_init();
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

