/*!
 * \file
 * \brief Implémentation de la classe LedBarTest.
 */

#include "O_LedBarTest.hpp"

#include <string>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Common/Utils/json.hpp"
#include "../Log/Logger.hpp"
#include "../Thread/Thread.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_LedBarTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    int wait = 2000000;

    robot.chrono().start();


    robot.actions().ledBar().resetAll();

    //test avec asserv
    robot.asserv().startMotionTimerAndOdo(false);

    robot.actions().ledBar().setOff(0);
    robot.actions().ledBar().setOff(1);
    robot.actions().ledBar().setOn(0);
    robot.actions().ledBar().setOn(1);
    utils::sleep_for_micros(wait);
    robot.actions().ledBar().setOff(0);
    robot.actions().ledBar().setOff(1);


    robot.actions().ledBar().set(0, LED_ORANGE);
    robot.actions().ledBar().set(0, LED_GREEN);
    robot.actions().ledBar().setOff(0);

    robot.actions().ledBar().blink(5, 100000, LED_ORANGE);

    robot.actions().ledBar().k2mil(4, 100000, LED_GREEN);

    robot.actions().ledBar().resetAll();


//    //telemetry log
//        nlohmann::json j;
//        j["pi"] = 3.14;
//        logger().telemetry() << j.dump() << logs::end;
//    logger().error() << "errorSTARTEEE ActionTimer" << logs::end;


    //START ActionTimer
    robot.actions().start(); //start ActionTimer
    logger().info() << "robot.actions().started" << logs::end;

    logger().info() << "startSet" << logs::end;
    int i = 0;
    while (i < 20) {
        robot.actions().ledBar().startSet(0, LED_RED);
        robot.actions().ledBar().startSet(1, LED_RED);
        robot.actions().ledBar().startSet(2, LED_RED);
        robot.actions().ledBar().startSet(3, LED_RED);
        robot.actions().ledBar().startSet(4, LED_RED);
        robot.actions().ledBar().startSet(5, LED_RED);
        robot.actions().ledBar().startSet(6, LED_RED);
        robot.actions().ledBar().startSet(7, LED_RED);
        utils::sleep_for_micros(100000);
        robot.actions().ledBar().startSet(0, LED_BLACK);
        robot.actions().ledBar().startSet(1, LED_BLACK);
        robot.actions().ledBar().startSet(2, LED_BLACK);
        robot.actions().ledBar().startSet(3, LED_BLACK);
        robot.actions().ledBar().startSet(4, LED_BLACK);
        robot.actions().ledBar().startSet(5, LED_BLACK);
        robot.actions().ledBar().startSet(6, LED_BLACK);
        robot.actions().ledBar().startSet(7, LED_BLACK);
        i++;
        utils::sleep_for_micros(100000);
    }

    logger().info() << "startAlternate" << logs::end;
    robot.actions().ledBar().startTimerAlternate(10, 500000, 0x55, 0xAA, LED_GREEN, true);

    robot.actions().ledBar().startTimerAlternate(10, 500000, 0x55, 0xAA, LED_ORANGE, false);
    utils::sleep_for_micros(2000000);

    robot.actions().ledBar().startTimerAlternate(10, 200000, 0x55, 0xAA, LED_ORANGE, false);
    utils::sleep_for_micros(1000000);
    robot.actions().ledBar().stop(true);

    logger().info() << "startBlinkPin" << logs::end;
    robot.actions().ledBar().startTimerBlinkPin(10, 300000, 0, LED_RED, true);

    robot.actions().ledBar().resetAll();
    logger().info() << "startK2mil" << logs::end;
    robot.actions().ledBar().startTimerK2mil(4, 100000, LED_GREEN, true);

    logger().info() << "startsetx10" << logs::end;
    i = 0;
    while (i < 10) {
        robot.actions().ledBar().startSet(0, LED_RED);
        robot.actions().ledBar().startSet(1, LED_RED);
        robot.actions().ledBar().startSet(2, LED_RED);
        robot.actions().ledBar().startSet(3, LED_RED);
        robot.actions().ledBar().startSet(4, LED_RED);
        robot.actions().ledBar().startSet(5, LED_RED);
        robot.actions().ledBar().startSet(6, LED_RED);
        robot.actions().ledBar().startSet(7, LED_RED);
        utils::sleep_for_micros(100000);
        robot.actions().ledBar().startSet(0, LED_BLACK);
        robot.actions().ledBar().startSet(1, LED_BLACK);
        robot.actions().ledBar().startSet(2, LED_BLACK);
        robot.actions().ledBar().startSet(3, LED_BLACK);
        robot.actions().ledBar().startSet(4, LED_BLACK);
        robot.actions().ledBar().startSet(5, LED_BLACK);
        robot.actions().ledBar().startSet(6, LED_BLACK);
        robot.actions().ledBar().startSet(7, LED_BLACK);
        i++;
        utils::sleep_for_micros(100000);
    }

    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;
}

