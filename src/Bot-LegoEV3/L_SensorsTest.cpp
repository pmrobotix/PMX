#include "L_SensorsTest.hpp"

#include <cmath>
#include <string>
#include <vector>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Interface.Driver/ASensorsDriver.hpp"
#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "../Thread/Thread.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_SensorsTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.setMyColor(PMXGREEN);
    robot.asserv().startMotionTimerAndOdo(false); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(1000.0, 800.0, 90.0, (robot.getMyColor() != PMXGREEN)); //pour mettre une position dans la table
    ROBOTPOSITION p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x << " " << p.y << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus
            << logs::end;
    robot.svgPrintPosition();

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

    while (chrono.getElapsedTimeInSec() < 60) {

        //Positions de l'adv sur le repere robot
        vadv = robot.actions().sensors().getPositionsAdv();

        p = robot.asserv().pos_getPosition();

        for (ASensorsDriver::bot_positions::size_type i = 0; i < vadv.size(); i++) {
            logger().info() << " vadv nb=" << vadv.size() << " detected=" << vadv[i].nbDetectedBots << " x="
                    << vadv[i].x << " y=" << vadv[i].y << " a_deg=" << vadv[i].theta_deg << " d=" << vadv[i].d
                    << logs::end;

            float x_pos_adv_table = 0;
            float y_pos_adv_table = 0;

            //transcritption du repere robot vers repere table
            robot.sharedPosition()->convertPositionBeaconToRepereTable(vadv[i].d,
                    vadv[i].x, vadv[i].y, vadv[i].theta_deg, &x_pos_adv_table, &y_pos_adv_table);
            robot.svgw().writePosition_AdvPos(x_pos_adv_table, y_pos_adv_table, p.x, p.y, 4); //4= BLUE
        }

        front = robot.actions().sensors().front(true);
        back = robot.actions().sensors().back(true);

        utils::sleep_for_micros(1000000);
        logger().info() << " front=" << front << " back=" << back << logs::end;
    }



    logger().info() << "Happy End." << logs::end;
}

