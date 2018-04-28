#include "O_State_DecisionMakerIA.hpp"

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

O_State_DecisionMakerIA::O_State_DecisionMakerIA(Robot& robot) :
        robot_(robot)
{
}

void O_State_DecisionMakerIA::IASetupHomologation()
{
    logger().info() << "IASetupHomologation" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

}
/*
 IAutomateState*
 O_State_DecisionMakerIA::execute(Robot &, void *data)
 {
 logger().info() << "O_State_DecisionMaker execute" << logs::end;
 Data* sharedData = (Data*) data; //TODO objet sur robot.data ?

 OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

 robot.svgPrintPosition();

 //TODO choisir IA suivant sharedData->ia string
 IASetupHomologation();

 robot.actions().sensors().startSensors();
 //robot.ia().iAbyZone().ia_start(); //launch IA

 robot.actions().servo_init_end();
 robot.freemotion();
 robot.actions().servoObjects().releaseAll();

 robot.stopAll();
 return NULL; //finish all state
 }*/

void O_State_DecisionMakerIA::execute()
{
    logger().info() << "O_State_DecisionMakerIA executing..." << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.svgPrintPosition();


    //robot.actions().ledBar().k2mil(100, 100000, LED_GREEN);
    robot.actions().start();
    robot.actions().ledBar().startK2mil(100, 100000, LED_GREEN);

    while (1) {
usleep(1000);
    }

    logger().info() << "O_State_DecisionMakerIA executed" << logs::end;
}
