/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTimerTest.
 */

#include "../../src/Common/Action/ActionManagerTimer.hpp"
#include "ActionManagerTimerTest.hpp"

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

class MockTimer: public ITimerListener
{
public:

	MockTimer()
	{
	}

	virtual ~MockTimer()
	{
	}

	virtual void onTimer(utils::Chronometer chrono)
	{

	}

	virtual void onTimerEnd(utils::Chronometer chrono)
	{

	}
};

void test::ActionManagerTimerTest::suite()
{
	this->testCountActions();
	this->testAddAction();
	this->testClearActions();
	this->testExecute();

	this->testCountTimers();
}

void test::ActionManagerTimerTest::testCountActions()
{
	/*! \todo Finalize the tests */
	ActionManagerTimer manager;
	this->assert(manager.countActions() == 0,
			"ActionManagerTimer::count() ne renvoie pas 0 pour un nouvel ActionManagerTimer");
	MockAction *action = new MockAction();
	manager.addAction(action);
	this->assert(manager.countActions() == 1,
			"ActionManagerTimer::count() ne renvoie pas 1 après un ajout");

	manager.clearActions();
	this->assert(manager.countActions() == 0,
			"ActionManagerTimer::count() ne renvoie pas 0 après clearActions()");
	delete action;

}

void test::ActionManagerTimerTest::testAddAction()
{
	/*! \todo Finalize the tests */
}

void test::ActionManagerTimerTest::testClearActions()
{
	/*! \todo Finalize the tests */
}

void test::ActionManagerTimerTest::testExecute()
{
	/*! \todo Finalize the tests */
}

void test::ActionManagerTimerTest::testCountTimers()
{

}

