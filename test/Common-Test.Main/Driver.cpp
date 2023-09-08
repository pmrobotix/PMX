#include <string>

#include "../../src/Common/Interface.Driver/AAsservDriver.hpp"
#include "../../src/Common/Interface.Driver/AButtonDriver.hpp"
#include "../../src/Common/Interface.Driver/AColorDriver.hpp"
#include "../../src/Common/Interface.Driver/ALcdShieldDriver.hpp"
#include "../../src/Common/Interface.Driver/ALedDriver.hpp"
#include "../../src/Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../../src/Common/Interface.Driver/ASensorsDriver.hpp"
#include "../../src/Common/Interface.Driver/AServoDriver.hpp"
#include "../../src/Common/Interface.Driver/AServoUsingMotorDriver.hpp"
#include "../../src/Common/Interface.Driver/ASoundDriver.hpp"
#include "../../src/Common/Interface.Driver/ASwitchDriver.hpp"

ARobotPositionShared* ARobotPositionShared::create()
{
    return 0;
}

AColorDriver* AColorDriver::create(std::string botid)
{
    return 0;
}

AAsservDriver* AAsservDriver::create(std::string botid, ARobotPositionShared *aRobotPositionShared)
{
    return 0;
}

ALedDriver* ALedDriver::create(std::string, int nb)
{
    return 0;
}

AButtonDriver* AButtonDriver::create()
{
    return 0;
}

ALcdShieldDriver* ALcdShieldDriver::create(std::string botId)
{
    return 0;
}

ASensorsDriver* ASensorsDriver::create(std::string, ARobotPositionShared*)
{
    return 0;
}

AServoDriver* AServoDriver::create()
{
    return 0;
}

AServoUsingMotorDriver* AServoUsingMotorDriver::create()
{
    return 0;
}

ASoundDriver* ASoundDriver::create()
{
    return 0;
}

ASwitchDriver* ASwitchDriver::create(std::string)
{
    return 0;
}
