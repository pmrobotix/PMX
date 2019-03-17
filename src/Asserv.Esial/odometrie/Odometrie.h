#ifndef ODOMETRIE
#define ODOMETRIE



#include "../config/config.h"
#include "../Utils/Utils.h"
#include "../codeurs/CodeursInterface.h"


class Odometrie
{

public:
    // Constructeur
    /*
     * Lors de la création de l'objet, on calcul la distance entre les roues en UO et le nombre d'UO par front
     * Les infos nécessaires au calcul sont dans config.h
     */
    Odometrie(CodeursInterface *cdrs);
    ~Odometrie();

    // Paramétrage de la position du robot
    void setX(int64_t xval); // En UO
    void setY(int64_t yval); // En UO
    void setTheta(double thetaVal); // En radians

    // Mise à jour de la position du robot
    void refresh();

    //Position en unités arbritraires ( Unite Odometrie [UO] pour les distances, et rad pour l'angle)
    /*
     * La classe Utils contient les méthodes de conversions pour que les mesures soient humainement compréhensible
     */
    int64_t getX()
    {
        printf("\ngetXmm()= %d \n", getXmm());
        return x;   // Renvoie la position en X en UO par rapport au point de départ
    }
    int64_t getY()
    {
        return y;   // Renvoie la position en Y en UO par rapport au point de départ
    }
    double getTheta()
    {
        return theta;   // Renvoie l'angle en radians par rapport au cap de départ
    }

    int32_t getXmm()
    {
        return Utils::UOTomm(this, x); // Renvoie la position en X en mm par rapport au point de départ
    }
    int32_t getYmm()
    {
        return Utils::UOTomm(this, y);   // Renvoie la position en Y en mm par rapport au point de départ
    }

    // Variation de la position depuis la derniàre mise à jour
    double getDeltaTheta()
    {
        return deltaTheta;    // Variation du cap du robot
    }
    int64_t getDeltaDist()
    {
        return deltaDist;    // Variation de la distance du robot
    }
    int64_t getDeltaThetaBrut()
    {
        return compteurD - compteurG; // Variation d'angle en UO donnée par la différence de compte entre les codeurs
    }

    // Getteurs config
    double getFrontParMetre()
    {
        return frontParMetre;
    }
    int64_t getDistanceRouesUO()
    {
        return distanceRouesUO;
    }

private:

    // Données sur les codeurs
    double frontParMetre; // Nombre de tics codeurs pour un mètre parcouru
    int64_t distanceRouesUO; // Distance entre les roues en UO
    double ratioCodeurs; // Ratio frontParMetreCodeurMax / frontParMetreCodeurMin pour compenser la différence physique entre codeur
    bool applyRatioOnG; // Si vrai, le ratio s'applique au codeur Gauche, sinon le Droit

    // Position actuelle
    int64_t x, y; //En UO
    double theta; //En radian

    // Variation pour la mise a jour en cours
    int64_t deltaDist; // En UO
    double deltaTheta; //En radian
    int64_t compteurG; // Nombre de tics codeur G depuis dernier refresh * UO par front
    int64_t compteurD; // Nombre de tics codeur D depuis dernier refresh * UO par front

    //Codeurs
    CodeursInterface* codeurs;

};

#endif
