#ifndef SIMUCOCOS_SERVODRIVER_HPP_
#define SIMUCOCOS_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"


using namespace std;

class ServoDriver: public AServoDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriver.SIMUCOCOS");
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
	inline ~ServoDriver()
	{
	}

	void hold(int servo);

	void setPosition(int servo, int pos);

	void release(int servo);

	void setRate(int servo, int speed);

	void turn(int servo, int speed);

	int getMoving(int servo);

	int getPos(int servo);

};

#endif
