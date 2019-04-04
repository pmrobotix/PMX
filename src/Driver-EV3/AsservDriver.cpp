//drivers...EV3

#include "AsservDriver.hpp"

#include <unistd.h>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

AAsservDriver * AAsservDriver::create(std::string)
{
    static AsservDriver *instance = new AsservDriver();
    return instance;
}

void AsservDriver::reset()
{
    if (_motor_left_.connected())
        _motor_left_.reset();

    if (_motor_right_.connected())
        _motor_right_.reset();
}

AsservDriver::AsservDriver() :
        _motor_right_(OUTPUT_D), _motor_left_(OUTPUT_A), angleR_("in1:i2c1")
{
    logger().debug() << "AsservDriver()" << logs::end;

    if (angleR_.connected()) {
        logger().info() << "[" << angleR_.address() << "] (" << angleR_.driver_name() << ") type_name="
                << angleR_.type_name() << logs::end;
    }else
    {
        logger().error() << "NOT CONNECTED! NO angleR_ !" << logs::end;
    }
    /*
     usleep(10000);
     //sensor angle(INPUT_AUTO, { "ht-nxt-angle" });
     if (angle.connected()) {
     logger().info() << "angle = " << angle.value(0) << logs::end;
     usleep(10000);
     angle.set_mode("ANGLE-ACC");
     usleep(10000);
     logger().debug() << "angle = " << angle.value(0) << logs::end;
     angle.set_command("RESET");
     usleep(25000);
     const mode_set &m = angle.commands();
     std::ostringstream oss;
     oss << "available cmds are ";
     for (mode_set::const_iterator it = m.begin(); it != m.end(); ++it) {
     oss << *it << " ";
     }
     logger().debug() << oss.str() << logs::end;
     logger().debug() << "driver_name() = " << angle.driver_name() << logs::end;
     logger().debug() << "angle = " << angle.value(0) << logs::end;
     //        for (int i = 0; i < 10; i++) {
     //            usleep(500000);
     //            logger().debug() << "angle = " << angle.value(0) << logs::end;
     //        }

     }
     */
    /*
     //test1
     for (unsigned i = 0; i < 4; ++i) {
     motor &m = arrMotors[i];
     if (m.connected()) {

     logger().info() << "(" << 1 + i << ") " << m.driver_name() << " motor on port " << m.address() << logs::end;
     }

     }
     */
//test 2 - a garder
    if (_motor_right_.connected()) {

        logger().info() << "(" << "RIGHT" << ") " << _motor_right_.driver_name() << " motor on port "
                << _motor_right_.address() << " Pol=" << _motor_right_.polarity() << logs::end;
    }else
    {
        logger().error() << "NOT CONNECTED! NO _motor_right_ !" << logs::end;
    }
    if (_motor_right_.connected()) //if both motors are connected, then initialize each motor.
    {
        _motor_right_.reset();

        _motor_right_.set_ramp_down_sp(0);
        _motor_right_.set_ramp_up_sp(0);
        _motor_right_.set_stop_action("brake");

        //_motor_right_.set_position_sp(-10).set_speed_sp(300).run_to_rel_pos();
        /*
         logger().info() << "R=" << _motor_right_.position() << logs::end;
         _motor_right_.set_duty_cycle_sp(-60).run_direct();
         sleep(1);
         logger().info() << "R=" << _motor_right_.position() << logs::end;

         _motor_right_.set_duty_cycle_sp(60).run_direct();
         sleep(1);

         _motor_right_.stop();
         logger().info() << "R=" << _motor_right_.position() << logs::end;
         */
    }

    if (_motor_left_.connected()) {

        logger().info() << "(" << "LEFT " << ") " << _motor_left_.driver_name() << " motor on port "
                << _motor_left_.address() << " Pol=" << _motor_left_.polarity() << logs::end;
    }else
    {
        logger().error() << "NOT CONNECTED! NO _motor_left_ !" << logs::end;
    }

    if (_motor_left_.connected()) //if both motors are connected, then initialize each motor.
    {
        _motor_left_.reset();

        _motor_left_.set_ramp_down_sp(0);
        _motor_left_.set_ramp_up_sp(0);
        _motor_left_.set_stop_action("brake");

        //_motor_left_.set_position_sp(-10).set_speed_sp(300).run_to_rel_pos();
        /*
         logger().info() << "R=" << _motor_left_.position() << logs::end;
         _motor_left_.set_duty_cycle_sp(-60).run_direct();
         sleep(1);
         logger().info() << "R=" << _motor_left_.position() << logs::end;

         _motor_left_.set_duty_cycle_sp(60).run_direct();
         sleep(1);

         _motor_left_.stop();
         logger().info() << "R=" << _motor_left_.position() << logs::end;
         */
    }

    resetEncoders();

    /*
     _motor_porte_droite_.reset();
     sleep(2);
     _motor_porte_droite_.set_position_sp(500).set_speed_sp(300).run_to_rel_pos();
     _motor_porte_gauche_.reset();
     sleep(2);
     _motor_porte_gauche_.set_position_sp(500).set_speed_sp(300).run_to_rel_pos();
     */
    /*
     //test3
     if (_motor_left.connected()) {

     logger().info() << "(" << 1 << ") " << _motor_left.driver_name() << " motor on port " << _motor_left.address()
     << logs::end;
     }

     if (_motor_right.connected()) {

     logger().info() << "(" << 4 << ") " << _motor_right.driver_name() << " motor on port " << _motor_right.address()
     << logs::end;
     }
     */
    /*
     _motor_right = motor(OUTPUT_D);
     if (_motor_right.connected())
     {

     logger().info() << "EV3 Motor (RIGHT) - " << _motor_right.address() << " connected ;CPR="
     << _motor_right.count_per_rot() << " Name=" << _motor_right.driver_name() << " Pol="
     << _motor_right.polarity() << logs::end;
     }
     else
     {

     logger().error() << "ERROR OUTPUT_A - Motor (RIGHT) " << "not connected !!" << logs::end;
     }

     _motor_left = motor(OUTPUT_A);
     if (_motor_left.connected())
     {

     logger().info() << "EV3 Motor (LEFT) - " << _motor_left.address() << " connected ;CPR="
     << _motor_left.count_per_rot() << " Name=" << _motor_left.driver_name() << " Pol="
     << _motor_left.polarity() << logs::end;
     }
     else
     {

     logger().error() << "ERROR OUTPUT_D - Motor (LEFT) " << "not connected !!" << logs::end;
     }

     if (_motor_right.connected()) //if both motors are connected, then initialize each motor.
     {
     _motor_right.reset();
     _motor_right.set_ramp_down_sp(0);
     _motor_right.set_ramp_up_sp(0);
     _motor_right.set_stop_action("brake");

     }

     if (_motor_left.connected()) //if both motors are connected, then initialize each motor.
     {
     _motor_left.reset();
     _motor_left.set_ramp_down_sp(0);
     _motor_left.set_ramp_up_sp(0);
     _motor_left.set_stop_action("brake");

     }*/
}

