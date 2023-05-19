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

void L_SensorsTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.setMyColor(PMXGREEN);
    robot.asserv().startMotionTimerAndOdo(false); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(400.0, 200.0, 90.0, (robot.getMyColor() != PMXGREEN)); //pour mettre une position dans la table
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x << " " << p.y << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus
            << logs::end;

    int front = 0, back = 0;
    ASensorsDriver::bot_positions vadv;

    utils::Chronometer chrono("L_SensorsTest");
    chrono.start();
    /*
     while (chrono.getElapsedTimeInSec() < 100) {
     front = robot.actions().sensors().front(true);
     back = robot.actions().sensors().back(true);

     utils::sleep_for_micros(4000000);
     logger().info() << " front=" << front << " back=" << back << logs::end;
     }
     */

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(200);

    bool alive = robot.actions().sensors().is_connected();
    if (!alive)
        logger().error() << "!!!!!!!! NON alive =" << alive << logs::end;
    else
        logger().info() << "alive =" << alive << logs::end;

//    robot.actions().arm_right_init();
//    robot.actions().arm_left_init(1000);
//
//    robot.actions().arm_right_deploy();
//    robot.actions().arm_left_deploy(1000);

    while (chrono.getElapsedTimeInSec() < 200) {

        vadv = robot.actions().sensors().getPositionsAdv();

        for (ASensorsDriver::bot_positions::size_type i = 0; i < vadv.size(); i++) {
            logger().info() << " vadv nb=" << vadv.size() << " detected=" << vadv[i].nbDetectedBots << " x="
                    << vadv[i].x << " y=" << vadv[i].y << " a_deg=" << vadv[i].theta_deg << " d=" << vadv[i].d
                    << logs::end;
        }

        front = robot.actions().sensors().front(true);
        back = robot.actions().sensors().back(true);

        //default=>510
        //4,7k =>462
        //1k=>340
        //470=>246
        //int adc = robot.actions().sensors().getADC();

        utils::sleep_for_micros(1000000);
        logger().info() << " front=" << front << " back=" << back << logs::end;
    }

    logger().info() << "Happy End." << logs::end;
}

