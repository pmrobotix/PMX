#include "O_ServoStepTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_ServoStepTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.getArgs().addArgument("num", "Numero du servo");
    robot.getArgs().addArgument("step", "nombre à augmenter ou diminuer (en %)", "2");
    robot.getArgs().addArgument("pos", "position initiale", "512");
    robot.getArgs().addArgument("speed", "position initiale", "1023");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_ServoStepTest::run(int argc, char** argv)
{

    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    Arguments args = robot.getArgs();


    ServoObjectsSystem servoObjects = robot.actions().servosAx12();


    logger().info() << "N°" << this->position() << " - Executing - " << this->desc() << logs::end;


    int pos = 512;
    int step = 5;
    int speed = 1023;
    int num = 0;

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

    if (args["speed"] != "0") {
        speed = atoi(args["speed"].c_str());
        logger().info() << "Arg speed set " << args["speed"] << ", speed = " << speed << logs::end;
    }

    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().home();
    robot.actions().lcd2x16().print("SERVO n°");
    robot.actions().lcd2x16().print(num);

    servoObjects.setSpeed(num, speed);

    servoObjects.deploy(num, pos, 0);

    ButtonTouch touch = BUTTON_NONE;

    while (touch != BUTTON_BACK_KEY) {
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("SERVO ");
        robot.actions().lcd2x16().print(num);
        robot.actions().lcd2x16().print(" ");
        robot.actions().lcd2x16().print(pos);

        touch = robot.actions().buttonBar().waitOneOfAllPressed();
        //logger().info() << "touch = " << touch << logs::end;
        if (touch == BUTTON_UP_KEY) {
            pos += step;
            if (pos >= 1023)
                pos = 1023;
            logger().info() << "+" << step << " pos=" << pos << logs::end;
            servoObjects.deploy(num, pos, -1);
        }

        if (touch == BUTTON_DOWN_KEY) {
            pos -= step;
            if (pos <= 0)
                pos = 0;
            logger().info() << "-" << step << " pos=" << pos << logs::end;
            servoObjects.deploy(num, pos, -1);

        }

        if (touch == BUTTON_RIGHT_KEY) {
            pos = 512;
            servoObjects.release(num);
            num = num + 1;
            if (num >= 254)
                num--;

            logger().info() << "SERVO " << num << " pos=" << pos << logs::end;
            usleep(20000);
        }

        if (touch == BUTTON_LEFT_KEY) {
            pos = 512;
            servoObjects.release(num);
            num = num - 1;
            if (num < 0)
                num++;

            logger().info() << "SERVO " << num << " pos=" << pos << logs::end;
            usleep(20000);
        }
        usleep(10000);
    }
    logger().info() << "RELEASE ALL " << logs::end;
    robot.actions().releaseAll();

    logger().info() << "Happy End." << logs::end;
}

