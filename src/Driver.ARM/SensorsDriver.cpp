//drivers...ARM

#include "SensorsDriver.hpp"

#include <iostream>

#include "../Log/Logger.hpp"

using namespace std;


ASensorsDriver * ASensorsDriver::create()
{
	return new SensorsDriver();
}

SensorsDriver::SensorsDriver()
		: frontIR_(4, 30), rearIR_(5, 30)
{
	//std::cout << "SensorsDriver " << std::endl;
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
	int distmm = frontIR_.getDistance();

	logger().info() << "front dist mm=" << distmm << logs::end;
	if (distmm < 80)
		return true;
	else
		return false;
}

bool SensorsDriver::rear()
{
	int distmm = rearIR_.getDistance();

	logger().info() << "rear dist mm=" << distmm << logs::end;
	if (distmm < 80)
		return true;
	else
		return false;
}

