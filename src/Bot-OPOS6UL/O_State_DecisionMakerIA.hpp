#ifndef O_STATE_DECISIONMAKERIA_HPP
#define	O_STATE_DECISIONMAKERIA_HPP

#include <src/pmr_playground.h>
#include <string>

#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"

class Robot;

class O_State_DecisionMakerIA: public utils::Thread
{
private:

    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_State_DecisionMakerIA");
        return instance;
    }

    /*!
     * \brief Référence vers le robot.
     */
    Robot & robot_;


public:

    O_State_DecisionMakerIA(Robot& robot);

    ~O_State_DecisionMakerIA()
    {
    }

    /*!
     * \brief Execute l'action.
     */
    void execute();

    std::string name()
    {
        return "O_State_DecisionMakerIA";
    }

    void IASetupActivitiesZone();
    void IASetupActivitiesZoneTableTest();

};

#endif
