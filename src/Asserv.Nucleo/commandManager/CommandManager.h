#ifndef ASSERV_NUCLEO_COMMAND_MANAGER
#define ASSERV_NUCLEO_COMMAND_MANAGER

#include <cstdint>

#include "CommandList.h"
#include "Commands/Goto.h"
#include "Commands/GotoNoStop.h"

class Command;
class AccelerationDecelerationLimiter;
class BlockingDetector;


class CommandManager
{
    public:

        typedef enum {
            STATUS_IDLE     = 0,
            STATUS_RUNNING  = 1,
            STATUS_HALTED   = 2,
            STATUS_BLOCKED  = 3,
        } CommandStatus;

        explicit CommandManager(float straitLineArrivalWindows_mm, float turnArrivalWindows_rad,
                Goto::GotoConfiguration &preciseGotoConfiguration, Goto::GotoConfiguration &waypointGotoConfiguration, GotoNoStop::GotoNoStopConfiguration &gotoNoStopConfiguration,
                const Regulator &angle_regulator, const Regulator &distance_regulator,
                AccelerationDecelerationLimiter *accelerationDecelerationLimiter = nullptr,
                BlockingDetector *blockingDetector = nullptr);
        ~CommandManager() {};

        /*
         * Commandes ajoutables a la liste des consignes du robot
         */
        bool addStraightLine(float valueInmm);
        bool addTurn(float angleInDeg);
        bool addGoTo(float posXInmm, float posYInmm);
        bool addGoToWaypoint(float posXInmm, float posYInmm);
        bool addGoToBack(float posXInmm, float posYInmm);
        bool addGoToNoStop(float posXInmm, float posYInmm);
        bool addGoToNoStopBack(float posXInmm, float posYInmm);
        bool addGoToAngle(float posXInmm, float posYInmm);

        /*
         * Gestion de l'arret d'urgence
         */
        void setEmergencyStop();
        void resetEmergencyStop();

        /*
         * Mise à jour des consignes de sorties en fonction
         *     de la nouvelle position du robot
         */
        void update(float X_mm, float Y_mm, float theta_rad);

        /*
         * Sorties du commandManager
         */
        float getDistanceGoal()
        {
            return m_distRegulatorConsign;
        }
        float getAngleGoal()
        {
            return m_angleRegulatorConsign;
        }

        /*
         * Permet au haut niveau de savoir où en est la commande actuelle
         */
        CommandManager::CommandStatus getCommandStatus();
        uint8_t getPendingCommandCount();

        inline void reset()
        {
            setEmergencyStop();
            resetEmergencyStop();
        }

    private:

        void switchToNextCommand();

        CommandList m_cmdList;
        Command *m_currentCmd;

        float m_straitLineArrivalWindows_mm;
        float m_turnArrivalWindows_rad;
        Goto::GotoConfiguration m_preciseGotoConfiguration;
        Goto::GotoConfiguration m_waypointGotoConfiguration;
        GotoNoStop::GotoNoStopConfiguration m_gotoNoStopConfiguration;
        AccelerationDecelerationLimiter *m_accelerationDecelerationLimiter;

        const Regulator &m_angle_regulator;
        const Regulator &m_distance_regulator;

        bool m_emergencyStop;

        BlockingDetector *m_blockingDetector;

        float m_angleRegulatorConsign;
        float m_distRegulatorConsign;
};

#endif
