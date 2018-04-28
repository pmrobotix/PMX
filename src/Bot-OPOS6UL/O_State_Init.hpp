#ifndef O_STATE_INIT_HPP
#define	O_STATE_INIT_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"

class O_State_Init: public AAutomateState
{
private:

    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("O_State_Init");
        return instance;
    }

    void setPos();

public:

    O_State_Init()
    {
    }

    ~O_State_Init()
    {
    }

    IAutomateState* execute(Robot& robot);

    std::string name()
    {
        return "O_State_Init";
    }
};

#endif
