#include "L_LcdTest.hpp"

#include <string>

#include "../Common/Action/LcdShield.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_LcdTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.actions().lcd().clear();
    robot.actions().lcd().display_content_string("test", 1);

    int p = 100;
    //sleep(1);
    robot.actions().lcd().setCursor(1,1); //to change the font or home()
    robot.actions().lcd().display_content_string("PM-ROBOTIX Go !", 4);
    robot.actions().lcd().display_content_string("points =", 6);
    robot.actions().lcd().display_content_integer(p, 6, 10);
    sleep(3); //to have time to see it on the EV3
    //robot.actions().lcd().setCursor(1,1); //to change the font

    //logger().debug() << "Happy End." << logs::end;
}

