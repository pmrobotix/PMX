//drivers...OPOS

#include "ServoDriver.hpp"

#include <unistd.h>

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

	connected_ = CCAx12Adc::instance().begin();

	logger().error() << "connected=" << connected_ << logs::end;
	if (connected_)
	{
		CCAx12Adc::instance().setLedOn(3);

		int ping5 = CCAx12Adc::instance().pingAX(12);
		logger().error() << "ping=" << ping5 << logs::end;
		hold(8);
		setPosition(8, 500);
		sleep(2);
		release(8);
		release(5);
	}

}

void ServoDriver::hold(int servo)
{

	int err = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_ENABLE, 1);
	logger().info() << "hold enableax=" << err << logs::end;
	/*
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
	 */
	switch (servo)
	{

	default:
		break;
	}
}

void ServoDriver::setPosition(int servo, int pos)
{
	if (pos >= 4096) pos = 4096;
	if (pos <= -4096) pos = -4096;

	int writeposax = CCAx12Adc::instance().writeAXData(servo, P_GOAL_POSITION, pos);
	logger().info() << "setPosition P_GOAL_POSITION =" << writeposax << logs::end;

	logger().error() << "servo" << servo << " pos=" << pos << logs::end;

}

void ServoDriver::release(int servo)
{
	int torqueax4 = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_ENABLE, 0);
	logger().info() << "release disableax=" << torqueax4 << logs::end;
}

void ServoDriver::setRate(int servo, int millisec)
{
	switch (servo)
	{

	default:
		break;
	}
}
