/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTimerTest.
 */

#include "../../src/Common/Action/ActionManagerTimer.hpp"
#include "ActionManagerTimerTest.hpp"

#include <chrono>
#include <sstream>
#include <string>
#include <thread>

#include "../../src/Common/Action/IAction.hpp"
#include "../../src/Common/Action/ITimerListener.hpp"
#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Common/Action/ITimerPosixListener.hpp"
#include "../../src/Log/Logger.hpp"
#include "../../src/Log/LoggerFactory.hpp"

using namespace utils;

void test::ActionManagerTimerTest::suite() {
    //test de l'ancien timer
    //this->testCount();
    //this->testExecute();

//    this->testCountPosix();
    //this->testExecutePosix();

    this->testExecutePosixBig();
}

class MockAction: public IAction {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref MockAction.
     */
    static inline const logs::Logger& logger() {
        static const logs::Logger &instance = logs::LoggerFactory::logger("test::MockAction");
        return instance;
    }

    long int lasttime_;
    utils::Chronometer chronoA_;
    long nbmax_;
    long nb_;

public:

    MockAction(int num, long nbmax) :
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

    virtual ~MockAction() {
    }

    bool execute() {
        long int timechrono = chronoA_.getElapsedTimeInMicroSec();
        //if (nb_ == 0) lasttime_ = timechrono;
        //if (nb_ % 1000 == 0)
           logger().info() << "executing " << name_ << " nb=" << nb_ << " ... t=" << timechrono - lasttime_ << "us" << logs::end;
           //std::this_thread::sleep_for(std::chrono::microseconds(1000));
        lasttime_ = timechrono;
        nb_++;
        if (nb_ < nbmax_) {
            return true; //true si l'éxecution doit être conservée.
        }
        else {
            logger().info() << "===> last executing " << name_ << " nb=" << nb_ << logs::end;
            logger().info() <<  logs::end;
            nb_ = 0;
            lasttime_ = 0;
            chronoA_.stop();
            return false;
        }
    }

};

class MockPosixTimer: public ITimerPosixListener {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref MockPosixTimer.
     */
    static inline const logs::Logger& logger() {
        static const logs::Logger &instance = logs::LoggerFactory::logger("test::MockPosixTimer");
        return instance;
    }

    //reference vers le pere
    test::ActionManagerTimerTest &actionManagerTimerTest_;
    long last_t_;

public:

    MockPosixTimer(std::string label, int time_ms, test::ActionManagerTimerTest &actionManagerTimerTest) :
            actionManagerTimerTest_(actionManagerTimerTest)
    {
        this->init(label, time_ms*1000);
        last_t_ = 0;
    }

    virtual ~MockPosixTimer() {
    }

    void onTimer(utils::Chronometer chrono) {
        long t = chrono.getElapsedTimeInMicroSec();
        logger().info() << "onTimer executing " << name_ << "... t=" << t << " us  : " << t - last_t_ << logs::end;
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        //logger().info() << "onTimer executing finished " << name_ << "... t=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
        last_t_ = t;
    }

    void onTimerEnd(utils::Chronometer chrono) {
        logger().info() << "onTimerEnd executing " << name_ << "... t=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
    }
};

class MockTimer: public ITimerListener {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref MockTimer.
     */
    static inline const logs::Logger& logger() {
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
        name_ = label;
    }

    virtual ~MockTimer() {
    }

    void onTimer(utils::Chronometer chrono) {
        logger().info() << "onTimer executing " << name_ << "... t=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        logger().info() << "onTimer executing finished " << name_ << "... t=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
    }

    void onTimerEnd(utils::Chronometer chrono) {
        logger().info() << "onTimerEnd executing " << name_ << "... t=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
    }
};

void test::ActionManagerTimerTest::testCountPosix() {
    logger().info() << "ActionManagerTimerTest::testCount()..." << logs::end;
    ActionManagerTimer manager;
    this->assert(manager.countActions() == 0, "ActionManagerTimer::countActions() ne renvoie pas 0 pour un nouvel ActionManagerTimer");
    this->assert(manager.countPTimers() == 0, "ActionManagerTimer::countPTimers() ne renvoie pas 0 pour un nouvel ActionManagerTimer");

    MockAction *action = new MockAction(1, 10);
    manager.addAction(action);
    this->assert(manager.countActions() == 1, "ActionManagerTimer::countActions() ne renvoie pas 1 après un ajout");

    manager.debugActions();

    MockPosixTimer *ptimer = new MockPosixTimer("MockPosixTimer1", 100, *this);
    manager.addTimer(ptimer);
    this->assert(manager.countPTimers() == 1, "ActionManagerTimer::countPTimers() ne renvoie pas 1 après un ajout");

    manager.debugPTimers();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    manager.clearActions();
    this->assert(manager.countActions() == 0, "ActionManagerTimer::countActions() ne renvoie pas 0 après clearActions()");
    delete action;

    manager.clearTimers();
    this->assert(manager.countPTimers() == 0, "ActionManagerTimer::countTimers() ne renvoie pas 0 après clearPTimers()");
    delete ptimer;
}
void test::ActionManagerTimerTest::testCount() {
    logger().info() << "ActionManagerTimerTest::testCount()..." << logs::end;
    ActionManagerTimer manager;
    this->assert(manager.countActions() == 0, "ActionManagerTimer::countActions() ne renvoie pas 0 pour un nouvel ActionManagerTimer");

    this->assert(manager.countTimers() == 0, "ActionManagerTimer::countTimers() ne renvoie pas 0 pour un nouvel ActionManagerTimer");

    MockAction *action = new MockAction(1, 10);
    manager.addAction(action);
    this->assert(manager.countActions() == 1, "ActionManagerTimer::countActions() ne renvoie pas 1 après un ajout");

    manager.debugActions();

    MockTimer *timer = new MockTimer("MockTimer1", *this, 100);
    manager.addTimer(timer);
    this->assert(manager.countTimers() == 1, "ActionManagerTimer::countTimers() ne renvoie pas 1 après un ajout");

    manager.debugTimers();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    manager.clearActions();
    this->assert(manager.countActions() == 0, "ActionManagerTimer::countActions() ne renvoie pas 0 après clearActions()");
    delete action;

    manager.clearTimers();
    this->assert(manager.countTimers() == 0, "ActionManagerTimer::countTimers() ne renvoie pas 0 après clearTimers()");

    delete timer;

    logger().info() << "ActionManagerTimerTest::testCount()... OK" << logs::end;
}

