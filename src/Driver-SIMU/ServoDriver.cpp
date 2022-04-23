//drivers...SIMU

#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create()
{
    static ServoDriver *instance = new ServoDriver();
    return instance;
}

ServoDriver::ServoDriver() :
        connected_(0)
{
    logger().debug() << "ServoDriver()" << logs::end;

}

void ServoDriver::setType(int servo, ServoType)
{

}

void ServoDriver::hold(int servo) // 1 à 8
{
    switch (servo) {

    default:
        break;
    }
}

void ServoDriver::setPulsePos(int servo, int pos)
{
    switch (servo) {

    default:
        break;
    }
}

void ServoDriver::release(int servo)
{
    switch (servo) {

    default:
        break;
    }
}

void ServoDriver::setRate(int servo, int millisec)
{
    switch (servo) {

    default:
        break;
    }
}

void ServoDriver::turn(int servo, int speed)
{
}

int ServoDriver::getMoving(int servo)
{
    return -1;
}
int ServoDriver::getPulsePos(int servo)
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
