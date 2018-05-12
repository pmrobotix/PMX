#ifndef COMMON_ASSERV_MOVINGBASE_HPP_
#define COMMON_ASSERV_MOVINGBASE_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "EncoderControl.hpp"
#include "ExtEncoderControl.hpp"
#include "MotorControl.hpp"

/*
 * Base roulante du robot
 */
class MovingBase //: public AAsservElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Base.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("MovingBase");
		return instance;
	}

	float entraxe_mm; //en metre

	float diam_mm;  //en mm

	/*
	 * current distance for Moving.
	 */
	long distTicks_;

	/*!
	 * \brief Left and Right Motor Encoders.
	 */
	EncoderControl encoders_;

	/*!
	 * \brief Left and Right Motor external Encoders.
	 */
	ExtEncoderControl extEncoders_;

	/*!
	 * \brief Left and Right Motors.
	 */
	MotorControl motors_;

	bool collisionDetected_;

public:

	/*!
	 * \brief Constructor.
	 */
	MovingBase(std::string botId); //, Asserv * asserv

	/*!
	 * \brief Destructor.
	 */
	~MovingBase()
	{
	}

	/*!
	 * \brief Cette methode retourne l'objet EncoderControl.
	 * \return encoders_.
	 */
	inline EncoderControl & encoders()
	{
		return encoders_;
	}

	/*!
	 * \brief Cette methode retourne l'objet EncoderControl.
	 * \return extEncoders.
	 */
	inline ExtEncoderControl & extEncoders()
	{
		return extEncoders_;
	}

	/*!
	 * \brief Cette methode retourne l'objet MotorControl.
	 * \return motors_.
	 */
	inline MotorControl & motors()
	{
		return motors_;

	}

	inline void setCollisionDetected(bool b)
	{
		collisionDetected_ = b;
		//this->base().collisionOccured(1); //TODO
	}

	void moveDTime(int power, int timems);

	void moveD(long destTick, long restTick, int power);

	void moveForward(int mm, int power);

	long moveDWaitTrajectory(); //return le reste à parcourir

	void turn(float degree, int power);

	void turnLeft(float degree, int timems);

	void turnRight(float degree, int timems);

	void turnRightTime(int power, int timems);

	void turnLeftTime(int power, int timems);

	void emergencyStop();

};

#endif
