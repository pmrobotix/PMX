#include "ConsignController.h"

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

        int32_t dist_output = 0; // Calcul de la sortie moteur en distance
        int32_t angle_output = 0; // Calcul de la sortie moteur en angle

        // Si le régulateur d'angle est actif, il doit calculer la consigne angulaire en fonction de la différence des tics codeurs (variation d'angle en UO)
        if (angle_regu_on && !Config::disableAngleRegu) {
            angle_output = angle_regu.manage(angle_consigne, odometrie->getDeltaThetaBrut());
        }

        // Si le régu de distance est actif, il doit calculer la consigne de distance en fonction de la moyenne des tics codeurs (variation de distance en UO)
        if (dist_regu_on && !Config::disableDistanceRegu) {

            dist_output = dist_regu.manage(dist_consigne, odometrie->getDeltaDist());
//printf("dist_output=%d\n", dist_output);
        }

        //Calcul des vitesses à appliquer en les bornant évidemment
        int32_t VmoteurD = Utils::constrain(dist_output + angle_output, Config::V_MAX_NEG_MOTOR, Config::V_MAX_POS_MOTOR);
        int32_t VmoteurG = Utils::constrain(dist_output - angle_output, Config::V_MAX_NEG_MOTOR, Config::V_MAX_POS_MOTOR);

        // On donne l'ordre aux moteurs et roulez jeunesse !!
        motors->setVitesseG(VmoteurG);
        motors->setVitesseD(VmoteurD);

    // On vérifie si on n'est pas bloqué. Note: on utilise les getters
    // du MotorsController parce qu'il peut mettre les vitesses à 0
    // si elles sont trop faibles.
    if( motors->getVitesseG() != 0 && motors->getVitesseD() != 0
        && abs(odometrie->getDeltaThetaBrut()) < Config::BLOCK_ANGLE_SPEED_THRESHOLD
        && abs(odometrie->getDeltaDist()) < Config::BLOCK_DIST_SPEED_THRESHOLD )
    {
        // Bloqué !
        if(blocked_ticks < INT32_MAX) {
            // On n'incrémente pas en continue pour éviter l'overflow (au bout de 124 jours...)
            blocked_ticks++;
        }
    }
    else {
        // Moteurs arrêtés ou robot qui bouge: on n'est pas bloqué
        blocked_ticks = 0;
    }

        //printf("VG=%ld  \tVD=%ld\r\r\n", VmoteurG, VmoteurD);
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
    setLowSpeed(b,Config::DIST_QUAD_AR_LOW_DIV,Config::DIST_QUAD_AV_LOW_DIV);
}

void ConsignController::setLowSpeed(bool b, unsigned char factor_div_back, unsigned char factor_div_forward)
{
    if (b) {
        dist_regu.setVitesseMarcheArriere(Config::DIST_QUAD_1ST_NEG / factor_div_back);
        dist_regu.setVitesseMarcheAvant(Config::DIST_QUAD_1ST_POS / factor_div_forward);
    } else {
        dist_regu.setVitesseMarcheArriere(Config::DIST_QUAD_1ST_NEG);
        dist_regu.setVitesseMarcheAvant(Config::DIST_QUAD_1ST_POS);
    }
}

bool ConsignController::isBlocked()
{
    // Si on est bloqué pendant un certain temps, on le signale
    return blocked_ticks >= Config::BLOCK_TICK_THRESHOLD;
}
