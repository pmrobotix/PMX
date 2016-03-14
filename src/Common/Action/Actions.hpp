#ifndef ACTIONS_HPP_
#define ACTIONS_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "ActionManager.hpp"

/*!
 * List of robot actions. It contains all common RobotElement.
 */
class Actions
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Actions.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"Actions");
		return instance;
	}

	/*!
	 * \brief Assure la gestion des actions du robot en mode asynchrone.
	 */
	ActionManager actionManager_;

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
		actionManager_.addAction(action);
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
