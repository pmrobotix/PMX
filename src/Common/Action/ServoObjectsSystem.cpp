#include "ServoObjectsSystem.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(Actions & actions, AServoDriver::ServoType type) :
        AActionsElement(actions)
{
    servodriver_ = AServoDriver::create(type);
}

ServoObjectsSystem::~ServoObjectsSystem()
{
    delete servodriver_;
}

//keep_millisec = -1 : wait moving until position
//keep_millisec > 0 : time to wait
//keep_millisec = 0 : continue
void ServoObjectsSystem::deploy(int servo, int percent, int keep_millisec)
{
    hold(servo);
    servodriver_->setPosition(servo, percent); // percentage
    if (keep_millisec > 0) {
        utils::sleep_for_micros(keep_millisec * 1000); //TODO verifier la torque
        release(servo);
    } else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
            if (r<0)
            {
                logger().info() << "servo=" << servo << " deploy getMoving break"  << logs::end;
                break;
            }
            r = servodriver_->getMoving(servo);

//            if (r <= 0 || r >=200) {
//                r = servodriver_->getMoving(servo);
//            }
//            if (r <= 0 || r >=200) {
//                            r = servodriver_->getMoving(servo);
//                        }
//            if (r == 0) {
//                int torque = servodriver_->getTorque(servo);
//                if (torque > 600)
//                {
//                    logger().info() << "servo=" << servo << " percent= "<< percent << " TOO MUCH TORQUE torque= "<< torque << logs::end;
//                    release(servo);
//                    break; //TODO try a 2nde time
//                }
//            }


            //logger().info() << "wait "<< r << logs::end;
            utils::sleep_for_micros(2000);
        }
    }
    //int torque = getTorque(servo);
    logger().info() << "servo=" << servo << " percent= "<< percent  << logs::end;
}

int ServoObjectsSystem::getTorque(int servo)
{
    return servodriver_->getTorque(servo);
}

int ServoObjectsSystem::getPos(int servo)
{
    return servodriver_->getPos(servo);
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

void ServoObjectsSystem::setMinPulse(int servo, int value)
{
    servodriver_->setMinPulse(servo, value);
}
void ServoObjectsSystem::setMidPulse(int servo, int value)
{
    servodriver_->setMidPulse(servo, value);
}
void ServoObjectsSystem::setMaxPulse(int servo, int value)
{
    servodriver_->setMaxPulse(servo, value);
}
void ServoObjectsSystem::setPolarity(int servo, bool inversed)
{
    servodriver_->setPolarity(servo, inversed);
}

void ServoObjectsSystem::detectAll()
{
    for (int i = 0; i < 255; i++) {
        if (int err = servodriver_->ping(i) == 0) {
            logger().info() << i << " err=" << err << logs::end;
        }
    }
}

void ServoObjectsSystem::detect()
{
//    if (servodriver_->ping(51) != 0) {
//        logger().error() << "ERROR AX12 CONNECTION !!!" << logs::end;
//    }
}
