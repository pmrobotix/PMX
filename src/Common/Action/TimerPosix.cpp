/* EXAMPLE
 * https://quirk.ch/2009/07/how-to-use-posix-timer-within-c-classes
 *
 * https://www.softprayog.in/tutorials/alarm-sleep-and-high-resolution-timers
 * This code needs to be linked with the RealtTime library (g++ option -l rt) to work.
 */
#include "../Utils/Chronometer.hpp"
#include "TimerPosix.hpp"

utils::Chronometer TimerPosix::chrono = utils::Chronometer("TimerPosix");
