#include "QuadRampDerivee.h"

#include <cstdio>
#include <cstdlib>

#include "../../config/config.h"

//--------------------------------------------------------
//                      QUADRAMPm
//-------------------------------------------------------
QuadRampDerivee::QuadRampDerivee(bool isDistance)
{
    if (isDistance) {
        // Dérivées premières de la consigne ( dans notre cas, la vitesse )
        derivee_1ier_pos = Config::DIST_QUAD_1ST_POS;
        derivee_1ier_neg = Config::DIST_QUAD_1ST_NEG;

        // Dérivées secondes de la consigne ( dans notre cas, l'accélération )
        derivee_2nd_pos_av = Config::DIST_QUAD_AV_2ND_ACC;
        derivee_2nd_neg_av = Config::DIST_QUAD_AV_2ND_DEC;

        //la même chose pour le déplacement vers l'arrière
        derivee_2nd_pos_ar = Config::DIST_QUAD_AR_2ND_ACC;
        derivee_2nd_neg_ar = Config::DIST_QUAD_AR_2ND_DEC;

        //On ajoute de l' "anticipation " pour éviter que la vitesse ne chute brutalement à zéro quand le point d'arrivée arrive....( bien pourri ca dis donc -_-" )
        gainAnticipation_av = Config::DIST_QUAD_AV_ANTICIPATION_GAIN_COEF;
        gainAnticipation_ar = Config::DIST_QUAD_AR_ANTICIPATION_GAIN_COEF;

        //Pour éviter les vibrations à l'arrivée
        tailleFenetreArrivee = Config::DIST_TAILLE_FENETRE_ARRIVEE;
    } else {
        // Dérivées premières de la consigne ( dans notre cas, la vitesse ) et le sens n'a pas d'importance en rotation
        derivee_1ier_pos = Config::ANGLE_QUAD_1ST_POS;
        derivee_1ier_neg = Config::ANGLE_QUAD_1ST_POS; // pas de NEG pour l'angle

        // Dérivées secondes de la consigne ( dans notre cas, l'accélération )
        derivee_2nd_pos_av = Config::ANGLE_QUAD_2ND_ACC;
        derivee_2nd_neg_av = Config::ANGLE_QUAD_2ND_DEC;

        //la même chose pour le déplacement vers l'arrière et le sens n'a pas d'importance en rotation
        derivee_2nd_pos_ar = Config::ANGLE_QUAD_2ND_ACC;
        derivee_2nd_neg_ar = Config::ANGLE_QUAD_2ND_DEC;

        //On ajoute de l' "anticipation " pour éviter que la vitesse ne chute brutalement à zéro quand le point d'arrivée arrive....( bien pourri ca dis donc -_-" )
        gainAnticipation_av = Config::ANGLE_QUAD_ANTICIPATION_GAIN_COEF;
        gainAnticipation_ar = Config::ANGLE_QUAD_ANTICIPATION_GAIN_COEF;

        //Pour éviter les vibrations à l'arrivée
        tailleFenetreArrivee = Config::ANGLE_TAILLE_FENETRE_ARRIVEE;
    }

    // La vitesse initiale est nulle
    prevConsigneVitesse = 0;
    // Comme il n'y a pas encore de consigne, on est arrivé
    arrivee = true;
}

// Destructeur
QuadRampDerivee::~QuadRampDerivee()
{
}

