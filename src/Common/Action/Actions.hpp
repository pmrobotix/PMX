#ifndef ACTIONS_HPP_
#define ACTIONS_HPP_

#include <string>

#include "../../Log/Logger.hpp"
#include "../../Log/LoggerFactory.hpp"
#include "ActionManagerTimer.hpp"

class IAction;

/*!
 * List of robot actions. It contains all common RobotElement.
 */
class Actions
{
private:

	/*!
	 * \brief Return \ref Logger linked to \ref Actions.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Actions");
		return instance;
	}

	/*!
	 * \brief Assure la gestion des actions/timers du robot en mode asynchrone.
	 */
	ActionManagerTimer actionManagerTimer_;

public:

	/*!
	 * \brief Constructor.
	 *
	 */
	Actions()
	{
	}

	/*!
	 * \brief Destructor.
	 */
	~Actions()
	{
		logger().debug() << "~Actions()" << logs::end;
	}

	/*!
	 * \brief Ajout d'une action.
	 * \param action
	 *        L'action à ajouter.
	 */
	inline void addAction(IAction * action)
	{
		//actionManager_.addAction(action);
		actionManagerTimer_.addAction(action);
	}

	inline void addTimer(ITimerListener * timer)
	{
		actionManagerTimer_.addTimer(timer);
	}

	inline void stopTimer(std::string name)
	{
		actionManagerTimer_.stopTimer(name);
	}

	/*!
	 * \brief Active les actions.
	 *
	 * Cette méthode lance le thread gérant le ActionManager.
	 */
	void start();

	/*!
	 * \brief Arrete le robot et libère les ressources associés.
	 */
	void stop();

	/*!
	 * \brief Arrete le thread sensorManager et actionManager.
	 */
	void waitAndStopManagers();

	/*!
	 * \brief Arrete les composant du robot.
	 */
	void stopDevices();

};

#endif
