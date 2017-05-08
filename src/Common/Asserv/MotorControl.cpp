#include "MotorControl.hpp"

#include "../Asserv.Driver/AAsservDriver.hpp"

using namespace std;

MotorControl::MotorControl(std::string botid)//Asserv & asserv) : AAsservElement(asserv)
{
	asservdriver = AAsservDriver::create(botid);
}

MotorControl::~MotorControl()
{
}

void MotorControl::runMotorLeft(int power, int timems)
{
	asservdriver->setMotorLeftPower(power, timems);
}

void MotorControl::runMotorRight(int power, int timems)
{
	asservdriver->setMotorRightPower(power, timems);
}

void MotorControl::stopMotors()
{
	asservdriver->stopMotorLeft();
	asservdriver->stopMotorRight();
}

void MotorControl::setMotorLeftPosition(int power, long ticks) //tick à parcourir
{
	asservdriver->setMotorLeftPosition(power, ticks); //tick à parcourir
}

void MotorControl::setMotorRightPosition(int power, long ticks) //tick à parcourir
{
	asservdriver->setMotorRightPosition(power, ticks); //tick à parcourir
}


