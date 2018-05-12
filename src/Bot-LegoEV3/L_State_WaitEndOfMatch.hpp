#ifndef L_STATE_WAITENDOFMATCH_HPP
#define	L_STATE_WAITENDOFMATCH_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"

class Robot;

/*!
 * \brief Cette classe est la tache principale du robot.
 */
class L_State_WaitEndOfMatch: public AAutomateState
{
private:

    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_State_WaitEndOfMatch");
        return instance;
    }

public:

    /*!
     * \brief Constructeur de la classe.
     */
    L_State_WaitEndOfMatch()
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    ~L_State_WaitEndOfMatch()
    {
    }

    IAutomateState* execute(Robot& robot);

    std::string name()
    {
        return "L_State_WaitEndOfMatch";
    }
};

#endif
