#ifndef COMMON_ASSERV_HPP_
#define COMMON_ASSERV_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Asserv.Insa/AsservInsa.hpp"

/*!
 * Asservissement of the robot.It contains default elements.
 */
class Asserv
{
private:

	/*!
	 * \brief Return \ref Logger linked to \ref Asserv.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Asserv");
		return instance;
	}
protected:
	/*!
	 * \brief motorisation = motors + encoders
	 */
	MovingBase * pMovingBase_;

	AsservInsa * pAsservInsa_;

	bool ignoreRearCollision_;
	bool ignoreFrontCollision_;

public:

	/*!
	 * \brief Constructor.
	 *
	 */
	Asserv(std::string botId);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Asserv()
	{
	}

	/*!
	 * \brief return objet movingBase.
	 * \return movingBase_.
	 */
	MovingBase * base();

	virtual void startMotionTimerAndOdo();

	void stopMotionTimerAndOdo();

	void freeMotion();

	// if distance <0, move backward
	TRAJ_STATE cc_move(float distance_mm);

	void findPidAD(float degrees, int mm, int sec);

	void configureAlphaPID(float Ap, float Ai, float Ad);

	void configureDeltaPID(float Dp, float Di, float Dd);

	void setPosition(float x_mm, float y_mm, float degrees);

	float pos_getX_mm();
	float pos_getY_mm();
	// angle in radian
	float pos_getTheta();
	// angle in degrees
	float pos_getThetaInDegree();

	void setVmax(float vmax);
	void setAccel(float acc);
	void setDecel(float dec);

};

#endif
