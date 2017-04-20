#ifndef SERVOOBJECTSSYSTEM_HPP_
#define SERVOOBJECTSSYSTEM_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "AActionsElement.hpp"

class AServoDriver;

class ServoObjectsSystem: public AActionsElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoObjectsSystem.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoObjectsSystem");
		return instance;
	}

	AServoDriver* servodriver;

public:

	/*!
	 * \brief Constructor.
	 */
	ServoObjectsSystem(Actions & actions);

	/*!
	 * \brief Destructor.
	 */
	~ServoObjectsSystem();

	/*!
	 * \brief Destructor.
	 * \param keep_sec : 1 sec par defaut et release, 0 (pas de release), ou n secondes et release
	 */
	void deploy(ServoLabel servo, double percent, int keep_sec = 1);
	void release(ServoLabel servo);
	void hold(ServoLabel servo);
	void releaseAll();
	void holdAll();

	//deprecated
	void leftDeploy(double percent = 0, bool keep = false);
	void leftRelease();
	void leftHold();
	void centreDeploy(double percent = 0, bool keep = false);
	void centreRelease();
	void centreHold();
	void rightDeploy(double percent = 0, bool keep = false);
	void rightRelease();
	void rightHold();

};

#endif
