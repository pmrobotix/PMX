/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTimerTest.
 */

#include "../../src/Common/Action/ActionManagerTimer.hpp"
#include "ActionManagerTimerTest.hpp"

#include <unistd.h>
#include <sstream>
#include <string>

#include "../../src/Common/Action/IAction.hpp"
#include "../../src/Common/Action/ITimerListener.hpp"
#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Log/Logger.hpp"
#include "../../src/Log/LoggerFactory.hpp"

class MockAction: public IAction
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref MockAction.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("test::MockAction");
        return instance;
    }

    long int lasttime_;
    utils::Chronometer chronoA_;
    int nbmax_;
    int nb_;

public:

    MockAction(int num, int nbmax) :
            chronoA_("MockAction")
    {
        std::ostringstream oss;
        oss << "MockAction" << num;
        name_ = oss.str();

        lasttime_ = 0;
        chronoA_.start();
        nb_ = 0;
        nbmax_ = nbmax;
    }

    virtual ~MockAction()
    {
    }

    bool execute()
    {

        long int time = chronoA_.getElapsedTimeInMicroSec();
        if (nb_ == 0)
            lasttime_ = time;

        logger().info() << "executing " << name_ << "... t=" << time - lasttime_ << "us" << logs::end;
        lasttime_ = time;
        nb_++;
        if (nb_ < nbmax_) {

            return true; //true si l'éxecution doit être conservée.
        } else {
            nb_ = 0;
            lasttime_ = 0;
            chronoA_.stop();
            return false;
        }
    }

};

class MockTimer: public ITimerListener
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref MockTimer.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("test::MockTimer");
        return instance;
    }

    //reference vers le pere
    test::ActionManagerTimerTest &actionManagerTimerTest_;

public:

    MockTimer(std::string label, test::ActionManagerTimerTest &actionManagerTimerTest, int timeSpan_ms) :
            actionManagerTimerTest_(actionManagerTimerTest)
    {
        timeSpan_ms_ = timeSpan_ms;
        nameListener_ = label;
    }

    virtual ~MockTimer()
    {
    }

    void onTimer(utils::Chronometer chrono)
    {
        logger().info() << "onTimer executing " << nameListener_ << "... t=" << chrono.getElapsedTimeInMicroSec()
                << " us" << logs::end;
    }

    void onTimerEnd(utils::Chronometer chrono)
    {
        logger().info() << "onTimerEnd executing " << nameListener_ << "... t=" << chrono.getElapsedTimeInMicroSec()
                << " us" << logs::end;
    }
};

void test::ActionManagerTimerTest::suite()
{
    this->testCount();
    this->testExecute();
}

void test::ActionManagerTimerTest::testCount()
{
    logger().info() << "ActionManagerTimerTest::testCount()..." << logs::end;
    ActionManagerTimer manager;
    this->assert(manager.countActions() == 0,
            "ActionManagerTimer::countActions() ne renvoie pas 0 pour un nouvel ActionManagerTimer");

    this->assert(manager.countTimers() == 0,
            "ActionManagerTimer::countTimers() ne renvoie pas 0 pour un nouvel ActionManagerTimer");

    MockAction *action = new MockAction(1, 1);
    manager.addAction(action);
    this->assert(manager.countActions() == 1, "ActionManagerTimer::countActions() ne renvoie pas 1 après un ajout");

    manager.debugActions();

    MockTimer *timer = new MockTimer("MockTimer1", *this, 100);
    manager.addTimer(timer);
    this->assert(manager.countTimers() == 1, "ActionManagerTimer::countTimers() ne renvoie pas 1 après un ajout");

    manager.debugTimers();

    manager.clearActions();
    this->assert(manager.countActions() == 0,
            "ActionManagerTimer::countActions() ne renvoie pas 0 après clearActions()");
    delete action;

    manager.clearTimers();
    this->assert(manager.countTimers() == 0, "ActionManagerTimer::countTimers() ne renvoie pas 0 après clearTimers()");
    delete timer;

    logger().info() << "ActionManagerTimerTest::testCount()... OK" << logs::end;
}

void test::ActionManagerTimerTest::testExecute()
{
    logger().info() << "ActionManagerTimerTest::testExecute()..." << logs::end;
    ActionManagerTimer manager;

    MockAction *action1 = new MockAction(1, 10);
    MockAction *action2 = new MockAction(2, 20);
    MockTimer *timer1 = new MockTimer("timer1", *this, 50);
    MockTimer *timer2 = new MockTimer("timer2", *this, 51);

    manager.addTimer(timer1);
    manager.addTimer(timer2);
    manager.addAction(action1);

    manager.start("actionsAndTimers", 50);
    logger().debug() << "manager.start(actionsAndTimers)... wait 1sec" << logs::end;
    sleep(1);
    logger().debug() << "sleep(1) done" << logs::end;
    manager.stopTimer("timer1");
    manager.addAction(action2);
    manager.addAction(action1);

    manager.debugTimers();
    logger().debug() << "wait 120ms" << logs::end;
    usleep(120000);/*
     //manager.pause(true);
     usleep(500000);
     //manager.pause(false);
     //manager.addAction(action1);
     manager.addTimer(timer1);
     usleep(1000000);
     manager.stopTimer("timer2");
     //manager.debugTimers();
     */
    manager.stop(); //remplacer par pause et reprendre
    logger().info() << "ActionManagerTimerTest::testExecute()... OK" << logs::end;
}

