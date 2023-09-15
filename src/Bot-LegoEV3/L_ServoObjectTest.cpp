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

void L_ServoObjectTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    Arguments args = robot.getArgs();

    string action = "";
    if (args["action"] != "0") {
        action = args["action"].c_str();
        logger().info() << "Arg action set " << args["action"] << ", action = " << action << logs::end;
    }

    if (action == "GETALL") {

        robot.actions().init_servos();

        int fb = robot.actions().servos().getPulseWidth(SERVO_7_fUNNY_BACK);
        int ff = robot.actions().servos().getPulseWidth(SERVO_8_fUNNY_FRONT);

        logger().info() << "fb=" << fb << " ff=" << ff << logs::end;
    }

    if (action == "RR") {
        robot.actions().releaseAll();
    }

    if (action == "I") {
        robot.actions().init_servos(1000);
    }

    if (action == "L") {
        robot.actions().launchAR_L();
        robot.actions().launchAR_R();
        utils::sleep_for_micros(200000);
    }

    if (action == "AL") {
        robot.actions().arm_left_deploy(1000);
    }

    if (action == "AR") {
        robot.actions().arm_right_deploy(1000);
    }

    if (action == "FKRHALF") {
        robot.actions().fork_front_right_deploy_half(1000);
    }

    if (action == "FKRH") {
        logger().info() << "FK R HIGH.. " << logs::end;
        robot.actions().fork_front_right_deploy(1000);
    }
    if (action == "FKRI") {
        logger().info() << "FK R INIT.. " << logs::end;
        robot.actions().fork_front_right_init(1000);
    }

    if (action == "FKTEST") {
        logger().info() << "FKTEST.. " << logs::end;
        robot.actions().start();
        robot.actions().fork_test_parallele();

        //utils::sleep_for_micros(000000);

        //robot.actions().servo().stop(true); //TODO a stop pour les servos

    }
    if (action == "FKISLOW") {
        logger().info() << "FK INIT SLOW.. " << logs::end;
        robot.actions().fork_front_right_deploy(0);
        robot.actions().fork_front_left_deploy(0);
        robot.actions().start();
        robot.actions().fork_init_slow(true);
        utils::sleep_for_micros(1000000);
    }

    //FUNNY
    if (action == "FUNA") {
        robot.actions().funny_action_full(1000);
    }
    if (action == "FUNI") {
        robot.actions().funny_action_init(1000);
    }

    robot.actions().stopExtra();

    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;

}

