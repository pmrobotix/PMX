#ifndef LegoEV3_ASSERVEXTENDED_HPP_
#define LegoEV3_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"

class LegoEV3RobotExtended;

class LegoEV3AsservExtended: public Asserv
{
private:
    /*!
     * \brief Return \ref Logger linked to \ref LegoEV3AsservExtended.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3AsservExtended");
        return instance;
    }
    LegoEV3RobotExtended * robot_extended_;

public:
    LegoEV3AsservExtended(std::string botId, LegoEV3RobotExtended * robot);

    ~LegoEV3AsservExtended()
    {
    }

    void startMotionTimerAndOdo(bool assistedHandlingEnabled);

    void resetDisplayTS();
    void displayTS(TRAJ_STATE ts);

    bool filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm);
};

#endif

