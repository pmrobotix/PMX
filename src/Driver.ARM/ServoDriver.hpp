#ifndef ARM_SERVODRIVER_HPP_
#define ARM_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"


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

	virtual void hold(int servoId);

	virtual void setPosition(int servoId, int pos);

	virtual void release(int servoId);

};

#endif
