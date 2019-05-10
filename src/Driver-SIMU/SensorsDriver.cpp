//drivers...SIMU

#include "SensorsDriver.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string)
{

    return new SensorsDriver();
}

SensorsDriver::SensorsDriver()
{

}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
    return 0;
}

bool SensorsDriver::rear()
{
    return 0;
}

bool SensorsDriver::frontVeryClosed()
{
    return 0;
}
bool SensorsDriver::rearVeryClosed()
{
    return 0;
}
