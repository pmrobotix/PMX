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
	virtual inline ~ServoDriver()
	{
	}

	virtual void hold(ServoLabel servo);

	//percentage pour EV3 , 0->4096 pour APF
	virtual void setPosition(ServoLabel servo, double percent);

	virtual void release(ServoLabel servo);

	virtual void setRate(ServoLabel servo, int millisec);

};

#endif