// On filtre la consigne pour prendre en compte l'accélération et la décélération
int64_t QuadRampDerivee::filtre(int64_t consigne, int64_t position_actuelle, int64_t vitesse)
{
    arrivee = false;

    int64_t reste = consigne - position_actuelle;
    printf("reste=%ld   consigne=%ld  pos=%ld\n", reste, consigne, position_actuelle);
    if (llabs(reste) < tailleFenetreArrivee) {
        prevConsigneVitesse = 0; // On reset la consigne precedente
        arrivee = true;
        printf("ARRIVED !!! reste=%ld   consigne=%ld  pos=%ld\n", reste, consigne, position_actuelle);
        return consigne;
    }

    char sens = (reste >= 0) ? 1 : -1;

    // 0 ---- X1 ---------- X2 ---- consigne

    // calcul la distance que parcourerait le robot si on freine a fond
    float deltaT = 0.020f * 10000000.0f*1.1f;
    float distanceFreinage = (vitesse * deltaT) / derivee_2nd_neg_av;

    int64_t vitesseConsigne = prevConsigneVitesse;
    int64_t accelerationConsigne = 0;
    if (distanceFreinage < reste) {
        // on accelere
        accelerationConsigne = derivee_2nd_pos_av;

    } else {
        // on decelere
        accelerationConsigne = -derivee_2nd_neg_av;

    }

    vitesseConsigne += accelerationConsigne;

    // limitation de la vitesse
    int64_t vitesseConsigneLimitee = Utils::constrain(vitesseConsigne, -derivee_1ier_neg, derivee_1ier_pos);

    prevConsigneVitesse = vitesseConsigneLimitee;

    int64_t positionConsigne = position_actuelle + vitesseConsigneLimitee;
    if ( positionConsigne > consigne) {
        positionConsigne = consigne + 1;
    }

    printf(
            "consigne=%ld \tpos=%ld \tvitesse=%ld \tdistanceFreinage=%f \tpositionConsigne=%ld \tvitesseConsigne=%ld \taccelerationConsigne=%ld \tvitesseConsigneLimitee=%ld \n",
            consigne, position_actuelle, vitesse, distanceFreinage, positionConsigne, vitesseConsigne,
            accelerationConsigne, vitesseConsigneLimitee);

    return positionConsigne;

//    if(position_actuelle<200 && vitesse<=0){
//           vitesse=20;
//       }
//
//
//       //Calcul de la position du pivot qui sert à déterminer si l'on doit commencer à décélérer ou non
//       char sens = (consigne - position_actuelle >= 0) ? 1 : -1;
//
//       // Reset du flag "arrivee" signalant que la consigne est atteinte
//       arrivee = false;
//       float a1=(float)vitesse/(float)derivee_2nd_pos_av;
//       float a2=(float)vitesse/(float)derivee_2nd_neg_av;
//
//       float a1r=(float)vitesse/(float)derivee_2nd_pos_ar;
//       float a2r=(float)vitesse/(float)derivee_2nd_neg_ar;
//
//
//       int64_t position_pivot_cho;
//       if (sens == 1) {
//           position_pivot_cho = (a1 * consigne) / (a1 + a2);
//       } else {
//           position_pivot_cho = (a1r * consigne) / (a1r + a2r);
//       }
//       //printf("position_pivot_cho=%lld\n", position_pivot_cho);
//       int32_t consigne_temp= ((consigne * 1.0 / (4000 * 2048)) * 1000.0);
//
//
//
//       //Calcul de la consigne d'accélération qui dépend dans le sens dans lequel on roule et vient de config.h
//       int64_t acceleration_consign;
//
//       if (position_actuelle <= position_pivot_cho) {
//           acceleration_consign = (sens == 1) ? derivee_2nd_pos_av : derivee_2nd_neg_ar;
//       } else {
//           acceleration_consign = (sens == 1) ? -derivee_2nd_neg_av : -derivee_2nd_pos_ar;
//       }
//
//       // Calcul de la consigne de vitesse
//       int64_t consigneVitesse = prevConsigneVitesse + acceleration_consign;
//       //int64_t consigneVitesse = vitesse + acceleration_consign;
//       printf("consigne=%lld %dmm \tpos=%lld \tvitesse=%lld \tpivot=%lld \tarrived=%lld a1=%f a2=%f  consigneVitesse=%lld prevConsigneVitesse=%lld acceleration_consign=%lld\n",consigne, consigne_temp ,position_actuelle,vitesse, position_pivot_cho, consigne - position_actuelle,a1, a2, consigneVitesse, prevConsigneVitesse, acceleration_consign);
//
//
//       //int64_t consigneVitesse =  consigne;
//       // On borne la consigne, parce que faut éviter de trop déconner en atelier // limitation vitesse
//       consigneVitesse = Utils::constrain(consigneVitesse, -derivee_1ier_neg, derivee_1ier_pos);
//       // On stocke la nouvelle consigne pour l'itération suivante
//       prevConsigneVitesse = consigneVitesse;
//
//       //printf("consigne=%lld position_pivot=%lld\r\n", consigne, position_pivot);
//
//       // On vérifie si on est dans la fenêtre d'arrivée et si oui, on est arrivé à la fin de la rampe
//       if (llabs(consigne - position_actuelle) < tailleFenetreArrivee) {
//           prevConsigneVitesse = 0; // On reset la consigne precedente
//           arrivee = true;
//           return consigne;
//       }
//
//       //On retourne la consigne de position
//       return position_actuelle + consigneVitesse;

}

