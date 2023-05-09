//drivers...SIMU

#include "ServoUsingMotorDriver.hpp"

#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>

#include "MD25.hpp"

using namespace std;

AServoUsingMotorDriver * AServoUsingMotorDriver::create() {
//	static ServoUsingMotorDriver *instance = new ServoUsingMotorDriver();
//	return instance;
    return new ServoUsingMotorDriver();
}

ServoUsingMotorDriver::ServoUsingMotorDriver() :
        connected_(0)
{
    logger().debug() << "ServoUsingMotorDriver()" << logs::end;
    connected_ = MD25::instance().begin();

}

bool ServoUsingMotorDriver::is_connected()
{
    return connected_;
}

void ServoUsingMotorDriver::setMotorPosition(int power, int pos, int ramptimems) {
    //MD25::instance().speed1_set(power);
    MD25::instance().speed2_set(power);
}

long ServoUsingMotorDriver::getInternalEncoder() {
    return 0;
}

void ServoUsingMotorDriver::stopMotor() {
    MD25::instance().speed1_set(0);
    MD25::instance().speed2_set(0);
}

void ServoUsingMotorDriver::resetEncoder(int pos) {

}

int ServoUsingMotorDriver::getMotorCurrent() {
    return 0;
}

void ServoUsingMotorDriver::enableHardRegulation(bool enable) {

}
