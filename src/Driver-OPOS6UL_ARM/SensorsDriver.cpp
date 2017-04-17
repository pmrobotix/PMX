//drivers...OPO

#include "SensorsDriver.hpp"

#include <string>


using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string)
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

