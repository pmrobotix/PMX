/*!
 * \file
 * \brief Définition de la classe L_ActionManagerTimerTest.
 */

#ifndef LEGOEV3ACTIONMANAGERTIMERTEST_HPP_
#define	LEGOEV3ACTIONMANAGERTIMERTEST_HPP_

#include <string>

#include "../Common/Action/IAction.hpp"
#include "../Common/Action/ITimerListener.hpp"
#include "../Common/FunctionalTest.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'action manager avec timer
 */
class L_ActionManagerTimerTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_ActionManagerTimerTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_ActionManagerTimerTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_ActionManagerTimerTest()
			: FunctionalTest("ActionManagerTimer", "actions + timers")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_ActionManagerTimerTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

/*!
 * \brief Cette action permet de
 *
 */
class TestAction: public IAction
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref TestAction.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("TestAction");
		return instance;
	}

	/*!
	 * \brief Référence vers le test.
	 */
	L_ActionManagerTimerTest & amt_;

	utils::Chronometer chrono_;

	int i_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 * \param amt
	 *        Reference vers l'objet associée.
	 */
	TestAction(L_ActionManagerTimerTest & amt);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~TestAction()
	{
		logger().debug() << "~TestAction()" << logs::end;
	}

	/*!
	 * \brief Execution de l'action.
	 */
	virtual bool execute();

	/*!
	 * \brief Retourne la description de l'action.
	 */
	virtual inline std::string info()
	{
		return "TestAction";
	}
};

/*!
 * \brief Ce timer
 *
 */
class TestTimer: public ITimerListener
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref TestTimer.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("TestTimer");
		return instance;
	}

	/*!
	 * \brief Référence vers le test.
	 */
	L_ActionManagerTimerTest & amt_;

	utils::Chronometer chrono_;


public:

	/*!
	 * \brief Constructeur de la classe.
	 * \param amt
	 *        Reference vers l'objet associée.
	 */
	TestTimer(L_ActionManagerTimerTest & amt, int timeSpan_ms, std::string name);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~TestTimer()
	{
		logger().debug() << "~TestTimer()" << logs::end;
	}

	virtual void onTimer(utils::Chronometer chrono);

	virtual void onTimerEnd(utils::Chronometer chrono);

	/*!
	 * \brief Retourne la description de l'action.
	 */
	virtual inline std::string info()
	{
		return name_;
	}
};

#endif
