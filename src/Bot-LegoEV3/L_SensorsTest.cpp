#include "L_SensorsTest.hpp"

#include <unistd.h>
#include <string>
#include <cmath>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_SensorsTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.setMyColor(PMXYELLOW);
    robot.asserv().startMotionTimerAndOdo(false); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(1000.0, 1000.0, 0.0, (robot.getMyColor() != PMXYELLOW)); //pour mettre une position dans la table
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;

    int front = 0, back = 0;
    utils::Chronometer chrono("L_SensorsTest");
    chrono.start();
    /*
     while (chrono.getElapsedTimeInSec() < 100) {
     front = robot.actions().sensors().front(true);
     back = robot.actions().sensors().back(true);

     usleep(400000);
     logger().info() << " front=" << front << " back=" << back << logs::end;
     }
     */

    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(200);

    while (chrono.getElapsedTimeInSec() < 200) {
        front = robot.actions().sensors().front(true);
        back = robot.actions().sensors().back(true);

        usleep(1000000);
        logger().info() << " front=" << front << " back=" << back << logs::end;
    }

    logger().info() << "Happy End." << logs::end;
}

