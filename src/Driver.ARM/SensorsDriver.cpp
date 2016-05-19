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
		: frontIR_(5, 150), frontIRRight_(0, 30), frontIRLeft_(3, 30), rearIR_(1, 30)
{
	//std::cout << "SensorsDriver " << std::endl;
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
	bool detect = false;
	double centredistmm = frontIR_.getDistance();
	double centredistmm2 = frontIR_.getDistance();
	centredistmm = (centredistmm + centredistmm2) / 2.0;
	if (centredistmm < 220)
	{
		logger().info() << "!! detected FRONT centre=" << std::setw(5) << centredistmm << logs::end;
		detect = true;
	}

	double rightdistmm = frontIRRight_.getDistance();
	double rightdistmm2 = frontIRRight_.getDistance();
	rightdistmm = (rightdistmm + rightdistmm2) / 2.0;
	if (rightdistmm < 180 )
	{
		logger().info() << "!! detected FRONT  right=" << std::setw(5) << rightdistmm << logs::end;
		detect = true;
	}

	double leftdistmm = frontIRLeft_.getDistance();
	double leftdistmm2 = frontIRLeft_.getDistance();
	leftdistmm = (leftdistmm + leftdistmm2) / 2.0;
	if (leftdistmm < 180)
	{
		logger().info() << "!! detected FRONT   left=" << std::setw(5) << leftdistmm << logs::end;
		detect = true;
	}

	if (detect)
		return true;
	else
		return false;
}

bool SensorsDriver::rear()
{
	double distmm = rearIR_.getDistance();
	//usleep(1000);
	double distmm2 = rearIR_.getDistance();
	//usleep(1000);
	double distmm3 = rearIR_.getDistance();
	//usleep(1000);
	double distmm4 = rearIR_.getDistance();
	distmm = (distmm + distmm2 + distmm3 + distmm4) / 4.0;

	logger().debug() << "rear dist mm=" << distmm << logs::end;
	if (distmm < 180)
	{
		logger().info() << "!! detected REAR dist mm=" << distmm << logs::end;
		return true;
	}
	else
		return false;
}

