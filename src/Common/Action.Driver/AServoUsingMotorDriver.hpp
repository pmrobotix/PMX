/* pour les moteurs utilisés en mode servomotor.
 *
 */
#ifndef ASERVOUSINGMOTORDRIVER_HPP_
#define ASERVOUSINGMOTORDRIVER_HPP_

class AServoUsingMotorDriver
{

public:

	/*!
	 * \brief single instance creation.
	 */
	static AServoUsingMotorDriver * create();

	/*!
	 * \brief Destructor.
	 */
	virtual ~AServoUsingMotorDriver()
	{
	}

	/*!
	 * \brief Constructor.
	 */
	AServoUsingMotorDriver()
	{
	}

	virtual void setMotorPosition(int power, int pos, int ramptimems) = 0;

	virtual long getInternalEncoder()= 0;

	virtual void resetEncoder(int pos)= 0;

	virtual void stopMotor()= 0;

	virtual int getMotorCurrent()= 0;

	virtual void enableHardRegulation(bool enable)= 0;

private:

protected:

};

#endif
