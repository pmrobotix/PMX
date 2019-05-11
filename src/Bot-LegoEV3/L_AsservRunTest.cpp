#include "L_AsservRunTest.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "../Common/Action/Actions.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Automate.hpp"
#include "L_State_WaitEndOfMatch.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservRunTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //match color
    robot.getArgs().addArgument("x", "x mm");
    //robot.getArgs().addArgument("y", "y mm");
    //robot.getArgs().addArgument("d", "distance");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_AsservRunTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    utils::Chronometer chrono("L_AsservRunTest");
    long left;
    long right;

    float x = 0.0;
    //float y = 0.0;
    //float a = 0.0;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    if (args["x"] != "0") {
        x = atof(args["x"].c_str());
        logger().debug() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
    }
    /*if (args["y"] != "0")
     {
     y = atof(args["y"].c_str());
     logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
     }
     if (args["d"] != "0")
     {
     a = atof(args["d"].c_str());
     logger().debug() << "Arg a set " << args["d"] << ", a = " << a << logs::end;
     }*/

    robot.asserv().startMotionTimerAndOdo(true);

    logger().info() << "COORD 0,300" << logs::end;
    robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));

    robot.svgPrintPosition();

    chrono.start();

    logger().info() << "COORD x,300 avec x=" << x << logs::end;

    robot.actions().start();
    robot.actions().addAction(new TestActionRun(*this));

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    TRAJ_STATE ts;
    while ((ts = robot.asserv().doMoveForwardAndRotateTo(x, 300, 0)) != TRAJ_OK)
    {
        robot.svgPrintPosition();
        robot.actions().ledBar().set(0, LED_AMBER);
        logger().info() << "attente trajstate=" << ts << logs::end;
        //break;
        //robot.asserv().freeMotion();
        sleep(2);
        robot.actions().ledBar().setOff(0);
    }

    left = robot.asserv().base()->encoders().getLeftEncoder();
    right = robot.asserv().base()->encoders().getRightEncoder();
    logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
            << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
            << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();

    robot.stopAll();
    logger().info() << "Happy End." << logs::end;
}

TestActionRun::TestActionRun(L_AsservRunTest & amt) :
        amt_(amt), chrono_("TestActionRun")
{
    chrono_.start();
    i_ = 0;
}

//execution de la tâche
bool TestActionRun::execute()
{
    logger().info() << " !!!!! execution time=" << chrono_.getElapsedTimeInMicroSec() << " us i=" << i_ << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    logger().info() << "ACTIVATION DETECTION ADVERSAIRE !" << logs::end;

    //robot.actions().sensors().startSensors();

    Automate automate;
    IAutomateState* stateWaitEndOfMatch = new L_State_WaitEndOfMatch();
    // Start the automate and wait for its return
    automate.run(robot, stateWaitEndOfMatch);

    return false;

}
