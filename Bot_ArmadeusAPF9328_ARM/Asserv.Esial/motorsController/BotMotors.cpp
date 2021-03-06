#if CONFIG_MOTORCTRL_BOTMOTORS

#include "../../Common/Asserv/Asserv.hpp"
#include "../../Common/Asserv/MotorControl.hpp"
#include "../../Common/Asserv/MovingBase.hpp"
#include "../../Common/Robot.hpp"
#include "../config/config.h"

#include "BotMotors.h"

// Constructeur d'une instance
BotMotors::BotMotors(Robot * robot)
{
    robot_ = robot; //Reference vers le robot
    vitMoteurG = 0;
    vitMoteurD = 0;
}

BotMotors::~BotMotors()
{
    // On arrête les moteurs à la destruction
    setVitesseG(0);
    setVitesseD(0);
}

void BotMotors::setVitesseG(int vitMoteurG)
{
    if (Config::reglageCodeurs)
        return;

    if (Config::inverseMoteurG) {
        vitMoteurG = -vitMoteurG;
    }

    if (vitMoteurG > Config::V_MAX_POS_MOTOR) {
        vitMoteurG = Config::V_MAX_POS_MOTOR;
    } else if (vitMoteurG > 0 && vitMoteurG < Config::V_MIN_POS_MOTOR) {
        vitMoteurG = 0;
    }

    if (vitMoteurG < Config::V_MAX_NEG_MOTOR) {
        vitMoteurG = Config::V_MAX_NEG_MOTOR;
    } else if (vitMoteurG < 0 && vitMoteurG > Config::V_MIN_NEG_MOTOR) {
        vitMoteurG = 0;
    }

    //Apply
    robot_->logger().debug() << "setVitesseG = " << vitMoteurG << logs::end;
    robot_->asserv_default->base()->motors().runMotorLeft(vitMoteurG, 0);

}

void BotMotors::setVitesseD(int vitMoteurD)
{
    if (Config::reglageCodeurs)
        return;

    if (Config::inverseMoteurD) {
        vitMoteurD = -vitMoteurD;
    }

    if (vitMoteurD > Config::V_MAX_POS_MOTOR) {
        vitMoteurD = Config::V_MAX_POS_MOTOR;
    } else if (vitMoteurD > 0 && vitMoteurD < Config::V_MIN_POS_MOTOR) {
        vitMoteurD = 0;
    }

    if (vitMoteurD < Config::V_MAX_NEG_MOTOR) {
        vitMoteurD = Config::V_MAX_NEG_MOTOR;
    } else if (vitMoteurD < 0 && vitMoteurD > Config::V_MIN_NEG_MOTOR) {
        vitMoteurD = 0;
    }

    //APPLY
    robot_->logger().debug() << "vitMoteurD = " << vitMoteurD << logs::end;
    robot_->asserv_default->base()->motors().runMotorRight(vitMoteurD, 0);

}

#endif /* CONFIG_MOTORCTRL_BOTMOTORS */
