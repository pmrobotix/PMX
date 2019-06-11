#include "O_AsservTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.getArgs().addArgument("d", "dist en mm");
    robot.getArgs().addArgument("a", "angle en degrees", "0");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_AsservTest::run(int argc, char** argv)
{
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    Arguments args = robot.getArgs();

    float d = 0.0;
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }
    float a = 0.0;
    if (args["a"] != "0") {
        a = atof(args["a"].c_str());
        logger().info() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
    }

    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(50);

    logger().info() << "Start Asserv " << logs::end;
    robot.setMyColor(PMXVIOLET);
    robot.asserv().startMotionTimerAndOdo(true); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(0.0, 300.0, -90, (robot.getMyColor() != PMXVIOLET));
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;
    robot.svgPrintPosition();

    logger().info() << "GO distance mm=" << d << logs::end;
    TRAJ_STATE ts = TRAJ_OK;

//    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
//    robot.actions().sensors().setIgnoreBackNearObstacle(false, false, false);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
int f=0;
    while ((ts = robot.asserv().doMoveBackwardTo(robot.asserv().pos_getX_mm(), robot.asserv().pos_getY_mm() + d))
                != TRAJ_FINISHED) {
            robot.svgPrintPosition();
            robot.asserv().displayTS(ts);
            if (ts == TRAJ_NEAR_OBSTACLE) {
                robot.logger().error() << " O_take_gold ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
    //            if (f > 2)
    //                return false;
                f++;

            }
            if (ts == TRAJ_COLLISION) {
                robot.logger().error() << " O_take_gold ===== COLLISION essai n°" << f << logs::end;
    //            if (f >= 1)
    //                return false;
                f++;
            }
            usleep(200000);
            robot.asserv().resetDisplayTS();

        }

    exit(0);

    robot.asserv().resetDisplayTS();
    int c = 0;
    f = 0;
    while ((ts = robot.asserv().doMoveForwardTo(d, 300)) != TRAJ_FINISHED) {

        logger().info() << "Interruption dist TRAJ_STATE=" << ts << logs::end;
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            logger().error() << "===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

            if (f >= 5)
                break; //return
            f++;
            printf("f=%d\n", f);
            usleep(1000000);
        }

        if (ts == TRAJ_COLLISION) {
            logger().error() << "===== COLLISION ASSERV essai n°" << c << logs::end;

            if (c >= 2)
                break; // ou return;
            c++;
            printf("c=%d\n", c);
            usleep(1000000);
        }
        robot.asserv().resetDisplayTS();
    }

    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;
    robot.svgPrintPosition();

    ts = TRAJ_OK;
    c = 0;
    f = 0;
    logger().info() << "GO turn angle=" << a << logs::end;

    while ((ts = robot.asserv().doRotateAbs(a)) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            logger().error() << "===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

            if (f >= 5)
                break; //return
            f++;
            printf("f=%d\n", f);
            usleep(1000000);
        }

        if (ts == TRAJ_COLLISION) {
            logger().error() << "===== COLLISION ASSERV essai n°" << c << logs::end;

            if (c >= 2)
                break; // ou return;
            c++;
            printf("c=%d\n", c);
            usleep(1000000);
        }
        robot.asserv().resetDisplayTS();
    }
//    if ((ts = robot.asserv().doRotateAbs(a)) != TRAJ_OK) {
//        logger().info() << "Interruption turn !! TRAJ_STATE=" << ts << logs::end;
//
//    }

    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;
    sleep(1);
    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;

    robot.svgPrintPosition();

    robot.asserv().freeMotion();
    robot.stopExtraActions();
    logger().info() << "Happy End." << logs::end;
}

