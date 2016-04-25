#ifndef SERVOOBJECTSSYSTEM_HPP_
#define SERVOOBJECTSSYSTEM_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

class AServoDriver;

#define LEFT_CARPET 8
#define RIGHT_CARPET 1

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

	void leftDeploy(int debug = 0);

	void rightDeploy(int debug = 0);

	void releaseLeft();
	void releaseRight();

	void holdLeft();
	void holdRight();

	void releaseAll();
	void holdAll();

};

#endif
