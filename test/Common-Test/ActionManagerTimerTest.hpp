/*!
 * \file
 * \brief Définition de la classe ActionManagerTimerTest.
 */

#ifndef TEST_ACTIONMANAGERTEST_HPP
#define TEST_ACTIONMANAGERTEST_HPP

#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref pmx::ActionManagerTimerTest.
 */
class ActionManagerTimerTest: public UnitTest
{
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ActionManagerTimerTest() :
			UnitTest("ActionManagerTimerTest - Actions + Timer")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ActionManagerTimerTest()
	{
	}

	virtual void suite();

	/*!
	 * \brief Teste la méthode \ref ActionManager::count.
	 */
	void testCountActions();

	/*!
	 * \brief Teste la méthode \ref ActionManager::addAction.
	 */
	void testAddAction();

	/*!
	 * \brief Teste la méthode \ref ActionManager::clearActions.
	 */
	void testClearActions();

	/*!
	 * \brief Teste la méthode \ref ActionManager::execute.
	 */
	void testExecute();

	/*!
	 * \brief Teste la méthode \ref ActionManager::count.
	 */
	void testCountTimers();
};
}

#endif
