#include "OPOS6UL_ActionsExtended.hpp"

OPOS6UL_ActionsExtended::OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
        ledbar_(botId, *this, 8), buttonbar_(*this), lcd2x16_(botId, *this), tirette_(*this), sensors_(*this, robot), servos_ax12_(
                *this, AServoDriver::SERVO_DYNAMIXEL), servos_std_(*this, AServoDriver::SERVO_STANDARD)

{
    lcd2x16_.init();

    //config des sensors
    sensors_.addConfigFront(true, false, true);
    sensors_.addConfigBack(false, true, false);

    sensors_.addThresholdFront(340, 345, 340);
    sensors_.addThresholdFrontVeryClosed(240, 240, 240);

    sensors_.addThresholdBack(0, 200, 0);
    sensors_.addThresholdBackVeryClosed(0, 130, 0);

}

