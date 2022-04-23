#include "l_ServoStepTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoStepTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.getArgs().addArgument("num", "Numero du servo", "0");
    robot.getArgs().addArgument("step", "increment/decrement", "2");
    robot.getArgs().addArgument("pos", "position initiale [0-3000]", "1500");
    robot.getArgs().addArgument("type", "Std[0] or AX12[12]", "0");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_ServoStepTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    //args.usage();

    int pos = 0;
    int step = 0;
    int num = 0;
    int type = 0;

    if (args["num"] != "0") {
        num = atoi(args["num"].c_str());
        logger().info() << "Arg num set " << args["num"] << ", num = " << num << logs::end;
    }

    if (args["step"] != "0") {
        step = atoi(args["step"].c_str());
        logger().info() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
    }

    if (args["pos"] != "0") {
        pos = atoi(args["pos"].c_str());
        logger().info() << "Arg pos set " << args["pos"] << ", pos = " << pos << logs::end;
    }

    if (args["type"] != "0") {
        type = atoi(args["type"].c_str());
        logger().info() << "Arg type set " << args["type"] << ", type = " << type << logs::end;
    }

    int pos_default = pos;

    robot.actions().servos().hold(num);

    ButtonTouch touch = BUTTON_NONE;

    AServoDriver::ServoType aType = AServoDriver::SERVO_STANDARD;
    if(type == 12)
        aType = AServoDriver::SERVO_DYNAMIXEL;

    robot.actions().servos().setup(num, aType, 0, 1500, 3000, false);
    while (touch != BUTTON_BACK_KEY) {


        touch = robot.actions().buttonBar().waitOneOfAllPressed();
        if (touch == BUTTON_UP_KEY) {
            pos += step;

            logger().info() << "+" << step << " pos=" << pos << logs::end;
            robot.actions().servos().deploy(num, pos, 0);

        }

        if (touch == BUTTON_DOWN_KEY) {
            pos -= step;

            logger().info() << "-" << step << " pos=" << pos << logs::end;
            robot.actions().servos().deploy(num, pos, 0);
        }

        if (touch == BUTTON_ENTER_KEY) {

            robot.actions().servos().release(num);

            switch (step) {
                case 1:
                    step = 2;
                    break;
                case 2:
                    step = 5;
                    break;
                case 5:
                    step = 10;
                    break;
                case 10:
                    step = 15;
                    break;
                case 15:
                    step = 20;
                    break;
                case 20:
                    step = 50;
                    break;
                default:
                    step = 1;
            }
            logger().info() << "-" << step << " pos=" << pos << logs::end;
            //usleep(200000);
        }

        if (touch == BUTTON_RIGHT_KEY) {
            pos = pos_default;
            robot.actions().servos().release(num);

            num = num + 1;
            if (num >= SERVO_enumTypeEnd) num--;

            robot.actions().servos().setup(num, aType, 0, 1500, 3000, false);
            robot.actions().servos().hold(num);

            logger().info() << "SERVO " << num << " pos=" << pos << logs::end;
            //usleep(200000);
        }

        if (touch == BUTTON_LEFT_KEY) {
            pos = pos_default;
            robot.actions().servos().release(num);
            num = num - 1;
            if (num < 0) num++;

            robot.actions().servos().setup(num, aType, 0, 1500, 3000, false);
            robot.actions().servos().hold(num);

            logger().info() << "SERVO " << num << " pos=" << pos << logs::end;
            //usleep(200000);
        }

        usleep(100000);
    }

    robot.actions().releaseAll();

    logger().info() << "Happy End." << logs::end;
}

