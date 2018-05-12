#ifndef L_STATE_INIT_HPP
#define	L_STATE_INIT_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"

class L_State_Init: public AAutomateState
{
private:

    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("L_State_Init");
        return instance;
    }

    void setPos();

public:

    L_State_Init()
    {
    }

    ~L_State_Init()
    {
    }

    IAutomateState* execute(Robot& robot);

    std::string name()
    {
        return "L_State_Init";
    }
};

#endif
