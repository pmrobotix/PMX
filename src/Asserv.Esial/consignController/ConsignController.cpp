#include "ConsignController.h"

#include <stdlib.h>
#include <cstdint>

// Constructeur prenant deux objets initialisé avec l'asserv en paramètre
ConsignController::ConsignController(Odometrie *odo, MotorsController *mot) :
        angle_regu(false), dist_regu(true)
{

    // Au départ on se place à (0,0)
    angle_consigne = 0;
    dist_consigne = 0;

    // On set l'odométrie et le contrôle des moteurs
    odometrie = odo;
    motors = mot;

    // Les 2 regus sont actifs par défaut
    angle_regu_on = true;
    dist_regu_on = true;

    // On ne commence pas bloqué
    blocked_ticks = 0;

    perform_on = true;
}

// Destructeur
ConsignController::~ConsignController()
{
}

/*
 * Les consignes, que ce soit des add ou des set sont en UO !!!!
 */
void ConsignController::add_angle_consigne(int64_t deltaUO)
{
    angle_consigne += deltaUO;
}

void ConsignController::add_dist_consigne(int64_t deltaUO)
{
    dist_consigne += deltaUO;
}

void ConsignController::set_angle_consigne(int64_t consigneUO)
{
    angle_consigne = consigneUO;
}

void ConsignController::set_dist_consigne(int64_t consigneUO)
{
    dist_consigne = consigneUO;
}

// On recalcule la consigne à appliquer aux moteurs et on leur envoie l'ordre
void ConsignController::perform()
{
    if (perform_on) {
        //utils::Chronometer chrono("ConsignController::perform()");
        //chrono.start();
        //long t1 = chrono.getElapsedTimeInMicroSec();

        int32_t dist_output = 0; // Calcul de la sortie moteur en distance
        int32_t angle_output = 0; // Calcul de la sortie moteur en angle

        // Si le régulateur d'angle est actif, il doit calculer la consigne angulaire en fonction de la différence des tics codeurs (variation d'angle en UO)
        if (angle_regu_on && !Config::disableAngleRegu) {
            angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut());
        }

        //long t2 = chrono.getElapsedTimeInMicroSec();

        // Si le régu de distance est actif, il doit calculer la consigne de distance en fonction de la moyenne des tics codeurs (variation de distance en UO)
        if (dist_regu_on && !Config::disableDistanceRegu) {

            dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist());
        }
        //long t3 = chrono.getElapsedTimeInMicroSec();

        //Calcul des vitesses à appliquer en les bornant évidemment
        int32_t VmoteurD = Utils::constrain(dist_output + angle_output, Config::V_MAX_NEG_MOTOR, Config::V_MAX_POS_MOTOR);
        int32_t VmoteurG = Utils::constrain(dist_output - angle_output, Config::V_MAX_NEG_MOTOR, Config::V_MAX_POS_MOTOR);

        //long t4 = chrono.getElapsedTimeInMicroSec();
        // On donne l'ordre aux moteurs et roulez jeunesse !!
        motors->setVitesseG(VmoteurG);
        motors->setVitesseD(VmoteurD);

        //long t5 = chrono.getElapsedTimeInMicroSec();
        // On vérifie si on n'est pas bloqué. Note: on utilise les getters
        // du MotorsController parce qu'il peut mettre les vitesses à 0
        // si elles sont trop faibles.
