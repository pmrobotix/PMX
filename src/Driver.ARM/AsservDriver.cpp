//drivers...ARM

#include "AsservDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create(std::string)
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver()
		: extRightEncoder_('D', 31), extLeftEncoder_('B', 17)
{
	md25_.begin();
	float volts = md25_.getBatteryVolts();
	logger().info() << "volts=" << volts << logs::end;

	//encoderLeft_(*this, 'B', 17), encoderRight_(*this, 'D', 31),
/*
	unsigned char status = extRightEncoder_.readStatus();
	logger().error() << "status spi=" << reinterpret_cast<void*>(status) << logs::end;
	extRightEncoder_.clearCounter();*/
}

AsservDriver::~AsservDriver()
{
}

void AsservDriver::setMotorLeftPosition(int power, long ticks)
{

}

void AsservDriver::setMotorRightPosition(int power, long ticks)
{

}

void AsservDriver::setMotorLeftPower(int power, int timems)
{
	power = power + 127;
	md25_.setSpeedReg(power, MD25_SPEED1_REG);
}

void AsservDriver::setMotorRightPower(int power, int timems)
{
	power = power + 127;
	md25_.setSpeedReg(power, MD25_SPEED2_REG);
}

long AsservDriver::getLeftExternalEncoder()
{
	return extLeftEncoder_.readCounter();
}
long AsservDriver::getRightExternalEncoder()
{
	long r = extRightEncoder_.readCounter();
	logger().error() << " r=" << r << logs::end;

	return r;
}

long AsservDriver::getLeftInternalEncoder()
{
//+/- 2,147,483,648

	return md25_.ensureGetEncoder(0, MD25_ENCODER1_REG);

}
long AsservDriver::getRightInternalEncoder()
{
	return md25_.ensureGetEncoder(0, MD25_ENCODER2_REG);
}

void AsservDriver::resetEncoder()
{
	md25_.resetEncoders();
}

void AsservDriver::stopMotorLeft()
{
	md25_.stopMotor(MD25_SPEED1_REG);
}
void AsservDriver::stopMotorRight()
{
	md25_.stopMotor(MD25_SPEED2_REG);
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
