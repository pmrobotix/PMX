#ifndef ASSERV_HPP_
#define ASSERV_HPP_

#include <stdlib.h>

#include "../../Log/LoggerFactory.hpp"
#include "../Asserv.Insa/AsservInsa.hpp"
#include "MovingBase.hpp"

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
	MovingBase movingBase_;

	AsservInsa asservinsa_;

	bool ignoreRearCollision_;
	bool ignoreFrontCollision_;

public:

	/*!
	 * \brief Constructor.
	 *
	 */
	Asserv(std::string botId)
			: movingBase_(botId, *this), asservinsa_()
	{

		//asservinsa_.setMovingBase(&movingBase_); //doit etre surchargé

		ignoreRearCollision_ = false;
		ignoreFrontCollision_ = false;
	}

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
	MovingBase & base()
	{
		return movingBase_;
	}

	virtual void startMotionTimerAndOdo();

	void stopMotionTimerAndOdo();

	void freeMotion();

	// if distance <0, move backward
	TRAJ_STATE cc_move(float distance_mm);

	float pos_getX_mm();
	float pos_getY_mm();
	// angle in radian
	float pos_getTheta();
	// angle in degrees
	float pos_getThetaInDegree();


	void stop();

};

#endif
