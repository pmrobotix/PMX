#include "ServoUsingMotor.hpp"

#include "../Interface.Driver/AServoUsingMotorDriver.hpp"


using namespace std;

ServoUsingMotor::ServoUsingMotor(std::string botId, Actions & actions) :
        AActionsElement(actions), botId_(botId)
{
    servoMotordriver_ = AServoUsingMotorDriver::create();

    servoMotordriver_->resetEncoder(0);

    //ticks_place = 400; //pour 1000 - 350  //TODO a separer par robot !!!
}

ServoUsingMotor::~ServoUsingMotor()
{
    delete servoMotordriver_;
}

bool ServoUsingMotor::is_connected()
{
    return servoMotordriver_->is_connected();
}

void ServoUsingMotor::turn(int power)
{
    servoMotordriver_->setMotorPosition(power,0,0);
}

void ServoUsingMotor::stop()
{
    servoMotordriver_->stopMotor();

}
//void ServoUsingMotor::moveRight(int nb)
//{
//    servoMotordriver_->setMotorPosition(500, ticks_place * nb / 1000.0, 1000);
//}
//void ServoUsingMotor::moveLeft(int nb)
//{
//    servoMotordriver_->setMotorPosition(-500, -ticks_place * nb / 1000.0, 1000);
//}
//void ServoUsingMotor::ejectRight()
//{
//    servoMotordriver_->setMotorPosition(860, ticks_place * 7);
//}
//void ServoUsingMotor::ejectLeft()
//{
//    servoMotordriver_->setMotorPosition(-860, -ticks_place * 7);
//}


