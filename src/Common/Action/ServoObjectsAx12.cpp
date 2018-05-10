#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "Actions.hpp"
#include "ServoObjectsAx12.hpp"

using namespace std;

ServoObjectsAx12::ServoObjectsAx12(Actions & actions) :
        AActionsElement(actions)
{
    servodriver = AServoDriver::create();
    nb_servos_ = 4;
    servo_list_ = new int[nb_servos_];
    servo_list_[0] = 5;
    servo_list_[1] = 7;
    servo_list_[2] = 14;
    servo_list_[3] = 51;

}

ServoObjectsAx12::~ServoObjectsAx12()
{
    for (int i = 0; i < nb_servos_; i++) {
        servodriver->setPosition(servo_list_[i], 0);
        release(servo_list_[i]);
    }
}
//0 => hold
//>0 => wait sec keeping force
//-1 => wait moving and hold
void ServoObjectsAx12::deploy(int servo, int pos, int keep_millisec)
{
    //printf("deploy %i %i \n", servo, pos);
    logger().info() << "deploy "<< servo << pos << logs::end;
    hold(servo);
    servodriver->setPosition(servo, pos); // percentage
    if (keep_millisec > 0) {
        usleep(keep_millisec * 1000.0);
        release(servo);
    } else if (keep_millisec == -1) {

        while (int r = servodriver->getMoving(servo) >= 1) {
            r= servodriver->getMoving(servo);
            if (r < 0) {
                break;
            }
            //logger().info() << "wait "<< r << logs::end;
            usleep(10000);
        }
        /*
         int p = servodriver->getPos(servo);
         printf ("first p=%d b=%d\n", p, b);
         b++;
         while ( !((p <= pos + 5) && (p >= pos - 5)))
         {
         p = servodriver->getPos(servo);
         printf ("p=%d b=%d\n", p, b);
         b++;
         if (b>1000) break;
         //usleep(100000);
         }
         */
    }
    hold(servo);
}

void ServoObjectsAx12::turn(int servo, int speed, int keep_millisec)
{
    hold(servo);
    servodriver->turn(servo, speed); // percentage
    if (keep_millisec != 0) {
        usleep(keep_millisec * 1000.0);
        release(servo);
    }
    hold(servo);
}

void ServoObjectsAx12::setSpeed(int servo, int speed)
{
    servodriver->setRate(servo, speed);
}

void ServoObjectsAx12::setSpeedAll(int speed)
{
    for (int i = 0; i < nb_servos_; i++) {
        setSpeed(servo_list_[i], speed);
    }
}

void ServoObjectsAx12::release(int servo)
{
    servodriver->release(servo);
}

void ServoObjectsAx12::hold(int servo)
{
    servodriver->hold(servo);
}

void ServoObjectsAx12::releaseAll()
{

    for (int i = 0; i < nb_servos_ - 1; i++) {
        release(servo_list_[i]);
    }

}
void ServoObjectsAx12::holdAll()
{
    for (int i = 0; i < nb_servos_; i++) {
        hold(servo_list_[i]);
    }

}

void ServoObjectsAx12::detectAll()
{
    for (int i = 0; i < 255; i++) {
        if (servodriver->ping(i) == 0) {
            logger().info() << i << logs::end;
        }
    }

}
