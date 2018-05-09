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
    if (_motor_left.connected())
        _motor_left.reset();

    if (_motor_right.connected())
        _motor_right.reset();
}

AsservDriver::AsservDriver()
{
    logger().debug() << "AsservDriver()" << logs::end;

    lcd l;
    logger().debug() << "Resolution is " << l.resolution_x() << " x " << l.resolution_y() << ", " << l.bits_per_pixel()
            << " bit(s) per pixel" << "Frame buffer size is " << l.frame_buffer_size() << " byte, " << "line length is "
            << l.line_length() << " byte" << logs::end;

    l.fill(0xFF);


    sensor angle(INPUT_AUTO, { "ht-nxt-angle" });

    logger().debug() << "angle connected = " << angle.connected() << logs::end;

    if (angle.connected()) {

        angle.set_mode("ANGLE-ACC");
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



    }
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
    _motor_left_ = large_motor(OUTPUT_A);
    if (_motor_left_.connected()) {

        logger().info() << "(" << "A" << ") " << _motor_left_.driver_name() << " motor on port "
                << _motor_left_.address() << logs::end;
    }

    _motor_right_ = large_motor(OUTPUT_D);
    if (_motor_right_.connected()) {

        logger().info() << "(" << "D" << ") " << _motor_right_.driver_name() << " motor on port "
                << _motor_right_.address() << logs::end;
    }
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

void AsservDriver::setMotorLeftPosition(int power, long ticks)
{
    power = -power; //TODO add a parameter to reverse motor inverted or not using polarity function ?
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

void AsservDriver::setMotorRightPosition(int power, long ticks)
{
    power = -power;
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
void AsservDriver::setMotorLeftPower(int power, int timems)
{
    power = -power;
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

void AsservDriver::setMotorRightPower(int power, int timems)
{
    power = -power;
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
    return 0;
}
long AsservDriver::getRightExternalEncoder()
{
    return 0;
}

long AsservDriver::getLeftInternalEncoder()
{
    if (_motor_left.connected()) {
        //+/- 2,147,483,648
        return -_motor_left.position();
    } else
        return 0;

}
long AsservDriver::getRightInternalEncoder()
{
    if (_motor_right.connected()) {
        return -_motor_right.position();
    } else
        return 0;
}

void AsservDriver::stopMotorLeft()
{
    setMotorLeftPower(0, 0);
    if (_motor_left.connected()) {
        _motor_left.stop();
    }
    logger().debug() << "stopMotorLeft" << logs::end;

}
void AsservDriver::stopMotorRight()
{

    setMotorRightPower(0, 0);
    if (_motor_right.connected()) {
        _motor_right.stop();
    }
    logger().debug() << "stopMotorRight" << logs::end;
}

void AsservDriver::resetEncoders()
{
    if (_motor_left.connected()) {
        _motor_left.set_position(0);
    }
    if (_motor_right.connected()) {
        _motor_right.set_position(0);
    }
}

void AsservDriver::resetInternalEncoders()
{
    //TODO
}
void AsservDriver::resetExternalEncoders()
{
    //TODO
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
