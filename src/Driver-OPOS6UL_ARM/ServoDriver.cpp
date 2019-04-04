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
    if (percent >= 100)
        percent = 100;
    if (percent <= 0)
        percent = 0;



}

void ServoDriver::turn(int servo, int speed)
{
    if (speed >= 1023)
        speed = 1023;
    if (speed <= 0)
        speed = 0;



}

void ServoDriver::release(int servo)
{

}

void ServoDriver::setRate(int servo, int speed)
{
    if (speed >= 1023)
        speed = 1023;
    if (speed <= 0)
        speed = 0;



}
//return 1 when moving
int ServoDriver::getMoving(int servo)
{

}

int ServoDriver::getPos(int servo)
{

}

int ServoDriver::ping(int servo)
{

}

