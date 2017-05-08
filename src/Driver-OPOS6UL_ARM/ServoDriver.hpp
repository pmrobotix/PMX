#ifndef OPOS6UL_SERVODRIVER_HPP_
#define OPOS6UL_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "CCAx12Adc.hpp"



using namespace std;

class ServoDriver: public AServoDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriver.OPO");
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

	void hold(ServoLabel servo);

	//percentage pour EV3 , 0->4096 pour APF
	void setPosition(ServoLabel servo, double percent);

	void release(ServoLabel servo);

	void setRate(ServoLabel servo, int millisec);

};

#endif
