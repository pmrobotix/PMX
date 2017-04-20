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

	SERVO_BASCULE,

	SERVO_PINCE,

	SERVO_LEFT,

	SERVO_RIGHT,

	SERVO_CENTRE,

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



	virtual void hold(ServoLabel) = 0;

	virtual void setPosition(ServoLabel, double percent_pos) = 0;

	virtual void release(ServoLabel) = 0;

	virtual void setRate(ServoLabel, int millisec) = 0;

private:

protected:

};

#endif
