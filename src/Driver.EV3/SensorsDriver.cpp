//drivers...EV3

#include "SensorsDriver.hpp"

#include <cstdio>

#include "../Log/Logger.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create()
{
	return new SensorsDriver();
}

SensorsDriver::SensorsDriver()
{

	logger().debug() << "test" << logs::end;
	ir_ = infrared_sensor(INPUT_AUTO);
	if (ir_.connected())
	{
		logger().info() << ir_.type_name()
				<< " connected (device "
				<< ir_.driver_name()
				<< ", port "
				<< ir_.address()
				<< ", mode "
				<< ir_.mode()
				<< ")"
				<< logs::end;
		ir_.proximity();
	}
	else
	{
		logger().error() << "INPUT_2 (IR) not Connected !!" << logs::end;
	}

	us_ = ultrasonic_sensor();
	int value = 0;
	if (us_.connected())
	{
		logger().info() << us_.type_name()
				<< " connected (device "
				<< us_.driver_name()
				<< ", port "
				<< us_.address()
				<< ", mode "
				<< us_.mode()
				<< ")"
				<< logs::end;

		double mm = 0;
		value = us_.other_sensor_present(); //TODO

		logger().info() << "US other_sensor_present=" << value << logs::end;

		us_.distance_centimeters();

	}
	else
	{
		logger().error() << "INPUT_3 in3:i2c1 (US) not Connected !!" << logs::end;
	}
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
	// as a percentage. 100% is approximately 70cm/27in.
	int percent = ir_.value();

	double distance_mm = percent * 6.0;

	logger().info() << "front percent=" << percent << " mm=" << distance_mm << logs::end;
	if (distance_mm < 200)
		return 1;
	else
		return 0;
}

bool SensorsDriver::rear()
{
	double mm = 0;

	mm = us_.value() * 10.0;

	logger().info() << "rear mm=" << mm << logs::end;
	if (mm < 200)
		return 1;
	else
		return 0;
}