/*!
 * \file
 * \brief Définition de la classe ActionManagerTest.
 */

#ifndef TEST_ACTIONMANAGERTEST_HPP
#define TEST_ACTIONMANAGERTEST_HPP

#include "../Suite/DevUnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref pmx::ActionManager.
 */
class ActionManagerTest: public DevUnitTest
{
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ActionManagerTest()
			: DevUnitTest("ActionManagerTest")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ActionManagerTest()
	{
	}

	virtual void suite();

	/*!
	 * \brief Teste la méthode \ref ActionManager::count.
	 */
	void testCount();

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
};
}

#endif
