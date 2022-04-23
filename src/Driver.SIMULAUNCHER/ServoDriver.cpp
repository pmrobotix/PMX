//drivers...SIMU

#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create() {
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

void ServoDriver::setPulsePos(int servo, int pulse) {
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

void ServoDriver::release(int servo) {
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

void ServoDriver::setRate(int servo, int millisec) {
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

void ServoDriver::turn(int servo, int speed) {

}

int ServoDriver::getMoving(int servo) {

}

int ServoDriver::getPulsePos(int servo) {

}

int ServoDriver::ping(int servo) {

}

void ServoDriver::setMinPulse(int servo, int pulse) {

}

void ServoDriver::setMidPulse(int servo, int pulse) {

}

void ServoDriver::setMaxPulse(int servo, int pulse) {

}

void ServoDriver::setPolarity(int servo, bool inversed) {

}

int ServoDriver::getTorque(int servo) {
    return -1;
}
void ServoDriver::setType(int servo, ServoType)
{

}
