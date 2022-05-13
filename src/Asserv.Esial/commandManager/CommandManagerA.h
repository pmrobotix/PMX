#ifndef ASSERV_E_COMMAND_MANAGER
#define ASSERV_E_COMMAND_MANAGER


#include "../consignController/ConsignController.h"
#include "CMDList/CMDList.h"
#include "../Utils/Utils.h"
#include "../config/config.h"

enum CommandStatus {
    STATUS_IDLE     = 0, // Auncune consigne en cours, en attente...
    STATUS_RUNNING  = 1, // Commande en cours d'exécution
    STATUS_HALTED   = 2, // Arrêt d'urgence en cours,
    STATUS_BLOCKED  = 3, // Commande en cours, mais le robot ne bouge plus!
};

class CommandManagerA
{

public:
    CommandManagerA(int capacity, ConsignController *ctrlr, Odometrie *odo);
    ~CommandManagerA();

    bool addStraightLine(int32_t valueInmm);
    bool addTurn(int32_t angleInDeg);
    bool addGoTo(int32_t posXInmm, int32_t posYInmm);
    bool addGoToBack(int32_t posXInmm, int32_t posYInmm);
    bool addGoToEnchainement(int32_t posXInmm, int32_t posYInmm);
    bool addGoToAngle(int32_t posXInmm, int32_t posYInmm);
    bool addGoToAngleReverse(int32_t posXInmm, int32_t posYInmm);
    void perform();

    // Gestion d'un éventuel arrêt d'urgence
    void setEmergencyStop();
    void resetEmergencyStop();

    // Statut des commandes
    int getCommandStatus() { return commandStatus; }
    int getPendingCmdCount();

    void perform_On(bool enable)
    {
        perform_on = enable;
    }
    bool on()
    {
        return perform_on;
    }

private:
    CMDList *liste; //File d'attente des commandes
    ConsignController *cnsgCtrl;
    Odometrie *odometrie; // Odométrie, pour pouvoir faire les conversion et calculer la cible
    CMD currCMD; //commande courante
    CMD nextCMD; //commande suivante

    bool perform_on;

    CommandStatus commandStatus;

    float computeDeltaTheta(float deltaX, float deltaY); // Calcul de l'angle à parcourir
    float computeDeltaThetaReverse(float deltaX, float deltaY); // Calcul de l'angle à parcourir en arriere
    int64_t computeDeltaDist(float deltaX, float deltaY); // Calcul de la distance à parcourir
    // GoTo là où on veut
    void computeGoTo();
    void computeGoToBack();
    void computeGoToAngle();
    void computeGoToAngleReverse();
    void computeEnchainement(); // Tentative d'enchainement
};

#endif
