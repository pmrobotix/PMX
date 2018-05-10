#ifndef SERVOOBJECTSAX12_HPP_
#define SERVOOBJECTSAX12_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "AActionsElement.hpp"

class AServoDriver;

class ServoObjectsAx12: public AActionsElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoObjectsAx12.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoObjectsAx12");
		return instance;
	}

	AServoDriver* servodriver;

	int nb_servos_;
	int *servo_list_;

public:

	/*!
	 * \brief Constructor.
	 */
	ServoObjectsAx12(Actions & actions);

	/*!
	 * \brief Destructor.
	 */
	~ServoObjectsAx12();

	/*!
	 * \brief Destructor.
	 * \param keep_sec : 1 sec par defaut et release, 0 (pas de release), ou n secondes et release
	 */
	void deploy(int servo, int pos, int keep_millisec = -1);
	void turn(int servo, int speed, int keep_millisec);
	void release(int servo);
	void hold(int servo);
	void releaseAll();
	void holdAll();

	void setSpeed(int servo, int speed);
	void setSpeedAll(int speed);
	void detectAll();

};

#endif
