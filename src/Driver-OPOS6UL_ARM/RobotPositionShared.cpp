//drivers...ARM

#include "RobotPositionShared.hpp"
#include "../Common/Interface.Driver/ARobotPositionShared.hpp"

using namespace std;

ARobotPositionShared* ARobotPositionShared::create()
{
    static RobotPositionShared *instance = new RobotPositionShared();
    return instance;
}

RobotPositionShared::RobotPositionShared()
{

}

//RobotPositionShared::~RobotPositionShared()
//{
//}

void RobotPositionShared::setRobotPosition(ROBOTPOSITION p)
{

}

ROBOTPOSITION RobotPositionShared::getRobotPosition()
{

}
