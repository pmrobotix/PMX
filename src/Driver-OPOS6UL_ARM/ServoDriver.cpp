//drivers...OPOS

#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"
#include "DynamixelDriver.hpp"

using namespace std;

AServoDriver * AServoDriver::create(ServoType type)
{
    if (type == SERVO_DYNAMIXEL) {
        static DynamixelDriver *instance = new DynamixelDriver();
        return instance;
    } else { //type == SERVO_DYNAMIXEL
        static ServoDriver *instance = new ServoDriver();
        return instance;
    }
}

ServoDriver::ServoDriver() :
        connected_(1)
{
    logger().debug() << "ServoDriver::ServoDriver()" << logs::end;

}

void ServoDriver::hold(int servo)
{

}

void ServoDriver::setPosition(int servo, int percent)
{
    constrain(percent, -100, 100);

}

void ServoDriver::turn(int servo, int speed)
{
    constrain(speed, -100, 100);

}

void ServoDriver::release(int servo)
{

}

void ServoDriver::setRate(int servo, int speed)
{
    if (!testIf(servo, 0, 127))
        return;

    constrain(speed, -100, 100);

}
//return 1 when moving
int ServoDriver::getMoving(int servo)
{
    return -1;
}

int ServoDriver::getPos(int servo)
{
    return -1;
}

int ServoDriver::ping(int servo)
{
    return -1;
}

void ServoDriver::setMinPulse(int servo, int pulse)
{

}

void ServoDriver::setMidPulse(int servo, int pulse)
{

}

void ServoDriver::setMaxPulse(int servo, int pulse)
{

}

void ServoDriver::setPolarity(int servo, bool inversed)
{

}

int ServoDriver::getTorque(int servo)
{
    return -1;
}

bool ServoDriver::testIf(long value, long valeurMin, long valeurMax)
{
    if ((value >= valeurMin) && (value <= valeurMax))
        return true;
    else {
        logger().error() << "SERVO ID NOT EXISTS!! testIf()" << logs::end;
        return false;
    }
}

