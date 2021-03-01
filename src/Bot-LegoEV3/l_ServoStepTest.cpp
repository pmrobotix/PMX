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
    robot.getArgs().addArgument("step", "nombre à augmenter ou diminuer (en %)", "2");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_ServoStepTest::run(int argc, char** argv)
{

    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    //args.usage();

    int pos = 0;
    int step = 2;

    int num = 0;

    if (args["num"] != "0") {
        num = atoi(args["num"].c_str());
        logger().info() << "Arg num set " << args["num"] << ", num = " << num << logs::end;
    }

    if (args["step"] != "0") {
        step = atoi(args["step"].c_str());
        logger().info() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
    }

    robot.actions().servosStd().setMinPulse(num, 0); ////default 600 [300 to 700]
    robot.actions().servosStd().setMidPulse(num, 1500); //default 1500 [1300 to 1700]
    robot.actions().servosStd().setMaxPulse(num, 3000); //default 2400 [2300 to 2700]

    robot.actions().servosStd().hold(num);

    ButtonTouch touch = BUTTON_NONE;

    while (touch != BUTTON_BACK_KEY) {
        touch = robot.actions().buttonBar().waitOneOfAllPressed();
        //logger().info() << "touch = " << touch << logs::end;
        if (touch == BUTTON_UP_KEY) {
            pos += step;
            if (pos >= 100)
                pos = 100;
            logger().info() << "+" << step << " pos=" << pos << logs::end;
            robot.actions().servosStd().deploy(num, pos, 0);
            //robot.actions().servoObjects().deploy((ServoLabel) num, pos, 0);

        }

        if (touch == BUTTON_DOWN_KEY) {
            pos -= step;
            if (pos <= -100)
                pos = -100;
            logger().info() << "-" << step << " pos=" << pos << logs::end;
            robot.actions().servosStd().deploy(num, pos, 0);
            //robot.actions().servoObjects().deploy((ServoLabel) num, pos, 0);

        }

        if (touch == BUTTON_ENTER_KEY) {

            //robot.actions().servoObjects().release((ServoLabel) num);
            robot.actions().servosStd().release(num);

            switch(step)
            {
                case 1:
                    step = 2; break;
                case 2:
                    step = 5; break;
                case 5:
                    step = 10; break;
                case 10:
                    step = 15; break;
                case 15:
                    step = 20; break;
                case 20:
                    step = 1; break;
                default:
                    step = 1;
            }
            logger().info() << "-" << step << " pos=" << pos << logs::end;
            usleep(200000);
        }

        if (touch == BUTTON_RIGHT_KEY) {
            pos = 0;
            robot.actions().servosStd().release(num);
            //robot.actions().servoObjects().release((ServoLabel) num);
            num = num + 1;
            if (num >= SERVO_enumTypeEnd)
                num--;

            robot.actions().servosStd().hold(num);

            robot.actions().servosStd().setMinPulse(num, 0);
            robot.actions().servosStd().setMidPulse(num, 1500);
            robot.actions().servosStd().setMaxPulse(num, 3000);
            robot.actions().servosStd().setSpeed(num, 100);

            logger().info() << "SERVO " << num << " pos=" << pos << logs::end;
            usleep(200000);
        }

        if (touch == BUTTON_LEFT_KEY) {
            pos = 0;
            robot.actions().servosStd().release(num);
            //robot.actions().servoObjects().release((ServoLabel) num);
            num = num - 1;
            if (num < 0)
                num++;

            robot.actions().servosStd().hold(num);

            robot.actions().servosStd().setMinPulse(num, 0);
            robot.actions().servosStd().setMidPulse(num, 1500);
            robot.actions().servosStd().setMaxPulse(num, 3000);

            logger().info() << "SERVO " << num << " pos=" << pos << logs::end;
            usleep(200000);
        }

        usleep(100000);
    }

    robot.actions().releaseAll();

    logger().info() << "Happy End." << logs::end;
}

