#include "LegoEV3ActionsExtended.hpp"

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/Logger.hpp"

LegoEV3ActionsExtended::LegoEV3ActionsExtended(std::string botId, Robot * robot) :
        ledbar_(botId, *this, 2), buttonbar_(*this), soundbar_(*this), tirette_(*this), sensors_(*this, robot), lcd_(botId, *this),
        servos_std_(*this, AServoDriver::SERVO_STANDARD), servoUsingMotor_(*this)
{
    logger().debug() << "LegoEV3ActionsExtended()" << logs::end;

    releaseAll();

    servosStd().setSpeed(SERVO_7, 200);
    servosStd().setSpeed(SERVO_5, 200);
    servosStd().setSpeed(SERVO_1, 400);

    servosStd().setMinPulse(SERVO_1, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_1, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_1, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_2, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_2, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_2, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_3, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_3, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_3, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_4, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_4, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_4, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_5, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_5, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_5, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_6, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_6, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_6, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_7, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_7, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_7, 2700); //default 2400 [2300 to 2700]

    servosStd().setMinPulse(SERVO_8, 300); ////default 600 [300 to 700]
    servosStd().setMidPulse(SERVO_8, 1500); //default 1500 [1300 to 1700]
    servosStd().setMaxPulse(SERVO_8, 2700); //default 2400 [2300 to 2700]

    /*
     //Servo config specifique
     servosStd().setPolarity(0);
     servosStd().setMinPulse(0, 600);
     servosStd().setMidPulse(0, 1470);
     servosStd().setMaxPulse(0, 2350);*/



}
