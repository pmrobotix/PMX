#ifndef EV3_ASSERVDRIVER_HPP_
#define EV3_ASSERVDRIVER_HPP_

#include <sys/types.h>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

#define MAXVALUE_speed_sp 	860 //power
#define MAXVALUE_duty_cycle_sp 		100 //percentage

using namespace std;
using namespace ev3dev;

class AsservDriver: public AAsservDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.EV3");
		return instance;
	}

	large_motor _motor_left;
	large_motor _motor_right;

protected:

public:
	/*!
	 * \brief Constructor.
	 */
	AsservDriver();

	/*!
	 * \brief Destructor.
	 */
	inline ~AsservDriver()
	{
	}

	void reset();
	int limit(int power, int max);

	//regulation enabled  => power in ticks per second -860 / +860
	//regulation disabled => power in percentage -100 / +100
	//ticks : relative position to go
	void setMotorLeftPosition(int power, long ticks);
	void setMotorRightPosition(int power, long ticks);

	//regulation enabled  => power in ticks per second -860 / +860
	//regulation disabled => power in percentage -100 / +100
	void setMotorLeftPower(int power, int time);
	void setMotorRightPower(int power, int time);

	long getLeftExternalEncoder();
	long getRightExternalEncoder();

	long getLeftInternalEncoder();
	long getRightInternalEncoder();

	void resetEncoders();
	void resetInternalEncoders();
	void resetExternalEncoders();

	void stopMotorLeft();
	void stopMotorRight();

	int getMotorLeftCurrent();
	int getMotorRightCurrent();

	/*
	 * speed_regulation
	 (read/write) Turns speed regulation on or off. Valid values are:
	 on: The motor controller will vary the power supplied to the motor to try to maintain the speed specified in speed_sp.
	 off: The controller will drive the motor using the duty cycle specified in duty_cycle_sp.
	 */
	void enableRightHardRegulation(bool enable);
	void enableLeftHardRegulation(bool enable);
	void enableHardRegulation(bool enable);

};

#endif
