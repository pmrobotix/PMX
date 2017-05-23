#ifndef ARM_SERVODRIVER_HPP_
#define ARM_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ServoMotorStd.hpp"


using namespace std;

class ServoDriver: public AServoDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoDriver(ARM).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriver.ARM");
		return instance;
	}

	int connected_;

	ServoMotorStd left_;
	ServoMotorStd right_;
	ServoMotorStd centre_;

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

	//percentage pour EV3 , 0->4096 pour APF
	void setPosition(int servo, int percent);

	void release(int servo);

	void setRate(int servo, int millisec);

	void turn(int servo, int speed);
};

#endif
