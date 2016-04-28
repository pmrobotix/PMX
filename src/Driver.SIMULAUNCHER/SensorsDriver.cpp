//drivers...SIMULAUNCHER

#include "SensorsDriver.hpp"



using namespace std;

ASensorsDriver * ASensorsDriver::create()
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