void AsservDriver::setMotorLeftPosition(int ticks_per_second, long ticks)
{
    limit(ticks_per_second, MAXVALUE_speed_sp);
    //ticks_per_second = -ticks_per_second; //TODO add a parameter to reverse motor inverted or not using polarity function ?

    if (_motor_left_.connected()) {

        //ticks :the units are in tachometer pulse counts, so if you are not using a LEGO motor, then you will need to do some math to convert to/from degrees.
        _motor_left_.set_position_sp(ticks).set_speed_sp(ticks_per_second).run_to_rel_pos();
    }
    /*
     if (_motor_left.connected())
     {
     if (_motor_left.speed_regulation_enabled() == "on") //speed_sp
     {
     limit(power, MAXVALUE_speed_sp); //real MAX speed of EV3
     _motor_left.set_position_sp(ticks).set_speed_sp(power).run_to_rel_pos();

     }
     else if (_motor_left.speed_regulation_enabled() == "off") //duty_cycle_sp
     {
     limit(power, MAXVALUE_duty_cycle_sp);
     _motor_left.set_position_sp(ticks).set_duty_cycle_sp(power).run_to_rel_pos();
     }
     }*/

}

void AsservDriver::setMotorRightPosition(int ticks_per_second, long ticks)
{
    limit(ticks_per_second, MAXVALUE_speed_sp);
    //ticks_per_second = -ticks_per_second;
    if (_motor_right_.connected()) {

        //the units are in tachometer pulse counts, so if you are not using a LEGO motor, then you will need to do some math to convert to/from degrees.
        _motor_right_.set_position_sp(ticks).set_speed_sp(ticks_per_second).run_to_rel_pos();
    }
    /*
     if (_motor_right.connected())
     {
     if (_motor_right.speed_regulation_enabled() == "on") //speed_sp
     {
     limit(power, MAXVALUE_speed_sp); //real MAX speed of EV3
     _motor_right.set_position_sp(ticks).set_speed_sp(power).run_to_rel_pos();

     }
     else if (_motor_right.speed_regulation_enabled() == "off") //duty_cycle_sp
     {
     limit(power, MAXVALUE_duty_cycle_sp);
     _motor_right.set_position_sp(ticks).set_duty_cycle_sp(power).run_to_rel_pos();
     }
     }*/
}

