#include "O_Asserv_CalageTest.hpp"

#include <cmath>
#include <cstdlib>
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

void O_Asserv_CalageTest::configureConsoleArgs(int argc, char **argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.getArgs().addArgument("d", "dist en mm");
    robot.getArgs().addArgument("ty",
            "type de calage [B]ordure, [R]ight sensor, [L]eft sensor, [DR] demo right, [DL] demo left", "B");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_Asserv_CalageTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    Arguments args = robot.getArgs();

    float d = 0.0;
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }

    std::string type;
    if (args["ty"] != "0") {
        type = args["ty"];
        logger().info() << "Arg type set " << args["ty"] << ", type = " << type << logs::end;
    }
    if (type == "B") { //calage bordure

        logger().info() << "Start Asserv " << logs::end;
        robot.setMyColor(PMXGREEN);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXGREEN));
        RobotPosition p = robot.asserv().pos_getPosition();
        logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
                << p.asservStatus << logs::end;
        robot.svgPrintPosition();

        logger().info() << "GO distance calage mm=" << d << logs::end;
        robot.asserv().doCalage(d, 45);
        logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
                << p.asservStatus << logs::end;
        robot.svgPrintPosition();

    } else if (type == "R") { //right sensor

        robot.setMyColor(PMXGREEN);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(70, 450.0, 0.0, (robot.getMyColor() != PMXGREEN));

        //robot.asserv().doMoveForwardTo(1680, 255); // avec un angle supplémentaire ou pas.
        robot.asserv().doMoveForwardAndRotateTo(1680, 255, 0); // avec un angle supplémentaire

        //on trouve la position reelle xB, yB, et la direction à corriger, angle teta à partir du point de depart x,y, la distance d'avancée et l'angle de fin
        //float mesure_mm = robot.actions().sensors().rightSide();
        float mesure_mm = d;

        float pos_x_start_mm = 70;
        float pos_y_start_mm = 450;
        float delta_j_mm = 100;
        float delta_k_mm = 0;
        float robot_size_l_mm = 150; //largeur du robot à partir du centre
        RobotPosition p = robot.asserv().pos_getPosition();
        logger().info() << "POS before : x=" << p.x * 1000.0 << " y=" << p.y * 1000.0 << " a=" << p.theta << " degrees="
                << p.theta * 180 / M_PI << logs::end;

        int succeed = robot.asserv().adjustRealPosition(pos_x_start_mm, pos_y_start_mm, p, delta_j_mm, delta_k_mm,
                mesure_mm, robot_size_l_mm);
        sleep(1);
        RobotPosition pnew = robot.asserv().pos_getPosition();
        logger().info() << "succeed=" << succeed << " POS after : x=" << pnew.x * 1000.0 << " y=" << pnew.y * 1000.0
                << " a=" << pnew.theta << " degrees=" << pnew.theta * 180 / M_PI << logs::end;
    } else

    if (type == "L") { //left sensor

        robot.setMyColor(PMXBLUE);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(70, 450.0, 0.0, (robot.getMyColor() != PMXGREEN));

        //robot.asserv().doMoveForwardTo(1680, 255); // avec un angle supplémentaire ou pas.
        robot.asserv().doMoveForwardAndRotateTo(1680, 255, 0); // avec un angle supplémentaire

        //on trouve la position reelle xB, yB, et la direction à corriger, angle teta à partir du point de depart x,y, la distance d'avancée et l'angle de fin
        //float mesure_mm = robot.actions().sensors().leftSide();
        float mesure_mm = d;

        float pos_x_start_mm = 70;
        float pos_y_start_mm = 450;
        float delta_j_mm = 100;
        float delta_k_mm = 0;
        float robot_size_l_mm = 150;
        RobotPosition p = robot.asserv().pos_getPosition();
        logger().info() << "POS before : x=" << p.x * 1000.0 << " y=" << p.y * 1000.0 << " a=" << p.theta << " degrees="
                << p.theta * 180 / M_PI << logs::end;

        int succeed = robot.asserv().adjustRealPosition(pos_x_start_mm, pos_y_start_mm, p, delta_j_mm, delta_k_mm,
                mesure_mm, robot_size_l_mm);
        sleep(1);
        RobotPosition pnew = robot.asserv().pos_getPosition();
        logger().info() << "succeed=" << succeed << " POS after : x=" << pnew.x * 1000.0 << " y=" << pnew.y * 1000.0
                << " a=" << pnew.theta << " degrees=" << pnew.theta * 180 / M_PI << logs::end;
    } else

    if (type == "DR") {            //demo theorique sensor left

        robot.setMyColor(PMXGREEN);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(470, 1000.0, 0.0, (robot.getMyColor() != PMXGREEN));

        //robot.asserv().doMoveForwardTo(1680, 255); // avec un angle supplémentaire ou pas.
        robot.asserv().doMoveForwardAndRotateTo(1780, 280, 0); // avec un angle supplémentaire ou pas.

        RobotPosition p = robot.asserv().pos_getPosition();
        logger().info() << "POS before : x=" << p.x * 1000.0 << " y=" << p.y * 1000.0 << " a=" << p.theta << " degrees="
                << p.theta * 180 / M_PI << logs::end;

        //on trouve la position reelle xB, yB, et la direction à corriger, angle teta à partir du point de depart x,y, la distance d'avancée et l'angle de fin
        float mesure_mm = d;

        float pos_x_start_mm = 470;
        float pos_y_start_mm = 1000;
        float delta_j_mm = 200;
        float delta_k_mm = 100;
        float robot_size_l_mm = 150;

        int succeed = robot.asserv().adjustRealPosition(pos_x_start_mm, pos_y_start_mm, p, delta_j_mm, delta_k_mm,
                mesure_mm, robot_size_l_mm);

        RobotPosition pnew = robot.asserv().pos_getPosition();

        logger().info() << "succeed=" << succeed << " POS after : x=" << pnew.x * 1000.0 << " y=" << pnew.y * 1000.0
                << " a=" << pnew.theta << " degrees=" << pnew.theta * 180 / M_PI << logs::end;
    } else

    if (type == "DL") { //demo theorique sensor right

        robot.setMyColor(PMXBLUE);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(470, 1000.0, 0.0, (robot.getMyColor() != PMXGREEN));

        //robot.asserv().doMoveForwardTo(1680, 255); // avec un angle supplémentaire ou pas.
        robot.asserv().doMoveForwardAndRotateTo(1780, 280, 0); // avec un angle supplémentaire ou pas.

        RobotPosition p = robot.asserv().pos_getPosition();
        logger().info() << "POS before : x=" << p.x * 1000.0 << " y=" << p.y * 1000.0 << " a=" << p.theta << " degrees="
                << p.theta * 180 / M_PI << logs::end;

        //on trouve la position reelle xB, yB, et la direction à corriger, angle teta à partir du point de depart x,y, la distance d'avancée et l'angle de fin
        float mesure_mm = d;

        float pos_x_start_mm = 470;
        float pos_y_start_mm = 1000;
        float delta_j_mm = 200;
        float delta_k_mm = 100;
        float robot_size_l_mm = 150;

        int succeed = robot.asserv().adjustRealPosition(pos_x_start_mm, pos_y_start_mm, p, delta_j_mm, delta_k_mm,
                mesure_mm, robot_size_l_mm);

        RobotPosition pnew = robot.asserv().pos_getPosition();

        logger().info() << "succeed=" << succeed << " POS after : x=" << pnew.x * 1000.0 << " y=" << pnew.y * 1000.0
                << " a=" << pnew.theta << " degrees=" << pnew.theta * 180 / M_PI << logs::end;
    }

    logger().info() << "Happy End." << logs::end;
}

