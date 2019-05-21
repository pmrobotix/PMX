#include "ServoUsingMotor.hpp"

#include "../Action.Driver/AServoUsingMotorDriver.hpp"


using namespace std;

ServoUsingMotor::ServoUsingMotor(Actions & actions) :
        AActionsElement(actions)
{
    servoMotordriver_ = AServoUsingMotorDriver::create();

    servoMotordriver_->resetEncoder(0);

    ticks_place = 400; //pour 1000 - 350  //TODO a separer par robot !!!
}

ServoUsingMotor::~ServoUsingMotor()
{
    delete servoMotordriver_;
}

void ServoUsingMotor::moveRight(int nb)
{
    servoMotordriver_->setMotorPosition(500, ticks_place * nb / 1000.0, 1000);
}
void ServoUsingMotor::moveLeft(int nb)
{
    servoMotordriver_->setMotorPosition(-500, -ticks_place * nb / 1000.0, 1000);
}
void ServoUsingMotor::ejectRight()
{
    servoMotordriver_->setMotorPosition(860, ticks_place * 6);
}
void ServoUsingMotor::ejectLeft()
{
    servoMotordriver_->setMotorPosition(-860, -ticks_place * 6);
}


