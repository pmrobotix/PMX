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
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("OPOS6UL_AsservExtended");
        return instance;
    }

    OPOS6UL_RobotExtended *robot_extended_;

public:
    OPOS6UL_AsservExtended(std::string botId, OPOS6UL_RobotExtended *robot);

    ~OPOS6UL_AsservExtended()
    {
    }

    void startMotionTimerAndOdo(bool assistedHandlingEnabled);

    //verification de coordonnées table si dans le terrain (pour prise en compte) ou non
    bool filtre_IsInsideTableXY(int x_botpos_repereTable, int y_botpos_repereTable);

    //TODO deprecated
    bool filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc = "");


//TODO move to actions
    void update_adv();
};

#endif
