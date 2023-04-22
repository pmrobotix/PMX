#include "OPOS6UL_ActionsExtended.hpp"

OPOS6UL_ActionsExtended::OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
        ledbar_(botId, *this, 8),
        buttonbar_(*this),
        lcd2x16_(botId, *this),
        tirette_(*this),
        sensors_(*this, robot),
        servos_(botId, *this),
        lanceurCerises_(botId, *this)

{
    lcd2x16_.init();

    //config servos STD
    //servos().setup(STD_SERVO_3, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);

/*
    //config AX12 2022
    servos().setup(AX12_SERVO_BRAS_D, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_BRAS_G, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_ELEVATOR_L, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_ELEVATOR_R, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_ARM_L_BOTTOM, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_ARM_R_BOTTOM, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_ARM_L_TOP, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
    servos().setup(AX12_SERVO_ARM_R_TOP, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1024, false);
*/
    //config AX12 2023
    servos().setup(AX12_SERVO_BRAS_D, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    servos().setup(AX12_SERVO_BRAS_G, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    servos().setup(AX12_SERVO_ASPIRATION, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    servos().setup(AX12_SERVO_FUNNY, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    servos().setup(AX12_SERVO_TETE_ASPI, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    servos().setup(AX18_SERVO_RUSSEL_LINKAGE, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);

    //config des sensors
    //TODO ajouter la configuration de la position des capteurs sensors_.addConfigPosFront(-140, 0, +140);

    sensors_.addConfigFront(false, true, false);
    sensors_.addConfigBack(false, false, false);

    sensors_.addThresholdFront(450, 550, 450);
    sensors_.addThresholdFrontVeryClosed(200, 300, 200);

    sensors_.addThresholdBack(0, 220, 0);
    sensors_.addThresholdBackVeryClosed(0, 160, 0);

}

