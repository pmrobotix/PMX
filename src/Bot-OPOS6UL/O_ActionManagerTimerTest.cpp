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

void O_ActionManagerTimerTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.actions().start();

    robot.actions().addAction(new TestAction(*this));
    robot.actions().addAction(new TestAction(*this));
    robot.actions().addTimer(new TestTimer(*this, 100, "timer1"));
    robot.actions().addTimer(new TestTimer(*this, 500, "timer2"));

    sleep(1);
    robot.actions().addAction(new TestAction(*this));
    robot.actions().stopTimer("timer1");
    robot.actions().stopTimer("timer2");
    sleep(1);
    robot.actions().addAction(new TestAction(*this));
    sleep(1);

    logger().info() << "Happy End." << logs::end;
}

TestAction::TestAction(O_ActionManagerTimerTest & amt) :
        amt_(amt), chrono_("TestAction")
{
    chrono_.start();
    i_ = 0;
}

//execution de la tâche
bool TestAction::execute()
{
    logger().info() << " !!!!! execution time=" << chrono_.getElapsedTimeInMicroSec() << " us i=" << i_ << logs::end;

    i_++;
    if (i_ >= 5)
        return false;
    return true;
}

TestTimer::TestTimer(O_ActionManagerTimerTest & amt, int timeSpan_ms, std::string name) :
        amt_(amt), chrono_("TestTimer")
{
    nameListener_ = name;
    lasttime_ = 0;
    timeSpan_ms_ = timeSpan_ms;
    logger().debug() << "timeSpan_ms=" << timeSpan_ms << logs::end;
}

void TestTimer::onTimer(utils::Chronometer chrono)
{
    logger().info() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
}

void TestTimer::onTimerEnd(utils::Chronometer chrono)
{
    logger().info() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us"
            << logs::end;

}
