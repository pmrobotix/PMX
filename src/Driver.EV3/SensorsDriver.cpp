//drivers...EV3

#include "SensorsDriver.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create()
{

	return new SensorsDriver();
}

SensorsDriver::SensorsDriver()
{
	ir_ = infrared_sensor(INPUT_2);
	us_ = ultrasonic_sensor(INPUT_3);
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
	// as a percentage. 100% is approximately 70cm/27in.
	int percent = ir_.proximity();

	double distance_mm = percent * 7.0;

	if (distance_mm < 120)
		return 1;
	else
		return 0;
}

bool SensorsDriver::rear()
{
	int value = us_.other_sensor_present(); //TODO

	int mm = us_.distance_centimeters() * 10.0;

	if (mm < 120)
		return 1;
	else
		return 0;
}
