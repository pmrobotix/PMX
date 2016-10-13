#ifndef L_STATE_WAIT90SECONDSACTION_HPP
#define	L_STATE_WAIT90SECONDSACTION_HPP

#include <string>

#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"

class Robot;

/*!
 * \brief Cette action lance le chronometer du robot.
 */
class L_State_Wait90SecAction: public utils::Thread // , public AAutomateState
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_State_Wait90SecAction");
		return instance;
	}

	/*!
	 * \brief Référence vers le robot.
	 */
	Robot & robot_;

	void * data_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_State_Wait90SecAction(Robot& robot, void *data);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ L_State_Wait90SecAction()
	{
	}

	virtual std::string name()
	{
		return "L_State_Wait90SecAction";
	}

	/*!
	 * \brief Execute l'action.
	 */
	virtual void execute();
};

#endif
