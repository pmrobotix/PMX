//drivers...OPOS

#include <unistd.h>
#include <iostream>
#include <string>
#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"
#include "CCAx12Teensy.hpp"

using namespace std;

AServoDriver* AServoDriver::create()
{

	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver()

{
	logger().debug() << "ServoDriver::ServoDriver()" << logs::end;

	connected_ = CCAx12Teensy::instance().connect(AX12TEENSY_ADDR);

	/*
	 setRate(1007, 1023);
	 //loop
	 while (1) {
	 CCAx12Teensy::instance().setLedOn(4);
	 setPulsePos(1007, 312, 100);
	 usleep(3000000);
	 CCAx12Teensy::instance().setLedOff(4);
	 setPulsePos(1007, 512, 100);
	 usleep(3000000);
	 //int p = CCAx12Teensy::instance().pingAX(1, 7);
	 int p = ping(1007);
	 logger().error() << "CCAx12Teensy...conn=" << conn << " p=" << p << logs::end;
	 }*/
}

bool ServoDriver::is_connected()
{
	return connected_;

}

//DEPRECATED pour OPOS6UL? uniquement pour servo STD ?
void ServoDriver::setType(int servo, ServoType type)
{
//    if (!testIf(servo, 0, MAXPORTNUM)) return;
//    servo_type_[constrain(servo, 0, NB_SERVO_DYN - 1)] = type;
}

void ServoDriver::hold(int servo)
{
	if (!connected_) return;
	if (!testIf(servo, 0, MAXPORTNUM)) return;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "hold() NOT IMPLEMENTED !" << logs::end;
	} else if (port < 5)
	{
		servo -= (1000 * port);
		int r = CCAx12Teensy::instance().writeAXData(port, servo, P_TORQUE_ENABLE, 1);
		if (r < 0) logger().error() << "hold() ERROR! port=" << port << " servo=" << servo << logs::end;
	}
}

//actuellement c'est la velocity de 0 à 1023
//AX12 :
//0 ~ 1,023(0x3FF) can be used, and the unit is about 0.111rpm.
//If it is set to 0, it means the maximum rpm of the motor is used without controlling the speed.
//If it is 1023, it is about 114rpm.
//For example, if it is set to 300, it is about 33.3 rpm.

void ServoDriver::setPulsePos(int servo, int pulsewidth, int velocity)
{
	if (!connected_) return;
	if (!testIf(servo, 0, MAXPORTNUM)) return;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "setPulsePos() NOT IMPLEMENTED FOR STD!" << logs::end;
	} else if (port < 5)
	{
		servo -= (1000 * port);
		pulsewidth = constrain(pulsewidth, 0, MAX_POS);

		if (velocity > 0)
		{
			int r = CCAx12Teensy::instance().writeAXData(port, servo, P_GOAL_SPEED, velocity);
			if (r < 0)
				logger().error() << "setPulsePos() writeAXData P_GOAL_SPEED ERROR port=" << port << " servo=" << servo
						<< " velocity=" << velocity << logs::end;

		}
		int r = CCAx12Teensy::instance().writeAXData(port, servo, P_GOAL_POSITION, pulsewidth);
		if (r < 0)
			logger().error() << "setPulsePos() writeAXDataP_GOAL_POSITION ERROR port=" << port << " servo=" << servo
					<< " pulsewidth=" << pulsewidth << logs::end;

	}
}

void ServoDriver::turn(int servo, int speed)
{
	if (!connected_) return;
	if (!testIf(servo, 0, MAXPORTNUM)) return;
	logger().error() << "ServoDriver::turn() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::release(int servo)
{
	if (!connected_) return;
	if (!testIf(servo, 0, MAXPORTNUM)) return;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "release() NOT IMPLEMENTED !" << logs::end;
	} else if (port < 5)
	{
		servo -= (1000 * port);
		int r = CCAx12Teensy::instance().writeAXData(port, servo, P_TORQUE_ENABLE, 0);
		if (r < 0) logger().debug() << "release() ERROR! port=" << port << " servo=" << servo << logs::end;
	}
}

void ServoDriver::setTorque(int servo, int torque)
{
	if (!connected_) return;
	if (!testIf(servo, 0, MAXPORTNUM)) return;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "setRate() NOT IMPLEMENTED !" << logs::end;
	} else if (port < 5)
	{
		servo -= (1000 * port);
		int r = CCAx12Teensy::instance().writeAXData(port, servo, P_TORQUE_LIMIT, constrain(torque, 0, 1023));
		if (r < 0)
			logger().error() << "setRate() ERROR! port=" << port << " servo=" << servo << " torque=" << torque
					<< logs::end;
	}
}