int AsservDriver::limit(int power, int max)
{
    if ((power < -max)) {
        logger().info() << "ERROR Motor power " << power << " exceeded minimum " << -max << "!!" << logs::end;
        power = -max;
    } else if (power > max) {
        logger().info() << "ERROR Motor power " << power << "exceeded maximum " << max << "!!" << logs::end;
        power = max;
    }
    return power;
}

//regulation enabled  => power in ticks per second -860 / +860
//regulation disabled => power in percentage -100 / +100

// Commands: read-only
// Returns a list of commands that are supported by the motor
// controller. Possible values are `run-forever`, `run-to-abs-pos`, `run-to-rel-pos`,
// `run-timed`, `run-direct`, `stop` and `reset`. Not all commands may be supported.
//
// - `run-forever` will cause the motor to run until another command is sent.
// - `run-to-abs-pos` will run to an absolute position specified by `position_sp`
//   and then stop using the action specified in `stop_action`.
// - `run-to-rel-pos` will run to a position relative to the current `position` value.
//   The new position will be current `position` + `position_sp`. When the new
//   position is reached, the motor will stop using the action specified by `stop_action`.
// - `run-timed` will run the motor for the amount of time specified in `time_sp`
//   and then stop the motor using the action specified by `stop_action`.
// - `run-direct` will run the motor at the duty cycle specified by `duty_cycle_sp`.
//   Unlike other run commands, changing `duty_cycle_sp` while running *will*
//   take effect immediately.
// - `stop` will stop any of the run commands before they are complete using the
//   action specified by `stop_action`.
// - `reset` will reset all of the motor parameter attributes to their default value.
//   This will also have the effect of stopping the motor.

//http://www.ev3dev.org/docs/tutorials/tacho-motors/
void AsservDriver::setMotorLeftPower(int percent, int timems)
{

    percent = -percent;
    if (_motor_left_.connected()) {
        limit(percent, MAXVALUE_duty_cycle_sp);
        logger().error() << "LEFT percent = " << percent << logs::end;
        _motor_left_.set_duty_cycle_sp(percent);
    }

    if (timems > 0) {
        _motor_left_.set_time_sp(timems);
        _motor_left_.run_timed();
    } else {
        //forever
        _motor_left_.run_direct();
    }
    /*
     if (_motor_left.connected())
     {
     //with time
     if (_motor_left.speed_regulation_enabled() == "on") //speed_sp
     {
     limit(power, MAXVALUE_speed_sp);

     //Writing sets the target speed in tacho counts per second used when `speed_regulation`
     // is on. Reading returns the current value.
     _motor_left.set_speed_sp(power);

     logger().debug() << "LEFT current motor speed in ticks per second:"
     << _motor_left.speed() << " count_per_rot:" << _motor_left.count_per_rot()
     << logs::end;
     }
     else if (_motor_left.speed_regulation_enabled() == "off") //duty_cycle_sp
     {
     limit(power, MAXVALUE_duty_cycle_sp);

     //Units are in percent. Valid values are -100 to 100. A negative value causes
     // the motor to rotate in reverse. This value is only used when `speed_regulation`
     // is off.
     _motor_left.set_duty_cycle_sp(power);
     }

     if (timems > 0)
     {
     _motor_left.set_time_sp(timems);
     _motor_left.run_timed();
     }
     else
     {
     //forever
     _motor_left.run_forever();
     }
     }
     else
     {
     logger().error() << "LEFT motor not connected !" << logs::end;
     }*/
}

