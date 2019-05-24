#include "OPOS6UL_ActionsExtended.hpp"


OPOS6UL_ActionsExtended::OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
ledbar_(botId, *this, 8), buttonbar_(*this), lcd2x16_(botId, *this), tirette_(*this), sensors_(*this,
        robot), servos_ax12_(*this, AServoDriver::SERVO_DYNAMIXEL), servos_std_(*this, AServoDriver::SERVO_STANDARD)

{
    lcd2x16_.init();

    sensors_.addThresholdFront(250, 0, 250);
    sensors_.addThresholdFrontVeryClosed(130,0,130);

    sensors_.addThresholdBack(250,0,250);
    sensors_.addThresholdBackVeryClosed(130,0,130);

    sensors_.addConfigFront(true, true, true);
    sensors_.addConfigBack(false, true, false);
}

