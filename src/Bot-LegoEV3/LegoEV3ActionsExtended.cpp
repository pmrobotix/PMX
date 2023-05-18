#include "LegoEV3ActionsExtended.hpp"

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/Logger.hpp"

LegoEV3ActionsExtended::LegoEV3ActionsExtended(std::string botId, Robot * robot) :
        ledbar_(botId, *this, 2), buttonbar_(*this), soundbar_(*this), tirette_(*this), sensors_(*this, robot)
, lcd_(botId, *this)
//, servoUsingMotor_(*this)
, servos_(botId, *this)
{
    logger().debug() << "LegoEV3ActionsExtended()" << logs::end;

    //min mid max, //TODO inversed(NOT IMPLEMENTED YET)
    bool up = false;
    up &= servos().setup(SERVO_1_ARM_R, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_3_FORK_R, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_7_fUNNY_BACK, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_8_fUNNY_FRONT, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_13_FORK_L, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_15_ARM_L, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);


    sensors_.addConfigFront(false, true, false);
    sensors_.addConfigBack(false, true, false);

    sensors_.addThresholdDiameterOpponent_mm(350); //deprecated

    //from center of the robot for the beacon jusqu'centre du robot adv
   //= rayon de notre robot + espace + rayon robot adverse
//    sensors_.addThresholdFront(50+360, 180+350+180, 50+360);
//    sensors_.addThresholdFrontVeryClosed(50+120, 180+70+180 , 50+120);
    sensors_.addThresholdFront(50+360, 180+400+180, 50+360);
        sensors_.addThresholdFrontVeryClosed(50+120, 180+140+180 , 50+120);

    sensors_.addThresholdBack(50+300, 180+350+180, 50+300);
    sensors_.addThresholdBackVeryClosed(50+120, 180+70+180, 50+120);


    //sensors_.toSVG();

}
