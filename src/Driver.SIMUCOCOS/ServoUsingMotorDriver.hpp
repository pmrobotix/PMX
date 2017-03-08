#ifndef SIMUCOCOS_SERVOUSINGMOTORDRIVER_HPP_
#define SIMUCOCOS_SERVOUSINGMOTORDRIVER_HPP_

#include "../Common/Action.Driver/AServoUsingMotorDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class ServoUsingMotorDriver: public AServoUsingMotorDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoUsingMotorDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"ServoUsingMotorDriver.SIMU");
		return instance;
	}

	int connected_;

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

	virtual void setMotorPosition(int pos, int timems, int power);

	virtual long getInternalEncoder();

	virtual void resetEncoder(int pos);

	virtual void stopMotor();

	virtual int getMotorCurrent();

	virtual void enableHardRegulation(bool enable);

};

#endif
