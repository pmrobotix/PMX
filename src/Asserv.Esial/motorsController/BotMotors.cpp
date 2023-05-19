#include "../../Log/Logger.hpp"

#if CONFIG_MOTORCTRL_BOTMOTORS

#include "../../Common/Asserv/Asserv.hpp"
#include "../../Common/Asserv/MotorControl.hpp"
#include "../../Common/Robot.hpp"
#include "../config/config.h"

#include "BotMotors.h"

// Constructeur d'une instance
BotMotors::BotMotors(Robot * robot)
{
    robot_ = robot; //Reference vers le robot
    vitMoteurG_ = 0;
    vitMoteurD_ = 0;
}

BotMotors::~BotMotors()
{
    // On arrête les moteurs à la destruction
    setVitesseG(0);
    setVitesseD(0);
}

void BotMotors::setVitesseG(int vitMoteurG)
{
    //int temp = vitMoteurG;
    if (Config::reglageCodeurs)
        return;

    if (vitMoteurG > Config::V_MAX_POS_MOTOR) {
        vitMoteurG = Config::V_MAX_POS_MOTOR;
    } else if (vitMoteurG > 0 && vitMoteurG < Config::V_MIN_POS_MOTOR) {
        int delta = Config::V_MIN_POS_MOTOR - vitMoteurG;
        //vitMoteurG += (delta * 2) / 3;
        vitMoteurG += (delta / 2.0);
    }

    if (vitMoteurG < Config::V_MAX_NEG_MOTOR) {
        vitMoteurG = Config::V_MAX_NEG_MOTOR;
    } else if (vitMoteurG < 0 && vitMoteurG > Config::V_MIN_NEG_MOTOR) {
        //vitMoteurG = Config::V_MIN_NEG_MOTOR;
        int delta = Config::V_MIN_NEG_MOTOR - vitMoteurG;
        //vitMoteurG += (delta * 2) / 3;
        vitMoteurG += (delta / 2.0);
    }

    vitMoteurG_ = vitMoteurG;

    if (Config::inverseMoteurG) {
        vitMoteurG = -vitMoteurG;
    }

    //Apply
    //robot_->logger().error() << "vit = " << temp << "  setVitesseG = " << vitMoteurG << logs::end;
    robot_->passerv()->runMotorLeft(vitMoteurG, -1);
}

void BotMotors::setVitesseD(int vitMoteurD)
{
    //int temp = vitMoteurD;

    if (Config::reglageCodeurs)
        return;

    if (vitMoteurD > Config::V_MAX_POS_MOTOR) {
        vitMoteurD = Config::V_MAX_POS_MOTOR;
    } else if (vitMoteurD > 0 && vitMoteurD < Config::V_MIN_POS_MOTOR) {
        //vitMoteurD = Config::V_MIN_POS_MOTOR;
        int delta = Config::V_MIN_POS_MOTOR - vitMoteurD;
        //vitMoteurD += (delta * 2) / 3;
        vitMoteurD += (delta / 2.0);
    }

    if (vitMoteurD < Config::V_MAX_NEG_MOTOR) {
        vitMoteurD = Config::V_MAX_NEG_MOTOR;
    } else if (vitMoteurD < 0 && vitMoteurD > Config::V_MIN_NEG_MOTOR) {
        //vitMoteurD = Config::V_MIN_NEG_MOTOR;
        int delta = Config::V_MIN_NEG_MOTOR - vitMoteurD;
        //vitMoteurD += (delta * 2) / 3;
        vitMoteurD += (delta / 2.0);
    }

    vitMoteurD_ = vitMoteurD;

    if (Config::inverseMoteurD) {
        vitMoteurD = -vitMoteurD;
    }

    //APPLY
    //robot_->logger().error() << "vit = " << temp << "  setVitesseD = " << vitMoteurD << logs::end;
    robot_->passerv()->runMotorRight(vitMoteurD, -1);
}

#endif /* CONFIG_MOTORCTRL_BOTMOTORS */
