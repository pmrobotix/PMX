#ifndef OPOS6UL_ASSERVEXTENDED_HPP_
#define OPOS6UL_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Log/LoggerFactory.hpp"

class OPOS6UL_RobotExtended;

class OPOS6UL_AsservExtended: public Asserv
{
private:
    /*!
         * \brief Return \ref Logger linked to \ref OPOS6UL_AsservExtended.
         */
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

    bool filtre_IsInsideTableXY(int d_mm, int x_mm, int y_mm, float theta_deg, int *x_botpos,
            int *y_botpos);
    bool filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc = "");

//    void setLowSpeedForward(bool enable, int);
//    void setLowSpeedBackward(bool enable, int);

//TODO move to actions
    void update_adv();
};

#endif
