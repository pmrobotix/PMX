#include "LegoEV3ActionsExtended.hpp"

#include <string>

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/Logger.hpp"

LegoEV3ActionsExtended::LegoEV3ActionsExtended(std::string botId, Robot * robot) :
        ledbar_(botId, *this, 2), buttonbar_(*this), soundbar_(*this), tirette_(*this), sensors_(*this, robot)//, servos_std_(*this, AServoDriver::SERVO_STANDARD)
{
    logger().debug() << "LegoEV3ActionsExtended()" << logs::end;

    /*
    releaseAll();

    //Servo config specifique
    servosStd().setPolarity(0);
    servosStd().setMinPulse(0, 600);
    servosStd().setMidPulse(0, 1470);
    servosStd().setMaxPulse(0, 2350);*/

}