//printf("ConsignController::perform() VitG=%d VitD=%d absA=%d absD=%d \n",motors->getVitesseG(), motors->getVitesseD(), abs(odometrie->getDeltaThetaBrut()), abs(odometrie->getDeltaDist()));
//        if ((abs(VmoteurG) >= 10 || abs(VmoteurD)  >= 10)
//                && (abs(odometrie->getDeltaThetaBrut()) < Config::BLOCK_ANGLE_SPEED_THRESHOLD
//                || abs(odometrie->getDeltaDist()) < Config::BLOCK_DIST_SPEED_THRESHOLD
//                        )
//                )
//
//            if ((abs(VmoteurG) >= 10 || abs(VmoteurD)  >= 10)
//                            && ( ((VmoteurG*VmoteurD >0) && (abs(odometrie->getDeltaDist()) < Config::BLOCK_DIST_SPEED_THRESHOLD))
//                            || ( (VmoteurG*VmoteurD <0) && (abs(odometrie->getDeltaThetaBrut()) < Config::BLOCK_ANGLE_SPEED_THRESHOLD))
//                            )
//                )
        if ((abs(VmoteurG) >= 10 || abs(VmoteurD) >= 10)
                && (((VmoteurG * VmoteurD >= 0) && (abs(odometrie->getDeltaDist()) < Config::BLOCK_DIST_SPEED_THRESHOLD))
                        || ((VmoteurG * VmoteurD <= 0)
                                && (abs(odometrie->getDeltaThetaBrut()) < Config::BLOCK_ANGLE_SPEED_THRESHOLD))))

            {
            // Bloqué !
            if (blocked_ticks < INT32_MAX) {
                // On n'incrémente pas en continue pour éviter l'overflow (au bout de 124 jours...)
                blocked_ticks++;
//                printf(" blocked_ticks++ %d  (getDeltaThetaBrut())=%lld (getDeltaDist())=%lld , (getVitesseG())=%d , (getVitesseD()=%d\n", blocked_ticks, (odometrie->getDeltaThetaBrut()), (odometrie->getDeltaDist()), motors->getVitesseG(), motors->getVitesseD() );
//                printf(" blocked_ticks++ %d  (getDeltaThetaBrut())=%lld (getDeltaDist())=%lld , VmoteurG=%d , VmoteurD=%d\n", blocked_ticks, (odometrie->getDeltaThetaBrut()), (odometrie->getDeltaDist()), VmoteurG, VmoteurD );
            }
        } else {
            // Moteurs arrêtés ou robot qui bouge: on n'est pas bloqué
            blocked_ticks = 0;
        }

        //long t6 = chrono.getElapsedTimeInMicroSec();

        //printf("t2-t1=%ld t3-t2=%ld t4-t3=%ld t5-t4=%ld t6-t5=%ld \n", t2-t1, t3-t2, t4-t3, t5-t4, t6-t5);
    }
}

void ConsignController::setLeftSpeed(int vit)
{
    motors->setVitesseG(vit);
}

void ConsignController::setRightSpeed(int vit)
{
    motors->setVitesseD(vit);
}

void ConsignController::setLowSpeed(bool b)
{
    setLowSpeedForward(b, Config::DIST_QUAD_AV_LOW_DIV);
    setLowSpeedBackward(b, Config::DIST_QUAD_AR_LOW_DIV);
}


void ConsignController::setLowSpeedForward(bool b, int percent)
{
    if (b) {
        dist_regu.setVitesseMarcheAvant(Config::DIST_QUAD_1ST_POS * percent / 100.0);
    } else {
        dist_regu.setVitesseMarcheAvant(Config::DIST_QUAD_1ST_POS);
    }
}
void ConsignController::setLowSpeedBackward(bool b, int percent)
{
    if (b) {
        dist_regu.setVitesseMarcheArriere(Config::DIST_QUAD_1ST_NEG * percent / 100.0);
    } else {
        dist_regu.setVitesseMarcheArriere(Config::DIST_QUAD_1ST_NEG);
    }
}

bool ConsignController::isBlocked()
{

    // Si on est bloqué pendant un certain temps, on le signale
    bool b = (blocked_ticks >= Config::BLOCK_TICK_THRESHOLD);
//    printf("isBlocked %d  %d\n", blocked_ticks, Config::BLOCK_TICK_THRESHOLD);
//    if (b) {
//        //blocked_ticks = 0;
//        printf("ConsignController::isBlocked() true\n");
//    }
    if (b) {
        blocked_ticks = 0;
    }
    return b;
}