//return 1 when in progress, 0 when idle, -1 on error
int ServoDriver::getMoving(int servo)
{
	if (!connected_) return -100;
	if (!testIf(servo, 0, MAXPORTNUM)) return -10;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "getMoving() NOT IMPLEMENTED !" << logs::end;
		return -1;
	} else if (port < 5)
	{
		servo -= (1000 * port);
		int r = CCAx12Teensy::instance().readAXData(port, servo, P_MOVING);
		if (r < 0)
		{
			logger().error() << "getMoving() ERROR! port=" << port << " servo=" << servo << logs::end;
		}
		return r;
	}
}

int ServoDriver::getPulsePos(int servo)
{
	if (!connected_) return -100;
	if (!testIf(servo, 0, MAXPORTNUM)) return -10;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "getPulsePos() NOT IMPLEMENTED !" << logs::end;
		return -1;
	} else if (port < 5)
	{
		servo -= (1000 * port);

		int r = CCAx12Teensy::instance().readAXData(port, servo, P_PRESENT_POSITION);
		if (r < 0) logger().error() << "getPulsePos() ERROR! port=" << port << " servo=" << servo << logs::end;
		return r;
	}
	return -2;
}

//return true if available
int ServoDriver::ping(int servo)
{
	if (!connected_) return -100;
	if (!testIf(servo, 0, MAXPORTNUM)) return -10;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "ping() NOT IMPLEMENTED !" << logs::end;
		return -1;
	} else if (port < 5)
	{
		servo -= (1000 * port);

		int r = CCAx12Teensy::instance().pingAX(port, servo);
		if (r < 0)
		{
			logger().error() << "ping() ERROR! port=" << port << " servo=" << servo << logs::end;
			return r;
		}
		if (r == AXT_AX_MISSING) return 0;
		if (r == AXT_OK) return 1;
	} else
	{
		logger().error() << "ping() port ERROR! port=" << port << " servo=" << servo << logs::end;
		return -10;
	}
}

void ServoDriver::setMinPulse(int servo, int pulse)
{
	logger().debug() << "ServoDriver::setMinPulse() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::setMidPulse(int servo, int pulse)
{
	logger().debug() << "ServoDriver::setMidPulse() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::setMaxPulse(int servo, int pulse)
{
	logger().debug() << "ServoDriver::setMaxPulse() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::setPolarity(int servo, bool inversed)
{
	logger().debug() << "ServoDriver::setPolarity() NOT IMPLEMENTED !" << logs::end;
}

int ServoDriver::getTorque(int servo)
{
	if (!connected_) return -100;
	if (!testIf(servo, 0, MAXPORTNUM)) return -10;
	int port = servo / 1000;
	if (port == 10)
	{
		logger().error() << "getTorque() NOT IMPLEMENTED !" << logs::end;
	} else if (port < 5)
	{
		servo -= (1000 * port);
		int r = CCAx12Teensy::instance().readAXData(port, servo, P_PRESENT_LOAD);
		if (r < 0)
			logger().error() << "getTorque() ERROR! port=" << port << " servo=" << servo << " r=" << r << logs::end;

		//removing CW and CCW so the result will be from 0 to 1023

//        It means currently applied load. The range of the value is 0~2047, and the unit is about 0.1%. If the value is 0~1,023,
//		it means the load works to the CCW direction. If the value is 1,024~2,047,
//		it means the load works to the CW direction. That is, the 10th bit becomes the direction bit to control the direction,
//		and 1,024 is equal to 0. For example, the value is 512, it means the load is detected in the
//		direction of CCW about 50% of the maximum torque.
//        Bit	15 ~ 11	      10	            9 ~ 0
//        Value	   0	      Load Direction	Data (Load Ratio)
//        NOTE : CCW Load : Load Direction = 0, CW Load : Load Direction = 1


//		int r2 = r;
//		if(r>1023)
//		{
//			//bool direction = true;
//			r2-=1023;
//			r2=-r2;
//		}

		bool direction = (r & 0x0400) >> 10;
		int r2 = (r & 0x03FF);
		//logger().error() << " dir=" << direction << " Torque=" << r << " r2=" << r2 << logs::end;
		if (direction) r2 = -r2;
		return r2;

	}
}

bool ServoDriver::testIf(long value, long valeurMin, long valeurMax)
{
	if ((value >= valeurMin) && (value <= valeurMax))
		return true;
	else
	{
		logger().error() << "SERVO ID NOT EXISTS!! testIf()" << logs::end;
		return false;
	}
}

