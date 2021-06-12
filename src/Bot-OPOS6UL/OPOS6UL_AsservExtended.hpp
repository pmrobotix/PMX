#ifndef OPOS6UL_ASSERVEXTENDED_HPP_
#define OPOS6UL_ASSERVEXTENDED_HPP_

#include <bits/stringfwd.h>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"

class OPOS6UL_RobotExtended;

class OPOS6UL_AsservExtended: public Asserv
{
private:
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("OPOS6UL_AsservExtended");
        return instance;
    }
    OPOS6UL_RobotExtended * robot_extended_;
public:
    OPOS6UL_AsservExtended(std::string botId, OPOS6UL_RobotExtended * robot);

    ~OPOS6UL_AsservExtended()
    {
    }

    bool filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc = "");

    void setLowSpeedForward(bool enable, int);
    void setLowSpeedBackward(bool enable, int);

//TODO move to actions
    void update_adv();
};

#endif
