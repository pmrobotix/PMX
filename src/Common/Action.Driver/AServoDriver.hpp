/*
 * Pour les servomoteurs standards
 */

#ifndef ASERVODRIVER_HPP_
#define ASERVODRIVER_HPP_


/*!
 * \brief Enumération des libellés des servos.
 */
enum ServoLabel
{
	SERVO_FUNNY_ACTION,

	SERVO_HERCULE, //puissance pince

	SERVO_3,

	SERVO_PINCE,

	SERVO_4,

	SERVO_5,

	SERVO_6,
	SERVO_7,
	SERVO_BASCULE,

	SERVO_enumTypeEnd
};

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


	virtual void hold(int) = 0;

	virtual void setPosition(int servo, int pos) = 0;

	virtual void release(int servo) = 0;

	virtual void setRate(int servo, int speed) = 0;

	virtual void turn(int servo, int speed) = 0;

	virtual int getMoving(int servo) = 0;

	virtual int getPos(int servo) = 0;

	virtual int ping(int servo) = 0;

private:

protected:

};

#endif
