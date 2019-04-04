//drivers...SIMU

#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create(ServoType type)
{
    static ServoDriver *instance = new ServoDriver();
    return instance;
}

ServoDriver::ServoDriver() :
        connected_(0)
{
    logger().debug() << "ServoDriver()" << logs::end;

}

void ServoDriver::hold(int servo) // 1 à 8
{
    switch (servo) {
    /*
     case SERVO_LEFT:
     break;

     case SERVO_RIGHT:
     break;

     case SERVO_CENTRE:
     break;
     */
    default:
        break;
    }
}

void ServoDriver::setPosition(int servo, int pos)
{
    switch (servo) {
    /*
     case SERVO_LEFT:
     break;

     case SERVO_RIGHT:
     break;

     case SERVO_CENTRE:
     break;
     */
    default:
        break;
    }
}

void ServoDriver::release(int servo)
{
    switch (servo) {
    /*
     case SERVO_LEFT:
     break;

     case SERVO_RIGHT:
     break;

     case SERVO_CENTRE:
     break;
     */
    default:
        break;
    }
}

void ServoDriver::setRate(int servo, int speed)
{
    switch (servo) {
    /*
     case SERVO_LEFT:
     break;

     case SERVO_RIGHT:
     break;

     case SERVO_CENTRE:
     break;
     */
    default:
        break;
    }
}

void ServoDriver::turn(int servo, int speed)
{

}

int ServoDriver::getMoving(int servo)
{
    return 0;
}

int ServoDriver::getPos(int servo)
{
    return 0;
}

int ServoDriver::ping(int servo)
{
    return 0;
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
