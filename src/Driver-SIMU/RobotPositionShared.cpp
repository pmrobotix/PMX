//drivers...SIMU

#include "RobotPositionShared.hpp"



using namespace std;

ARobotPositionShared* ARobotPositionShared::create()
{
    //return new RobotPositionShared();

    static RobotPositionShared *instance = new RobotPositionShared();
    return instance;
}

RobotPositionShared::RobotPositionShared()
{

}

RobotPositionShared::~RobotPositionShared()
{
}

void RobotPositionShared::setRobotPosition(ROBOTPOSITION p)
{
    this->lock();
    p_ = p;
    this->unlock();
}

ROBOTPOSITION RobotPositionShared::getRobotPosition()
{
    ROBOTPOSITION p;
    this->lock();
    p = p_;
    this->unlock();
    return p;
}