// On filtre la consigne pour prendre en compte l'accélération et la décélération
int64_t QuadRampDerivee::filtre_old(int64_t consigne, int64_t position_actuelle, int64_t vitesse)
{

    // Reset du flag "arrivee" signalant que la consigne est atteinte
    arrivee = false;

    //Calcul de la position du pivot qui sert à déterminer si l'on doit commencer à décélérer ou non
    char sens = (consigne - position_actuelle >= 0) ? 1 : -1;
    int64_t position_pivot;

    if (sens == 1) {
        //position_pivot = consigne + ((vitesse >= 0) ? -1 : 1) * (((vitesse * vitesse) / (2 * derivee_2nd_neg_av)) + llabs(vitesse) * gainAnticipation_av);
        position_pivot = consigne
                + ((vitesse >= 0) ? -1 : 1)
                        * (((vitesse * vitesse) / (2 * derivee_2nd_neg_av)) + llabs(vitesse) * gainAnticipation_av);

    } else {
        position_pivot = consigne
                + ((vitesse >= 0) ? -1 : 1)
                        * (((vitesse * vitesse) / (2 * derivee_2nd_neg_ar)) + llabs(vitesse) * gainAnticipation_ar);
    }

    //Calcul de la consigne d'accélération qui dépend dans le sens dans lequel on roule et vient de config.h
    int64_t acceleration_consign;

    if (position_pivot >= position_actuelle) {
        acceleration_consign = (sens == 1) ? derivee_2nd_pos_av : derivee_2nd_neg_ar;
    } else {
        acceleration_consign = (sens == 1) ? -derivee_2nd_neg_av : -derivee_2nd_pos_ar;
    }

    // Calcul de la consigne de vitesse
    int64_t consigneVitesse = prevConsigneVitesse + acceleration_consign;
    // On borne la consigne, parce que faut éviter de trop déconner en atelier // limitation vitesse
    consigneVitesse = Utils::constrain(consigneVitesse, -derivee_1ier_neg, derivee_1ier_pos);
    // On stocke la nouvelle consigne pour l'itération suivante
    prevConsigneVitesse = consigneVitesse;

    //printf("consigne=%lld position_pivot=%lld\r\n", consigne, position_pivot);
    //printf("pivot=%lld consigne=%lld position_actuelle+consigneVitesse=%lld\n", position_pivot, consigne ,position_actuelle + consigneVitesse);

    // On vérifie si on est dans la fenêtre d'arrivée et si oui, on est arrivé à la fin de la rampe
    if (llabs(consigne - position_actuelle) < tailleFenetreArrivee) {
        prevConsigneVitesse = 0; // On reset la consigne precedente
        arrivee = true;
        return consigne;
    }

    //On retourne la consigne de position
    return position_actuelle + consigneVitesse;
}
