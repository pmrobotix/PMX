#ifndef COMMAND_MANAGER
#define COMMAND_MANAGER


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

class CommandManager
{

public:
    CommandManager(int capacity, ConsignController *ctrlr, Odometrie *odo);
    ~CommandManager();

    bool addStraightLine(int32_t valueInmm);
    bool addTurn(int32_t angleInDeg);
    bool addGoTo(int32_t posXInmm, int32_t posYInmm);
    bool addGoToBack(int32_t posXInmm, int32_t posYInmm);
    bool addGoToEnchainement(int32_t posXInmm, int32_t posYInmm);
    bool addGoToAngle(int32_t posXInmm, int32_t posYInmm);
    void perform();

    // Gestion d'un éventuel arrêt d'urgence
    void setEmergencyStop();
    void resetEmergencyStop();

    // Statut des commandes
    int getCommandStatus() { return commandStatus; }
    int getPendingCommandCount();

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

    double computeDeltaTheta(double deltaX, double deltaY); // Calcul de l'angle à parcourir
    int64_t computeDeltaDist(double deltaX, double deltaY); // Calcul de la distance à parcourir
    // GoTo là où on veut
    void computeGoTo();
    void computeGoToBack();
    void computeGoToAngle();
    void computeEnchainement(); // Tentative d'enchainement
};

#endif
