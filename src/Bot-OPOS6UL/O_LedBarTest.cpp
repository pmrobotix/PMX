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
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_LedBarTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();


    robot.chrono().start();
    int wait = 100000;
    robot.actions().ledBar().resetAll();

    //test avec asserv
    robot.asserv().startMotionTimerAndOdo(false);


    robot.actions().ledBar().setOff(0);
    robot.actions().ledBar().setOff(1);
    utils::sleep_for_micros(wait);
    robot.actions().ledBar().setOn(0);
    robot.actions().ledBar().setOn(1);
    utils::sleep_for_micros(wait);
    robot.actions().ledBar().setOff(0);
    robot.actions().ledBar().setOff(1);
    utils::sleep_for_micros(wait);


    robot.actions().ledBar().set(0, LED_ORANGE);
    utils::sleep_for_micros(wait);
    robot.actions().ledBar().set(0, LED_GREEN);
    utils::sleep_for_micros(wait);
    robot.actions().ledBar().setOff(0);
    utils::sleep_for_micros(wait);

    robot.actions().ledBar().blink(5, 100000, LED_ORANGE);

    robot.actions().ledBar().k2mil(4, 100000, LED_GREEN);




    //TODO corriger le problème du Wait !!


    //START ActionTimer
    robot.actions().start(); //start ActionTimer
    logger().info() << "robot.actions().started" << logs::end;

    logger().info() << "startSet" << logs::end;
    robot.actions().ledBar().startSet(0, LED_RED);

    utils::sleep_for_micros(1);
    robot.actions().ledBar().startSet(1, LED_RED);
       robot.actions().ledBar().startSet(2, LED_RED);
       utils::sleep_for_micros(1);

    logger().info() << "startAlternate" << logs::end;
    robot.actions().ledBar().startAlternate(20, 1000000, 0x55, 0xAA, LED_GREEN, true);
    //sleep(2);

    logger().info() << "startBlinkPin" << logs::end;
    robot.actions().ledBar().startBlinkPin(20, 100000, 0, LED_RED, true);
    //sleep(2);

    logger().info() << "startK2mil" << logs::end;
    robot.actions().ledBar().startK2mil(40, 50000, LED_ORANGE, true);
    //sleep(2);

    logger().info() << robot.getID() << " " << this->name() << " Happy End." << logs::end;
}

