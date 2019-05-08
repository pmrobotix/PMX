#ifndef OPOS6UL_ASSERVEXTENDED_HPP_
#define OPOS6UL_ASSERVEXTENDED_HPP_

#include <bits/stringfwd.h>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"

class OPOS6UL_RobotExtended;

class OPOS6UL_AsservExtended: public Asserv
{
private:
    OPOS6UL_RobotExtended * robot_extended_;
public:
    OPOS6UL_AsservExtended(std::string botId, OPOS6UL_RobotExtended * robot);

    ~OPOS6UL_AsservExtended()
    {
    }

    void resetDisplayTS();
    void displayTS(TRAJ_STATE ts);

};

#endif