void test::ActionManagerTimerTest::testExecute() {
    logger().info() << "ActionManagerTimerTest::testExecute()..." << logs::end;
    ActionManagerTimer manager;

    MockAction *action1 = new MockAction(1, 10);
    MockAction *action2 = new MockAction(2, 20);
    MockTimer *timer1 = new MockTimer("timer1", *this, 1000);
    MockTimer *timer2 = new MockTimer("timer2", *this, 500);

    manager.addTimer(timer1);
    manager.addTimer(timer2);
    manager.addAction(action1);

    manager.start("ActionsAndTimers", 1);

    logger().debug() << "manager.start(actionsAndTimers)... wait 1sec" << logs::end;
    std::this_thread::sleep_for(std::chrono::seconds(8));
    logger().debug() << "sleep(1) done" << logs::end;
    manager.stopTimer("timer1");
    manager.addAction(action2);
    manager.addAction(action1);

    manager.debugTimers();
    logger().debug() << "wait 4sec" << logs::end;
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    manager.pause(true);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    manager.pause(false);
    manager.addAction(action1);
    manager.addTimer(timer1);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    manager.stopTimer("timer2");
    manager.debugTimers();

    manager.stop(); //remplacer par pause et reprendre
    logger().info() << "ActionManagerTimerTest::testExecute()... OK" << logs::end;
}

void test::ActionManagerTimerTest::testExecutePosixBig() {
    logger().info() << "ActionManagerTimerTest::testExecutePosix()..." << logs::end;


    ActionManagerTimer manager;

    MockAction *action1 = new MockAction(1, 10);
    MockAction *action2 = new MockAction(2, 20);
    MockAction *action3 = new MockAction(3, 10);
    MockPosixTimer *ptimer1 = new MockPosixTimer("ptimer1", 250, *this);
    MockPosixTimer *ptimer2 = new MockPosixTimer("ptimer2", 500, *this);


    manager.debugPTimers();
    manager.debugActions();

    manager.start("actionsAndTimers", 2);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    manager.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    manager.addTimer(ptimer1);
    manager.addTimer(ptimer2);
    manager.addAction(action1);
    manager.addAction(action3);


    std::this_thread::sleep_for(std::chrono::seconds(1));
    manager.start("actionsAndTimersv2", 20);



    std::this_thread::sleep_for(std::chrono::seconds(4));
    logger().info() << "add actions 1+2..." << logs::end;
    manager.addAction(action1);
    manager.addAction(action2);
//    manager.addAction(action3);
    manager.addTimer(ptimer1);
//
    std::this_thread::sleep_for(std::chrono::seconds(2));
    manager.debugPTimers();
    manager.debugActions();

    manager.stop(); //remplacer par pause et reprendre
    logger().info() << "ActionManagerTimerTest::testExecutePosixBig()... END" << logs::end;
}

void test::ActionManagerTimerTest::testExecutePosix() {
    logger().info() << "ActionManagerTimerTest::testExecutePosix()..." << logs::end;
    ActionManagerTimer manager;

    MockAction *action1 = new MockAction(1, 1000);
    MockAction *action2 = new MockAction(2, 20);
    MockAction *action3 = new MockAction(3, 30);
    MockPosixTimer *ptimer1 = new MockPosixTimer("ptimer1", 1000, *this);
    MockPosixTimer *ptimer2 = new MockPosixTimer("ptimer2", 200, *this);

    manager.addTimer(ptimer1);
    manager.addTimer(ptimer2);
    manager.addAction(action1);
    manager.addAction(action3);

    manager.debugPTimers();
    manager.debugActions();

    manager.start("actionsAndTimers", 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    manager.debugPTimers();
    manager.debugActions();

    logger().debug() << "manager.start(actionsAndTimers)... wait 5sec" << logs::end;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    logger().debug() << "sleep(5) done" << logs::end;


     manager.stopPTimer("ptimer1");


     manager.addAction(action2);
     manager.addAction(action1);

     manager.debugPTimers();
     logger().debug() << "wait 120ms - pause" << logs::end;
     std::this_thread::sleep_for(std::chrono::milliseconds(120));
     manager.pause(true);
     logger().debug() << "wait 4sec - pause" << logs::end;
     std::this_thread::sleep_for(std::chrono::seconds(4));
     manager.pause(false);

     //manager.addAction(action1);
     manager.addTimer(ptimer1);
     std::this_thread::sleep_for(std::chrono::seconds(2));
     manager.stopPTimer("ptimer2");
     manager.stopPTimer("ptimer1");
     manager.debugPTimers();

     manager.addTimer(ptimer1);
     manager.addTimer(ptimer1);
     manager.debugPTimers();
     std::this_thread::sleep_for(std::chrono::seconds(2));
     manager.stopAllPTimers();
     manager.debugPTimers();


    manager.stop(); //remplacer par pause et reprendre
    logger().info() << "ActionManagerTimerTest::testExecutePosix()... END" << logs::end;
}

