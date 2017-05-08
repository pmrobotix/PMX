//drivers...OPOS

#include "ServoDriver.hpp"

#include <unistd.h>
#include <cmath>

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create()
{
	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver() :
		connected_(0)
{
	logger().error() << "ServoDriver::ServoDriver() CCAx12Adc::instance().begin();" << logs::end;

	CCAx12Adc::instance().begin();

}

void ServoDriver::hold(ServoLabel servo)
{
	int swap = 0;
	for (int n = 0; n < 5; n++)
	{

		if (swap)
		{
			//usleep(100000);
			CCAx12Adc::instance().setLedOn(2);
		}
		else
			CCAx12Adc::instance().setLedOff(2);

		swap = !swap;

		int adc1 = CCAx12Adc::instance().getADC(1);
		int adc0 = CCAx12Adc::instance().getADC(0);
		int adc7 = CCAx12Adc::instance().getADC(7);
		logger().info() << "adc0=" << adc0 << " adc1=" << adc1 << " adc7=" << adc7 << logs::end;

		int ping4 = CCAx12Adc::instance().pingAX(4);
		logger().info() << "ping4=" << ping4 << logs::end;
		int ping6 = CCAx12Adc::instance().pingAX(6);
		logger().info() << "ping6=" << ping6 << logs::end;

		int readax4 = CCAx12Adc::instance().readAXData(4, P_ID);
		logger().info() << "readax4 P_ID =" << readax4 << logs::end;
		int readax6 = CCAx12Adc::instance().readAXData(6, P_ID);
		logger().info() << "readax6 P_ID =" << readax6 << logs::end;

		int posax4 = CCAx12Adc::instance().readAXData(4, P_PRESENT_POSITION);
		logger().info() << "posax4 P_PRESENT_POSITION=" << posax4 << logs::end;
		int posax6 = CCAx12Adc::instance().readAXData(6, P_PRESENT_POSITION);
		logger().info() << "posax6 P_PRESENT_POSITION=" << posax6 << logs::end;

		int writeposax = CCAx12Adc::instance().writeAXData(4, P_GOAL_POSITION, 700);
		logger().info() << "writeposax4 P_GOAL_POSITION =" << writeposax << logs::end;

		int err = CCAx12Adc::instance().writeAXData(6, P_TORQUE_ENABLE, 1);
		logger().info() << "torque6 enableax=" << err << logs::end;

		int writeposax6 = CCAx12Adc::instance().writeAXData(6, P_GOAL_POSITION, 700);
		logger().info() << "writeposax6 P_GOAL_POSITION 700 =" << writeposax6 << logs::end;

		int torqueax4 = CCAx12Adc::instance().writeAXData(4, P_TORQUE_ENABLE, 0);
		logger().info() << "torque4 disableax=" << torqueax4 << logs::end;
		int torqueax6 = CCAx12Adc::instance().writeAXData(6, P_TORQUE_ENABLE, 0);
		logger().info() << "torque6 disableax=" << torqueax6 << logs::end;

		logger().info() << "" << logs::end;

	}

	switch (servo)
	{
	/*
	 case SERVO_LEFT:

	 break;

	 case SERVO_RIGHT:

	 break;

	 case SERVO_CENTRE:

	 break;
	 */
	default:
		break;
	}
}

void ServoDriver::setPosition(ServoLabel servo, double percent)
{
	if (percent >= 100) percent = 100;
	if (percent <= -100) percent = -100;
	//   0   -> 4096
	//-100.0%-> 100.0% percent

	double pos = round((percent) * 20.48) + 2048;
	//double pos = percent;

//

	logger().error() << "servo" << servo << " pos=" << (int) pos << logs::end;
	switch (servo)
	{
	/*
	 case SERVO_LEFT:

	 break;

	 case SERVO_RIGHT:

	 break;

	 case SERVO_CENTRE:

	 break;
	 */
	default:
		break;
	}
}

void ServoDriver::release(ServoLabel servo)
{
	switch (servo)
	{
	/*
	 case SERVO_LEFT:

	 break;

	 case SERVO_RIGHT:

	 break;

	 case SERVO_CENTRE:

	 break;
	 */
	default:
		break;
	}
}

void ServoDriver::setRate(ServoLabel servo, int millisec)
{
	switch (servo)
	{
	/*
	 case SERVO_LEFT:
	 break;

	 case SERVO_RIGHT:
	 break;

	 case SERVO_CENTRE:
	 break;
	 */
	default:
		break;
	}
}
