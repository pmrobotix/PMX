#ifndef EV3_SERVOUSINGMOTORDRIVER_HPP_
#define EV3_SERVOUSINGMOTORDRIVER_HPP_

#include "../Common/Action.Driver/AServoUsingMotorDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

#define MAXVALUE_speed_sp 	860 //power
#define MAXVALUE_duty_cycle_sp 		100 //percentage

using namespace std;
using namespace ev3dev;

class ServoUsingMotorDriver: public AServoUsingMotorDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoUsingMotorDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoUsingMotorDriver.EV3");
		return instance;
	}

	int connected_;
	large_motor _servo_device;

protected:

public:
	/*!
	 * \brief Constructor.
	 */
	ServoUsingMotorDriver();

	/*!
	 * \brief Destructor.
	 */
	virtual inline ~ServoUsingMotorDriver()
	{
	}

	virtual void setMotorPosition(int power, int pos, int ramptimems);

	virtual long getInternalEncoder();

	virtual void resetEncoder(int pos);

	virtual void stopMotor();

	virtual int getMotorCurrent();

	virtual void enableHardRegulation(bool enable);

	int limit(int power, int max);

};

#endif
