#ifndef COMMON_ASSERVDRIVER_AASSERVDRIVER_HPP_
#define COMMON_ASSERVDRIVER_AASSERVDRIVER_HPP_

#include <string>

class AAsservDriver
{

public:

	/*!
	 * \brief single instance creation.
	 */
	static AAsservDriver * create(std::string botid);

	/*!
	 * \brief Destructor.
	 */
	virtual ~AAsservDriver()
	{
	}

	/*!
	 * \brief Constructor.
	 */
	AAsservDriver()
	{
	}

	virtual void setMotorLeftPosition(int power, long ticks) = 0;
	virtual void setMotorRightPosition(int power, long ticks) = 0;

	virtual void setMotorLeftPower(int power, int time_ms) = 0;
	virtual void setMotorRightPower(int power, int time_ms) = 0;

	virtual long getLeftExternalEncoder() = 0;
	virtual long getRightExternalEncoder() = 0;

	virtual long getLeftInternalEncoder() = 0;
	virtual long getRightInternalEncoder() = 0;

	virtual void resetEncoders() = 0;
	virtual void resetInternalEncoders() = 0;
	virtual void resetExternalEncoders() = 0;

	virtual void stopMotorLeft() = 0;
	virtual void stopMotorRight() = 0;

	virtual int getMotorLeftCurrent() = 0;
	virtual int getMotorRightCurrent() = 0;

	virtual void enableHardRegulation(bool enable) = 0;

};

#endif

