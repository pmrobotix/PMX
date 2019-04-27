#include "ServoUsingMotor.hpp"

#include "../Action.Driver/AServoUsingMotorDriver.hpp"



using namespace std;

ServoUsingMotor::ServoUsingMotor(Actions & actions) :
        AActionsElement(actions)
{
    servoMotordriver_ = AServoUsingMotorDriver::create();

    servoMotordriver_->resetEncoder(0);
    ticks_place = 350;
}


ServoUsingMotor::~ServoUsingMotor()
{
}

void ServoUsingMotor::moveRight(int nb)
{
    servoMotordriver_->setMotorPosition(500, ticks_place * nb);
}
void ServoUsingMotor::moveLeft(int nb)
{
    servoMotordriver_->setMotorPosition(-500, ticks_place * nb);
}
void ServoUsingMotor::ejectRight()
{
    servoMotordriver_->setMotorPosition(1000, ticks_place * 5);
}
void ServoUsingMotor::ejectLeft()
{
    servoMotordriver_->setMotorPosition(-1000, ticks_place * 5);
}


