/*
 * Pour les servomoteurs standards
 */

#ifndef ASERVODRIVER_HPP_
#define ASERVODRIVER_HPP_

class AServoDriver
{

public:

	/*!
	 * \brief single instance creation.
	 */
	static AServoDriver * create();

	/*!
	 * \brief Destructor.
	 */
	virtual ~AServoDriver()
	{
	}

	/*!
	 * \brief Constructor.
	 */
	AServoDriver()
	{
	}

	virtual void hold(int servoId) = 0;

	virtual void setPosition(int servoId, int pos) = 0;

	virtual void release(int servoId) = 0;

private:

protected:

};

#endif
