//drivers...EV3

#include "SensorsDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string botName)
{
	return new SensorsDriver();
}

SensorsDriver::SensorsDriver()
{

	ir_ = infrared_sensor(INPUT_2);
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

	irrear_ = infrared_sensor(INPUT_3);
		if (irrear_.connected())
		{
			logger().info() << irrear_.type_name()
					<< " connected (device "
					<< irrear_.driver_name()
					<< ", port "
					<< irrear_.address()
					<< ", mode "
					<< irrear_.mode()
					<< ")"
					<< logs::end;
			irrear_.proximity();
		}
		else
		{
			logger().error() << "INPUT_3 (IR REAR) not Connected !!" << logs::end;
		}
/*
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
	}*/
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
	// as a percentage. 100% is approximately 70cm/27in.
	double percent = ir_.value();
	double percent2 = ir_.value();
	percent = (percent + percent2) / 2.0;

	double distance_mm = percent * 6.0;

	logger().debug() << "front percent=" << percent << " mm=" << distance_mm << logs::end;
	if (distance_mm < 220)
	{
		logger().info() << "!! detected FRONT percent="
				<< percent
				<< " mm="
				<< distance_mm
				<< logs::end;
		return 1;
	}
	else
		return 0;
}

bool SensorsDriver::rear()
{
	// as a percentage. 100% is approximately 70cm/27in.
		double percent = irrear_.value();
		double percent2 = irrear_.value();
		percent = (percent + percent2) / 2.0;

		double distance_mm = percent * 6.0;

		logger().debug() << "rear percent=" << percent << " mm=" << distance_mm << logs::end;
		if (distance_mm < 170)
		{
			logger().info() << "!! detected REAR percent="
					<< percent
					<< " mm="
					<< distance_mm
					<< logs::end;
			return 1;
		}
		else
			return 0;
	/*
	double mm = us_.value() * 10.0;
	double mm2 = us_.value() * 10.0;
	mm = (mm + mm2) / 2.0;


	logger().info() << "rear mm=" << mm << logs::end;
	if (mm < 170)
	{
		logger().info() << "!! detected REAR mm=" << mm << logs::end;
		return 1;
	}
	else
		return 0;*/
}
