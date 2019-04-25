#ifndef L_STATE_DECISIONMAKERIA_HPP
#define	L_STATE_DECISIONMAKERIA_HPP

#include <string>

#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"

class Playground;

class Robot;

class L_State_DecisionMakerIA: public utils::Thread //todo A FUSIONNER AVEC IAEXTENDED ???
{
private:

    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_State_DecisionMakerIA");
        return instance;
    }

    /*!
     * \brief Référence vers le robot.
     */
    Robot & robot_;

    Playground *p_; //for new ia

public:

    L_State_DecisionMakerIA(Robot& robot);

    ~L_State_DecisionMakerIA()
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
    void initPlayground();

};

#endif
