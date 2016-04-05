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

	//0=>LEFT with coordinate x, y
	//1=>RIGHT with coordinate 3000-x, y
	bool matchColorPosition_;

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
	AsservInsa * insa();

	virtual void startMotionTimerAndOdo();

	void stopMotionTimerAndOdo();

	void freeMotion();
	void assistedHandling();

	//alsolute motion
	TRAJ_STATE doLineAbs(float distance_mm); // if distance <0, move backward
	TRAJ_STATE doRotateAbs(float degrees);
	TRAJ_STATE doRotateLeft(float degrees);
	TRAJ_STATE doRotateRight(float degrees);

	//relative motion (depends on current position of the robot)
	TRAJ_STATE doRotateTo(float thetaInDegree);
	TRAJ_STATE doMoveForwardTo(float xMM, float yMM);
	TRAJ_STATE doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
	TRAJ_STATE doMoveBackwardTo(float xMM, float yMM);
	TRAJ_STATE doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
	TRAJ_STATE doMoveArcRotate(int degrees, float radiusMM);



	void findPidAD(float degrees, int mm, int sec);

	void configureAlphaPID(float Ap, float Ai, float Ad);
	void configureDeltaPID(float Dp, float Di, float Dd);

	void setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees, bool matchColor);


	void setMatchColorPosition(bool c)
	{
		matchColorPosition_ = c;
	}
	float getRelativeX(float x)
	{
		if (matchColorPosition_ != 0)
		{
			return 3000 - x;
		}
		return x;
	}
	float getRelativeAngle(float degrees)
	{
		if (matchColorPosition_ != 0)
		{
			return 180 - degrees;
		}
		return degrees;
	}

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
