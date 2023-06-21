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
    // VERIF LCD + BUTTONS
    if (!lcd2x16_.is_connected()) {
        //Allumer une led d'error
        ledbar_.set(0, LED_GREEN);
        logger().error() << "lcd2x16 is NOT connected !" << logs::end;
    }
    lcd2x16_.init();

    if (!tirette_.is_connected()) {
        //Allumer une led d'error
        ledbar_.set(1, LED_GREEN);
        logger().error() << "tirette is NOT connected !" << logs::end;
    }

    if (!sensors_.is_connected()) {
        //Allumer une led d'error
        ledbar_.set(2, LED_GREEN);
        logger().error() << "sensors is NOT connected !" << logs::end;
    }

    if (!lanceurCerises_.is_connected()) {
        //Allumer une led d'error
        ledbar_.set(3, LED_GREEN);
        logger().error() << "lanceurCerises(MD25) is NOT connected !" << logs::end;
    }

    //TODO init des OBJETS si CONNECTE SINON ALLUMER UNE LED

    //config servos STD
    //servos().setup(STD_SERVO_3, AServoDriver::ServoType::SERVO_STANDARD, 600, 1500, 2500, false);

    //config AX12 2023
    int svrconnected = 0;
    svrconnected = servos().setup(AX12_SERVO_BRAS_D, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    svrconnected &= servos().setup(AX12_SERVO_BRAS_G, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    if (svrconnected == false) {
        ledbar_.set(4, LED_GREEN);
    }

    svrconnected = servos().setup(AX12_SERVO_ASPIRATION, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023,
            false);
    if (svrconnected == false) {
        ledbar_.set(5, LED_GREEN);
    }

    svrconnected = servos().setup(AX12_SERVO_FUNNY, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
    if (svrconnected == false) {
        ledbar_.set(6, LED_GREEN);
    }

//    svrconnected = servos().setup(AX12_SERVO_TETE_ASPI, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023, false);
//    svrconnected &= servos().setup(AX18_SERVO_RUSSEL_LINKAGE, AServoDriver::ServoType::SERVO_DYNAMIXEL, 0, 512, 1023,
//            false);
//    if (svrconnected == false) {
//        ledbar_.set(7, LED_GREEN);
//    }


    //config des sensors
    //TODO ajouter la configuration de la position des capteurs sensors_.addConfigPosFront(-140, 0, +140);

    sensors_.addConfigFront(false, true, false);
    sensors_.addConfigBack(false, false, false);

    sensors_.addThresholdFront(450, 140 + 450 + 140, 450);
    sensors_.addThresholdFrontVeryClosed(200, 140 + 200 + 140, 200);

    sensors_.addThresholdBack(0, 140 + 350 + 140, 0);
    sensors_.addThresholdBackVeryClosed(0, 140 + 70 + 140, 0);

}

