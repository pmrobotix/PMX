#ifndef SERVOOBJECTSSYSTEM_HPP_
#define SERVOOBJECTSSYSTEM_HPP_

#include "../../Log/LoggerFactory.hpp"
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

	void leftDeploy(double percent = 75, bool keep = false);
	void leftRelease();
	void leftHold();

	void centreDeploy(double percent = 75, bool keep = false);
	void centreRelease();
	void centreHold();

	void rightDeploy(double percent = 75, bool keep = false);
	void rightRelease();
	void rightHold();

	void releaseAll();
	void holdAll();

};

#endif
