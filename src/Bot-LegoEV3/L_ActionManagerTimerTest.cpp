/*!
 * \file
 * \brief Implémentation de la classe L_ActionManagerTimerTest.
 */

#include "L_ActionManagerTimerTest.hpp"

#include <unistd.h>

#include "../Common/Action/Actions.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ActionManagerTimerTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.actions().start();

    robot.actions().addAction(new TestAction(*this, "action1"));
    robot.actions().addAction(new TestAction(*this, "action2"));
    robot.actions().addAction(new TestAction(*this, "action3"));
    robot.actions().addAction(new TestAction(*this, "action4"));

    utils::sleep_for_secs(2);
    robot.actions().addTimer(new TestTimer(*this, 100, "timer1"));
    robot.actions().addAction(new TestAction(*this, "action5"));
    robot.actions().addTimer(new TestTimer(*this, 500, "timer2"));
    robot.actions().addAction(new TestAction(*this, "action6"));

    utils::sleep_for_secs(7);
    robot.actions().stopPTimer("timer1");
    robot.actions().addAction(new TestAction(*this, "action7"));
    robot.actions().stopPTimer("timer2");

    utils::sleep_for_secs(2);
    logger().info() << "Happy End." << logs::end;
}

TestAction::TestAction(L_ActionManagerTimerTest & amt, std::string label) :
        amt_(amt), name_(label), chrono_("TestAction")
{
    chrono_.start();
    i_ = 0;
}

//execution de la tâche
bool TestAction::execute() {
    logger().info() << " execute() " << this->info() << " time=" << chrono_.getElapsedTimeInMicroSec() << " us i=" << i_ << logs::end;

    i_++;
    if (i_ >= 5) return false;
    return true;
}

TestTimer::TestTimer(L_ActionManagerTimerTest & amt, int timeSpan_ms, std::string name) :
        amt_(amt), chrono_("TestTimer")
{
    name_ = name;
    timeSpan_us_ = timeSpan_ms * 1000;
    this->init(name_, timeSpan_us_);
}

void TestTimer::onTimer(utils::Chronometer chrono) {
    logger().info() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;
}

void TestTimer::onTimerEnd(utils::Chronometer chrono) {
    logger().info() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us" << logs::end;

}
