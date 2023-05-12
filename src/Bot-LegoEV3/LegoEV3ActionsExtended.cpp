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
    //servos().setup(SERVO_7_PUSH_SQUARES, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    //servos().setup(SERVO_11_TAKE_TROPHY, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_13_FORK_L, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);
    up &= servos().setup(SERVO_15_ARM_L, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);




    sensors_.addConfigFront(false, true, false);
    sensors_.addConfigBack(false, true, false);

    sensors_.addThresholdFront(50+360, 650, 50+360);
    sensors_.addThresholdFrontVeryClosed(50+120, 450, 50+120);

    sensors_.addThresholdBack(50+300, 650, 50+300);
    sensors_.addThresholdBackVeryClosed(50+120, 450, 50+120);






}
