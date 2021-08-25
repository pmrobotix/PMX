#include "OPOS6UL_ActionsExtended.hpp"

OPOS6UL_ActionsExtended::OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
        ledbar_(botId, *this, 8), buttonbar_(*this), lcd2x16_(botId, *this), tirette_(*this), sensors_(*this, robot), servos_ax12_(
                *this, AServoDriver::SERVO_DYNAMIXEL), servos_std_(*this, AServoDriver::SERVO_STANDARD)

{
    lcd2x16_.init();

    //config des sensors
    //TODO ajouter la configuration de la position des capteurs sensors_.addConfigPosFront(-140, 0, +140);

    sensors_.addConfigFront(true, false, true);
    sensors_.addConfigBack(false, false, false);

    sensors_.addThresholdFront(450, 450, 450);
    sensors_.addThresholdFrontVeryClosed(200, 200, 200);

    sensors_.addThresholdBack(0, 220, 0);
    sensors_.addThresholdBackVeryClosed(0, 160, 0);

}

