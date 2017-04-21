#ifndef COMMON_ASSERV_HPP_
#define COMMON_ASSERV_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Asserv.Driver/AAsservDriver.hpp"

class AsservInsa;

class AAsservDriver;

class MovingBase;

class Robot;

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

	/*!
	 * \brief asservissement interne. il peut etre remplacé par un asserv externe
	 */
	AsservInsa * pAsservInsa_;

	bool useInternalAsserv_; //permet de choisir entre l'utilisattion d'un asserv interne (type insa) ou ext (en utilisant les asserdriver).

	bool ignoreRearCollision_;
	bool ignoreFrontCollision_;

	//0=>LEFT with coordinate x, y, angle
	//1=>RIGHT with coordinate 3000-x, y , -angle
	bool matchColorPosition_;

	Robot * probot_; //reference du parent

public:

	/*!
	 * \brief Constructor.
	 *
	 */
	Asserv(std::string botId, Robot * robot);

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

	AAsservDriver* asservdriver;

	//Gestion de l'asservissement
	virtual void startMotionTimerAndOdo();
	virtual void stopMotionTimerAndOdo();

	//modes d'arret de l'asservissement
	virtual void freeMotion();
	virtual void assistedHandling();
	//absolute motion
	virtual TRAJ_STATE doLineAbs(float distance_mm); // if distance <0, move backward
	virtual TRAJ_STATE doRotateAbs(float degrees);
	virtual TRAJ_STATE doRotateLeft(float degrees);
	virtual TRAJ_STATE doRotateRight(float degrees);
	//relative motion (depends on current position of the robot)
	virtual TRAJ_STATE doRotateTo(float thetaInDegree);
	virtual TRAJ_STATE doMoveForwardTo(float xMM, float yMM);
	virtual TRAJ_STATE doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
	virtual TRAJ_STATE doMoveBackwardTo(float xMM, float yMM);
	virtual TRAJ_STATE doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
	virtual TRAJ_STATE doMoveArcRotate(int degrees, float radiusMM);

	//void findPidAD(float degrees, int mm, int sec);
	//void findPidLR(float posl, int posr, int sec);
	//void configureAlphaPID(float Ap, float Ai, float Ad);
	//void configureDeltaPID(float Dp, float Di, float Dd);

	/*!
	 * Attention startMotionTimerAndOdo() est necessaire auparavant pour configurer vTops et donc la position du robot
	 */
	virtual void setPositionAndColor(float x_mm, float y_mm, float theta_degrees, bool matchColor); //matchColor = 0 =>en bas à gauche du log svg

	void setMatchColorPosition(bool c)
	{
		matchColorPosition_ = c;
	}

	inline float getRelativeX(float x, float width = 0.0)
	{
		if (matchColorPosition_ != 0)
		{
			return 3000 - x - width;
		}
		return x;
	}
	inline float getRelativeAngle(float degrees)
	{
		if (matchColorPosition_ != 0)
		{
			return 180 - degrees;
		}
		return degrees;
	}

	virtual void setFrontCollision();
	virtual void setRearCollision();

	virtual float pos_getX_mm();
	virtual float pos_getY_mm();
	// angle in radian
	virtual float pos_getTheta();
	// angle in degrees
	virtual float pos_getThetaInDegree();

	void ignoreFrontCollision(bool ignore);

	void ignoreRearCollision(bool ignore);
};

#endif
