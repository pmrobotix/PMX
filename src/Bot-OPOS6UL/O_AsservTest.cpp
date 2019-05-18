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
    logger().info() << "Executing - " << this->desc() << logs::end;
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
    robot.actions().sensors().addTimerSensors();

    logger().info() << "Start Asserv " << logs::end;
    robot.setMyColor(PMXVIOLET);
    robot.asserv().startMotionTimerAndOdo(true); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;
    robot.svgPrintPosition();

    //robot.asserv().freeMotion();
    //robot.asserv().assistedHandling();

    logger().info() << "GO distance mm=" << d << logs::end;
    TRAJ_STATE ts = TRAJ_OK;

    //TEST GO x y
    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().setLowSpeed(false); //au cas où par les sensors (si pas de ARU)

    robot.asserv().resetDisplayTS();
    int c = 0;
    int f = 0;
    while ((ts = robot.asserv().doMoveForwardTo(d, 300)) != TRAJ_OK) {

        logger().info() << "Interruption dist TRAJ_STATE=" << ts << " front=" << robot.actions().sensors().front()
        << " frontV=" << robot.actions().sensors().frontVeryClosed()
                << logs::end;
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);
        if (ts == TRAJ_NEAR_OBSTACLE) {
            logger().error() << "===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

            if (f >= 5)
                break; //return
            f++;
            //usleep(100000);
        }

        if (ts == TRAJ_COLLISION) {
            logger().error() << "===== COLLISION ASSERV essai n°" << c << logs::end;

            if (c >= 1)
                break; // ou return;
            c++;
            usleep(100000);
        }
        robot.asserv().resetDisplayTS();
    }

    //Remettre le doLine
    //ts = robot.asserv().doLineAbs(d);
    /*
     p = robot.asserv().pos_getPosition();
     logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
     << p.asservStatus << logs::end;
     robot.svgPrintPosition();

     ts = TRAJ_OK;
     logger().info() << "GO turn angle=" << a << logs::end;
     if ((ts = robot.asserv().doRotateAbs(a)) != TRAJ_OK) {
     logger().info() << "Interruption turn !! TRAJ_STATE=" << ts << logs::end;

     }
     */
    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;
    sleep(1);
    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;

    robot.svgPrintPosition();

    robot.asserv().freeMotion();

    logger().info() << "Happy End." << logs::end;
}