void AsservDriver::setMotorRightPower(int percent, int timems)
{

    percent = -percent;
    if (_motor_right_.connected()) {
        limit(percent, MAXVALUE_duty_cycle_sp);
        logger().error() << "RIGHT percent = " << percent << logs::end;
        _motor_right_.set_duty_cycle_sp(percent);
    }

    if (timems > 0) {
        _motor_right_.set_time_sp(timems);
        _motor_right_.run_timed();
    } else {
        //forever
        _motor_right_.run_direct();
    }
    /*
     if (_motor_right.connected())
     {
     //with time
     if (_motor_right.speed_regulation_enabled() == "on")		//speed_sp
     {
     limit(power, MAXVALUE_speed_sp);

     //Writing sets the target speed in tacho counts per second used when `speed_regulation`
     // is on. Reading returns the current value.
     _motor_right.set_speed_sp(power);

     logger().debug() << "RIGHT current motor speed in ticks per second:"
     << _motor_right.speed() << " count_per_rot:" << _motor_right.count_per_rot()
     << logs::end;
     }
     else if (_motor_right.speed_regulation_enabled() == "off") //duty_cycle_sp
     {
     limit(power, MAXVALUE_duty_cycle_sp);

     //Units are in percent. Valid values are -100 to 100. A negative value causes
     // the motor to rotate in reverse. This value is only used when `speed_regulation`
     // is off.
     _motor_right.set_duty_cycle_sp(power);
     }

     if (timems > 0)
     {
     _motor_right.set_time_sp(timems);
     _motor_right.run_timed();
     }
     else
     {
     //forever
     _motor_right.run_forever();
     }
     }
     else
     {
     logger().error() << "RIGHT motor not connected !" << logs::end;
     }*/
}

long AsservDriver::getLeftExternalEncoder()
{
    if (angleR_.connected()) {
        long ticks = angleR_.getValueDegrees();

        return ticks;
    } else
        return -9999;
}
long AsservDriver::getRightExternalEncoder()
{
    return 0;
}

long AsservDriver::getLeftInternalEncoder()
{
    if (_motor_left_.connected()) {
        //+/- 2,147,483,648
        return -_motor_left_.position();
    } else
        return 0;

}
long AsservDriver::getRightInternalEncoder()
{
    if (_motor_right_.connected()) {
        return -_motor_right_.position();
    } else
        return 0;
}

void AsservDriver::stopMotorLeft()
{
    setMotorLeftPower(0, 0);
    if (_motor_left_.connected()) {
        _motor_left_.stop();
    } else
        logger().error() << "NOT CONNECTED! NO stopMotorLeft" << logs::end;

}
void AsservDriver::stopMotorRight()
{

    setMotorRightPower(0, 0);
    if (_motor_right_.connected()) {
        _motor_right_.stop();
    } else
        logger().error() << "NOT CONNECTED! NO stopMotorRight" << logs::end;
}

void AsservDriver::resetEncoders()
{
    resetInternalEncoders();
    resetExternalEncoders();
}

void AsservDriver::resetInternalEncoders()
{
    if (_motor_left_.connected()) {
        _motor_left_.set_position(0);
    }
    if (_motor_right_.connected()) {
        _motor_right_.set_position(0);
    }
}
void AsservDriver::resetExternalEncoders()
{
    if (angleR_.connected()) {
        angleR_.reset();
        logger().error() << "RESET angleR_ !" << logs::end;
    } else
        logger().error() << "NOT CONNECTED! NO RESET angleR_ !" << logs::end;
}

int AsservDriver::getMotorLeftCurrent()
{
    return 0;
}
int AsservDriver::getMotorRightCurrent()
{
    return 0;
}

void AsservDriver::odo_SetPosition(double x_m, double y_m, double angle_rad)
{
}
RobotPosition AsservDriver::odo_GetPosition()
{

    return {0,0,0,0};
}
int AsservDriver::path_GetLastCommandStatus()
{
    return 0;
}
void AsservDriver::path_InterruptTrajectory()
{
}
void AsservDriver::path_CollisionOnTrajectory()
{
}
void AsservDriver::path_CollisionRearOnTrajectory()
{
}
void AsservDriver::path_CancelTrajectory()
{
}
void AsservDriver::path_ResetEmergencyStop()
{
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_m, float y_m)
{
    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters)
{

    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{
    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
    return TRAJ_ERROR;
}
void AsservDriver::motion_FreeMotion()
{
}
void AsservDriver::motion_DisablePID()
{
}
void AsservDriver::motion_AssistedHandling()
{
}
void AsservDriver::motion_ActivateManager(bool enable)
{
}

void AsservDriver::motion_setLowSpeed(bool enable)
{

}

void AsservDriver::motion_ActivateReguDist(bool enable)
{

}
void AsservDriver::motion_ActivateReguAngle(bool enable)
{

}
void AsservDriver::motion_ResetReguDist()
{

}
void AsservDriver::motion_ResetReguAngle()
{

}
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_meters)
{
    return TRAJ_ERROR;
}
