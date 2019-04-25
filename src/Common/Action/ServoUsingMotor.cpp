#include "ServoUsingMotor.hpp"

#include "../Action.Driver/AServoUsingMotorDriver.hpp"



using namespace std;

ServoUsingMotor::ServoUsingMotor(Actions & actions) :
        AActionsElement(actions)
{
    servoMotordriver_ = AServoUsingMotorDriver::create();

    servoMotordriver_->resetEncoder(0);
    ticks_place = 50;
}

ServoUsingMotor::~ServoUsingMotor()
{
}

void ServoUsingMotor::moveRight(int nb)
{
    servoMotordriver_->setMotorPosition(50, ticks_place * nb);
}
void ServoUsingMotor::moveLeft(int nb)
{
    servoMotordriver_->setMotorPosition(-50, ticks_place * nb);
}
void ServoUsingMotor::ejectRight()
{
    servoMotordriver_->setMotorPosition(100, ticks_place * 5);
}
void ServoUsingMotor::ejectLeft()
{
    servoMotordriver_->setMotorPosition(-100, ticks_place * 5);
}


