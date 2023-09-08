#include "MotorControl.hpp"

#include "../Interface.Driver/AAsservDriver.hpp"

using namespace std;

MotorControl::MotorControl(std::string botid, AAsservDriver* asservdriver)
{
    asservdriver_ = asservdriver;
}

MotorControl::~MotorControl()
{
}

void MotorControl::runMotorLeft(int power, int timems)
{
	asservdriver_->setMotorLeftPower(power, timems);
}

void MotorControl::runMotorRight(int power, int timems)
{
	asservdriver_->setMotorRightPower(power, timems);
}

void MotorControl::stopMotors()
{
	asservdriver_->stopMotorLeft();
	asservdriver_->stopMotorRight();
}

void MotorControl::setMotorLeftPosition(int power, long ticks) //tick à parcourir
{
	asservdriver_->setMotorLeftPosition(power, ticks); //tick à parcourir
}

void MotorControl::setMotorRightPosition(int power, long ticks) //tick à parcourir
{
	asservdriver_->setMotorRightPosition(power, ticks); //tick à parcourir
}


