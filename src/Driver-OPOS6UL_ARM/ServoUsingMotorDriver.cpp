//drivers...SIMU

#include "ServoUsingMotorDriver.hpp"

#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>


using namespace std;


AServoUsingMotorDriver * AServoUsingMotorDriver::create()
{
	static ServoUsingMotorDriver *instance = new ServoUsingMotorDriver();
	return instance;
}

ServoUsingMotorDriver::ServoUsingMotorDriver() :
		connected_(0)
{
	logger().debug() << "ServoUsingMotorDriver()" << logs::end;


}

void ServoUsingMotorDriver::setMotorPosition(int pos, int ramptimems, int power)
{

}

long ServoUsingMotorDriver::getInternalEncoder()
{
	return 0;
}

void ServoUsingMotorDriver::stopMotor()
{

}

void ServoUsingMotorDriver::resetEncoder(int pos)
{

}

int ServoUsingMotorDriver::getMotorCurrent()
{
	return 0;
}

void ServoUsingMotorDriver::enableHardRegulation(bool enable)
{

}
