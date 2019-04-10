//utilise directement les moteurs et leurs fonctionnalités (internes ou via une carte d'asserv).

#ifndef COMMON_ASSERV_MOTORCONTROL_HPP_
#define COMMON_ASSERV_MOTORCONTROL_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"

class AAsservDriver;


class MotorControl
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref MotorControl.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("MotorControl");
		return instance;
	}

	AAsservDriver* asservdriver;

public:

	/*!
	 * \brief Constructor.
	 */
	MotorControl(std::string botid);//Asserv & asserv

	/*!
	 * \brief Destructor.
	 */
	~MotorControl();

	/*!
	 * \brief runMotorLeft
	 * if regulation enabled  => power in ticks per second -860 / +860 (depends on drivers)
	 * if regulation disabled => power in percentage -100 / +100
	 */
	void runMotorLeft(int power, int timems);
	/*!
	 * \brief runMotorRight
	 * if regulation enabled  => power in ticks per second -860 / +860 (depends on drivers)
	 * if regulation disabled => power in percentage -100 / +100
	 */
	void runMotorRight(int power, int timems);

	/*!
	 * \brief stop both motors.
	 */
	void stopMotors();

	//number of ticks to achieve using power.
	void setMotorLeftPosition(int power, long ticks);
	void setMotorRightPosition(int power, long ticks);

};

#endif
