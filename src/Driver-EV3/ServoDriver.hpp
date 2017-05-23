#ifndef EV3_SERVODRIVER_HPP_
#define EV3_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

/* deja dans ev3dev.h
#ifndef FSTREAM_CACHE_SIZE
#define FSTREAM_CACHE_SIZE 16
#endif*/

class ServoDriver: public AServoDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriver.EV3");
		return instance;
	}

	int connected_;
	servo_motor sv1_;
	servo_motor sv2_;
	servo_motor sv3_;
	servo_motor sv4_;
	servo_motor sv5_;
	servo_motor sv6_;
	servo_motor sv7_;
	servo_motor sv8_;

protected:

public:
	/*!
	 * \brief Constructor.
	 */
	ServoDriver();

	/*!
	 * \brief Destructor.
	 */
	inline ~ServoDriver()
	{
	}

	void hold(int servo);

	//pos : (-100% to 100%)
	void setPosition(int servo, int pos);

	void release(int servo);

	void setRate(int servo, int millisec);

	void turn(int servo, int speed);
};

#endif
