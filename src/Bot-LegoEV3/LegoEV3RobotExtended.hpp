#ifndef LegoEV3_ROBOTEXTENDED_HPP_
#define LegoEV3_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"

class L_State_DecisionMakerIA;
class LegoEV3ActionsExtended;
class LegoEV3AsservExtended;
class LegoEV3IAExtended;
class LegoEV3SvgWriterExtended;

class LegoEV3RobotExtended: public Robot
{
public:

    static LegoEV3RobotExtended& instance()
    {
        static LegoEV3RobotExtended instance;
        return instance;
    }

    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("LegoEV3RobotExtended");
        return instance;
    }

    ~LegoEV3RobotExtended();

//    inline void setActions(LegoEV3ActionsExtended *action)
//    {
//        actions_default_ = action;
//    }
    inline LegoEV3ActionsExtended& actions()
    {
        LegoEV3ActionsExtended &r_actions = *p_actions_;
        return r_actions;
    }

//    inline LegoEV3ActionsExtended& actions()
//    {
//        LegoEV3ActionsExtended& r_actions = *p_actions_;
//        return r_actions;
//    }

//    inline LegoEV3AsservExtended& asserv()
//    {
//        LegoEV3AsservExtended& r_asserv = *p_asserv_;
//        return r_asserv;
//
////        LegoEV3AsservExtended& r_asserv =  Robot::asserv();
////        return r_asserv;
//
//    }

    inline LegoEV3IAExtended& ia()
    {
        LegoEV3IAExtended &r_ia = *p_ia_;
        return r_ia;
    }

    //DATA custom
    //bool trophy_taken;
    //bool alea_done;
    //int square_pattern;

    bool force_end_of_match;

    void begin(int argc, char **argv);

    void stopExtraActions();

    void displayPoints();

    void resetDisplayTS();
    void displayTS(TRAJ_STATE ts);
    void resetDisplayObstacle();
    void displayObstacle(int level);

    L_State_DecisionMakerIA *decisionMaker_; //TODO mettre un accesseur

private:

    LegoEV3ActionsExtended * p_actions_;

    //LegoEV3AsservExtended * p_asserv_;

    LegoEV3IAExtended *p_ia_;

    //LegoEV3SvgWriterExtended * p_svg_;

    LegoEV3RobotExtended();

};

#endif

