/*!
 * \file
 * \brief Définition de la classe ActionManagerTest.
 */

#ifndef TEST_ACTIONMANAGERTEST_HPP
#define TEST_ACTIONMANAGERTEST_HPP

#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref pmx::ActionManager.
 */
class ActionManagerTest: public UnitTest
{
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ActionManagerTest()
			: UnitTest("ActionManagerTest")
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
