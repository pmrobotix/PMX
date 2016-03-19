/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTest.
 */

#include "../../src/Common/Action/ActionManager.hpp"
#include "ActionManagerTest.hpp"

#include <string>


class MockAction: public IAction
{
public:

	MockAction()
	{
	}

	virtual ~MockAction()
	{
	}

	virtual bool execute()
	{
		return true;
	}

	virtual std::string info()
	{
		return "MockAction";
	}
};

void test::ActionManagerTest::suite()
{
	this->testCount();
	this->testAddAction();
	this->testClearActions();
	this->testExecute();
}

void test::ActionManagerTest::testCount()
{
	/*! \todo Finalize the tests */
	ActionManager manager;
	this->assert(manager.count() == 0, "ActionManager::count() ne renvoie pas 0 pour un nouvel ActionManager");
	MockAction *action = new MockAction();
	manager.addAction(action);
	this->assert(manager.count() == 1, "ActionManager::count() ne renvoie pas 1 après un ajout");

	manager.clearActions();
	this->assert(manager.count() == 0, "ActionManager::count() ne renvoie pas 0 après clearActions()");
	delete action;
}

void test::ActionManagerTest::testAddAction()
{
	/*! \todo Finalize the tests */
}

void test::ActionManagerTest::testClearActions()
{
	/*! \todo Finalize the tests */
}

void test::ActionManagerTest::testExecute()
{
	/*! \todo Finalize the tests */
}
