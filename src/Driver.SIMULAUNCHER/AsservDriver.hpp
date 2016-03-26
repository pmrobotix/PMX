#ifndef SIMULAUNCHER_ASSERVDRIVER_HPP_
#define SIMULAUNCHER_ASSERVDRIVER_HPP_

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class AsservDriver: public AAsservDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(SIMULAUNCHER).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.SIMULAUNCHER");
		return instance;
	}

	utils::Chronometer chrono_;
	int leftPower_;
	int rightPower_;

	long tLeft_ms_;

	long tRight_ms_;

	long rightCounter_;
	long leftCounter_;

protected:

public:

	void computeCounterL();
	void computeCounterR();

	virtual void setMotorLeftPosition(int power, long ticks);
	virtual void setMotorRightPosition(int power, long ticks);

	virtual void setMotorLeftPower(int power, int time);
	virtual void setMotorRightPower(int power, int time);

	virtual long getLeftExternalEncoder();
	virtual long getRightExternalEncoder();

	virtual long getLeftInternalEncoder();
	virtual long getRightInternalEncoder();

	virtual void resetEncoder();

	virtual void stopMotorLeft();
	virtual void stopMotorRight();

	virtual int getMotorLeftCurrent();
	virtual int getMotorRightCurrent();

	virtual void enableHardRegulation(bool enable);

	/*!
	 * \brief Constructor.
	 */
	AsservDriver();

	/*!
	 * \brief Destructor.
	 */
	virtual ~AsservDriver();

};

#endif