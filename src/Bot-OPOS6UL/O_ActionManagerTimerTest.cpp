/*!
 * \file
 * \brief Implémentation de la classe L_ActionManagerTimerTest.
 */

#include "O_ActionManagerTimerTest.hpp"

#include <unistd.h>

#include "../Common/Action/Actions.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_ActionManagerTimerTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.actions().start();

    logger().info() << " test=>4 actions 5sec" << logs::end;
    robot.actions().addAction(new TestAction(*this, "action1"));
    robot.actions().addAction(new TestAction(*this, "action2"));
    robot.actions().addAction(new TestAction(*this, "action3"));
    robot.actions().addAction(new TestAction(*this, "action4"));

    sleep(5);

    logger().info() << " add action 4 and action5 + tiumer1 and timer2" << logs::end;

    robot.actions().addTimer(new TestTimer(*this, 100, "timer1"));
    robot.actions().addAction(new TestAction(*this, "action5"));
    robot.actions().addTimer(new TestTimer(*this, 500, "timer2"));
    robot.actions().addAction(new TestAction(*this, "action6"));
    sleep(5);

    logger().info() << " stop timer1 add action7, stop timer2" << logs::end;

    robot.actions().stopTimer("timer1");
    robot.actions().addAction(new TestAction(*this, "action7"));
    robot.actions().stopTimer("timer2");
    sleep(2);

    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;
}

TestAction::TestAction(O_ActionManagerTimerTest &amt, std::string name) :
        amt_(amt), name_(name), chrono_("TestAction")
{
    chrono_.start();
    i_ = 0;
}

//execution de la tâche
bool TestAction::execute()
{
    logger().info() << name_ << " executing time=" << chrono_.getElapsedTimeInMicroSec() << " us i=" << i_ << logs::end;

    i_++;
    if (i_ >= 10)
        return false;
    return true;
}

TestTimer::TestTimer(O_ActionManagerTimerTest &amt, int timeSpan_ms, std::string name) :
        amt_(amt), chrono_("TestTimer")
{
    lasttime_ = 0;
    name_ = name;
    timeSpan_us_ = timeSpan_ms * 1000;
    logger().debug() << "timeSpan_ms=" << timeSpan_ms << logs::end;
    this->init(name, timeSpan_us_);
}

void TestTimer::onTimer(utils::Chronometer chrono)
{

    int diff = chrono.getElapsedTimeInMicroSec() - lasttime_;
    logger().info() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us " << " diff="
            << diff << logs::end;
    lasttime_ = chrono.getElapsedTimeInMicroSec();
}

void TestTimer::onTimerEnd(utils::Chronometer chrono)
{
    logger().info() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us"
            << logs::end;

}
