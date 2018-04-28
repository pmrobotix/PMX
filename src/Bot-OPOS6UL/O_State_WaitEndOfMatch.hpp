#ifndef O_STATE_WAITENDOFMATCH_HPP
#define	O_STATE_WAITENDOFMATCH_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"

class Robot;

/*!
 * \brief Cette classe est la tache principale du robot.
 */
class O_State_WaitEndOfMatch: public AAutomateState
{
private:

    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_State_WaitEndOfMatch");
        return instance;
    }

public:

    /*!
     * \brief Constructeur de la classe.
     */
    O_State_WaitEndOfMatch()
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    ~ O_State_WaitEndOfMatch()
    {
    }

    IAutomateState* execute(Robot& robot);

    std::string name()
    {
        return "O_State_WaitEndOfMatch";
    }
};

#endif
