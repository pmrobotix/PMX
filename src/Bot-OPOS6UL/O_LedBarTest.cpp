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
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();


    robot.chrono().start();
    int wait = 100000;
    robot.actions().ledBar().resetAll();



/*
    robot.actions().ledBar().setOff(0);
    robot.actions().ledBar().setOff(1);
    usleep(wait);
    robot.actions().ledBar().setOn(0);
    robot.actions().ledBar().setOn(1);
    usleep(wait);
    robot.actions().ledBar().setOff(0);
    robot.actions().ledBar().setOff(1);
    usleep(wait);

    robot.actions().ledBar().set(0, LED_ORANGE);
    usleep(wait);
    robot.actions().ledBar().set(0, LED_GREEN);
    usleep(wait);
    robot.actions().ledBar().setOff(0);
    usleep(wait);
*/
    robot.actions().ledBar().blink(5, 100000, LED_ORANGE);

    robot.actions().ledBar().k2mil(4, 100000, LED_GREEN);
/*

    //START ActionTimer
    robot.actions().start(); //start ActionTimer



    robot.actions().ledBar().startSet(0, LED_RED);
    sleep(1);

    robot.actions().ledBar().startAlternate(20, 100000, 0x55, 0xAA, LED_GREEN, true);

    robot.actions().ledBar().startBlinkPin(20, 100000, 0, LED_RED, true);
    sleep(2);

    robot.actions().ledBar().startK2mil(10, 50000, LED_ORANGE, true);
    sleep(1);
    */

    logger().info() << "O_LedBarTest Happy End." << logs::end;
}

