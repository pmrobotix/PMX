//drivers...ARM

#include "SensorsDriver.hpp"

#include <unistd.h>
#include <iomanip>

#include "../Log/Logger.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create()
{
	return new SensorsDriver();
}

SensorsDriver::SensorsDriver()

{
	//std::cout << "SensorsDriver " << std::endl;
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{


}

bool SensorsDriver::rear()
{

}

