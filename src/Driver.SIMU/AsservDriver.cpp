//drivers...SIMU

#include "AsservDriver.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create()
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver()
{

}

AsservDriver::~AsservDriver()
{
}


void AsservDriver::setMotorLeftPosition(long ticks, int power)
{

}

void AsservDriver::setMotorRightPosition(long ticks, int power)
{

}

void AsservDriver::setMotorLeftPower(int power, int timems)
{

}
void AsservDriver::setMotorRightPower(int power, int timems)
{

}

long AsservDriver::getLeftExternalEncoder()
{
	return 0; //TODO getLeftExternalEncoder
}
long AsservDriver::getRightExternalEncoder()
{
	return 0; //TODO getRightExternalEncoder
}

long AsservDriver::getLeftInternalEncoder()
{
	//+/- 2,147,483,648
	return 0;

}
long AsservDriver::getRightInternalEncoder()
{
	return 0;
}

void AsservDriver::resetEncoder()
{

}

void AsservDriver::stopMotorLeft()
{

}
void AsservDriver::stopMotorRight()
{

}

int AsservDriver::getMotorLeftCurrent()
{
	return 0;
}
int AsservDriver::getMotorRightCurrent()
{
	return 0;
}

void AsservDriver::enableHardRegulation(bool enable)
{

}
