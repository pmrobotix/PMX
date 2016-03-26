/* pour les moteurs utilisés en mode servomotor.
 *
 */
#ifndef ASERVOWITHMOTORDRIVER_HPP_
#define ASERVOWITHMOTORDRIVER_HPP_

class AServoWithMotorDriver
{

public:

	/*!
	 * \brief single instance creation.
	 */
	static AServoWithMotorDriver * create();

	/*!
	 * \brief Destructor.
	 */
	virtual ~AServoWithMotorDriver()
	{
	}

	/*!
	 * \brief Constructor.
	 */
	AServoWithMotorDriver()
	{
	}

	virtual void setMotorPosition(int power, int pos, int rampTime_ms) = 0;

	virtual void setMotorLeftPower(int power, int time_ms) = 0;

	virtual long getInternalEncoder()= 0;

	virtual void resetEncoder(int pos)= 0;

	virtual void stopMotor()= 0;

	virtual int getMotorCurrent()= 0;

	virtual void enableHardRegulation(bool enable)= 0;

private:

protected:

};

#endif
