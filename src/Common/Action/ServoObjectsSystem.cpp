#include "ServoObjectsSystem.hpp"

#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(std::string botId, Actions & actions) :
        AActionsElement(actions), botId_(botId)
{
    servodriver_ = AServoDriver::create();
}

ServoObjectsSystem::~ServoObjectsSystem() {
    delete servodriver_;
}

void ServoObjectsSystem::setup(int servo, AServoDriver::ServoType type, int valueMinPulse, int valueMidPulse, int valueMaxPulse, bool inversed) {
    //release
    servodriver_->release(servo);

    servodriver_->setType(servo, type);
    servodriver_->setPolarity(servo, inversed);
    servodriver_->setMinPulse(servo, valueMinPulse);
    servodriver_->setMidPulse(servo, valueMidPulse);
    servodriver_->setMaxPulse(servo, valueMaxPulse);

}

//keep_millisec = -1 : wait moving until position
//keep_millisec > 0 : time to wait
//keep_millisec = 0 : continue and hold
void ServoObjectsSystem::deploy(int servo, int pos, int keep_millisec) {
    hold(servo);
    servodriver_->setPulsePos(servo, pos); // percentage
    if (keep_millisec > 0) {
        utils::sleep_for_micros(keep_millisec * 1000); //TODO verifier la torque
        release(servo);
    }
    else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
//            if (r<0)
//            {
//                logger().info() << "servo=" << servo << " deploy getMoving break"  << logs::end;
//                break;
//            }
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
    logger().info() << "servo=" << servo << " pos= " << pos << logs::end;
}
void ServoObjectsSystem::setTorque(int servo, int torque) {
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
}
int ServoObjectsSystem::getTorque(int servo) {
    return servodriver_->getTorque(servo);
}

int ServoObjectsSystem::setPos(int servo, int pos, int milli0to90) {

    hold(servo);
    servodriver_->setPulsePos(servo, pos, milli0to90);
}

int ServoObjectsSystem::getPulseWidth(int servo) {
    return servodriver_->getPulsePos(servo);
}

void ServoObjectsSystem::release(int servo) {
    servodriver_->release(servo);
}

void ServoObjectsSystem::hold(int servo) {
    servodriver_->hold(servo);
}
int ServoObjectsSystem::getSpeed(int servo) {
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    return -1;
}
void ServoObjectsSystem::setSpeed(int servo, int speed) {
    servodriver_->setRate(servo, speed);
}

void ServoObjectsSystem::turn(int servo, int speed_percent, int keep_millisec) {
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
}

void ServoObjectsSystem::detectAll() {
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    for (int i = 0; i < 255; i++) {
        if (int err = servodriver_->ping(i) == 0) {
            logger().info() << i << " err=" << err << logs::end;
        }
    }
}

void ServoObjectsSystem::detect() {
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
//    if (servodriver_->ping(51) != 0) {
//        logger().error() << "ERROR AX12 CONNECTION !!!" << logs::end;
//    }
}

ServoObjectsTimer::ServoObjectsTimer(ServoObjectsSystem & sOsS, string name, uint timeSpan_us) :
        servoObjectsSystem_(sOsS)
{
    this->init(name, timeSpan_us);
}

//void ServoObjectsSystem::deployByTimerTask(int servo, int percent, int keep_millisec)
//{
// //add the timer to move and wait until arrive at position.
//
//}
void ServoObjectsSystem::move_1_servo(int servo1, int pos1, int torque1, int time_eta_ms, bool keep_torque) {

}

void ServoObjectsSystem::move_2_servos(int servo1, int pos1, int torque1, int servo2, int pos2, int torque2, int time_eta_ms, bool keep_torque) {

}

void ServoObjectsTimer::onTimer(utils::Chronometer chrono) {
    //check des positions

}

void ServoObjectsTimer::onTimerEnd(utils::Chronometer chrono) {

}

std::string ServoObjectsTimer::info() {
    std::ostringstream oss;
    oss << "ServoObjectsTimer [" << name() << "] for " << servoObjectsSystem_.id();
    return oss.str();
}

