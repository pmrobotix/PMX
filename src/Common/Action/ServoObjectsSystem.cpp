#include "ServoObjectsSystem.hpp"

#include <unistd.h>

#include "../Action.Driver/AServoDriver.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(Actions & actions, AServoDriver::ServoType type) :
        AActionsElement(actions)
{
    servodriver_ = AServoDriver::create(type);
    for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
        ServoLabel foo = static_cast<ServoLabel>(fooInt);
        //servodriver->setPosition(foo, 0);
        release(foo);
    }
}

ServoObjectsSystem::~ServoObjectsSystem()
{
//    for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
//        ServoLabel foo = static_cast<ServoLabel>(fooInt);
//        //servodriver->setPosition(foo, 0);
//        release(foo);
//    }
}

void ServoObjectsSystem::deploy(int servo, int percent, int keep_millisec)
{
    hold(servo);
    servodriver_->setPosition(servo, percent); // percentage
    if (keep_millisec > 0) {
        sleep(keep_millisec * 1000);
        release(servo);
    } else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
            r = servodriver_->getMoving(servo);
            if (r < 0) {
                break;
            }
            //logger().info() << "wait "<< r << logs::end;
            usleep(10000);
        }
    }
}

void ServoObjectsSystem::release(int servo)
{
    servodriver_->release(servo);
}

void ServoObjectsSystem::hold(int servo)
{
    servodriver_->hold(servo);
}

void ServoObjectsSystem::setSpeed(int servo, int speed)
{
    servodriver_->setRate(servo, speed);
}

void ServoObjectsSystem::turn(int servo, int speed_percent, int keep_millisec)
{

}

void ServoObjectsSystem::detectAll()
{
    for (int i = 0; i < 255; i++) {
        if (servodriver_->ping(i) == 0) {
            logger().info() << i << logs::end;
        }
    }

}

