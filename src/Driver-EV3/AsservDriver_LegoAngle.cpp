//drivers...EV3

#include "AsservDriver_LegoAngle.hpp"

#include <unistd.h>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

//AAsservDriver* AAsservDriver::create(std::string)
//{
//    static AsservDriver *instance = new AsservDriver();
//    return instance;
//}

void AsservDriver_LegoAngle::reset()
{
    if (_motor_left_.connected())
        _motor_left_.reset();

    if (_motor_right_.connected())
        _motor_right_.reset();
}

AsservDriver_LegoAngle::AsservDriver_LegoAngle() :
        _motor_right_(OUTPUT_D), _motor_left_(OUTPUT_B), angleR_("ev3-ports:in2:i2c1"), angleL_("ev3-ports:in1:i2c1")
{
    logger().debug() << "AsservDriver()" << logs::end;

    if (angleR_.connected()) {
        logger().debug() << "[" << angleR_.address() << "] (" << angleR_.driver_name() << ") type_name="
                << angleR_.type_name() << logs::end;
    } else {
        logger().error() << "NOT CONNECTED! NO angleR_ !" << logs::end;
    }
    if (angleL_.connected()) {
        logger().debug() << "[" << angleL_.address() << "] (" << angleL_.driver_name() << ") type_name="
                << angleL_.type_name() << logs::end;
    } else {
        logger().error() << "NOT CONNECTED! NO angleL_ !" << logs::end;
    }

//    if (_motor_right_.connected()) //if both motors are connected, then initialize each motor.
//    {
//        //_motor_right_.
//        _motor_right_.reset();
//
//        _motor_right_.set_polarity(motor::polarity_inversed);
//
//        _motor_right_.set_ramp_down_sp(0);
//        _motor_right_.set_ramp_up_sp(0);
//        _motor_right_.set_stop_action("brake");
//    }

    if (_motor_right_.connected()) {

        logger().debug() << "(" << "RIGHT" << ") " << _motor_right_.driver_name() << " motor on port "
                << _motor_right_.address() << " Pol=" << _motor_right_.polarity() << logs::end;
    } else {
        logger().error() << "NOT CONNECTED! NO _motor_right_ !" << logs::end;
    }

//    if (_motor_left_.connected()) //if both motors are connected, then initialize each motor.
//    {
//        _motor_left_.reset();
//        _motor_left_.set_polarity(motor::polarity_inversed);
//        _motor_left_.set_ramp_down_sp(0);
//        _motor_left_.set_ramp_up_sp(0);
//        _motor_left_.set_stop_action("brake");
//
//    }

    if (_motor_left_.connected()) {

        logger().debug() << "(" << "LEFT " << ") " << _motor_left_.driver_name() << " motor on port "
                << _motor_left_.address() << " Pol=" << _motor_left_.polarity() << logs::end;
    } else {
        logger().error() << "NOT CONNECTED! NO _motor_left_ !" << logs::end;
    }

    resetEncoders();

}

void AsservDriver_LegoAngle::setMotorLeftPosition(int ticks_per_second, long ticks)
{
    limit(ticks_per_second, MAXVALUE_speed_sp);
    if (_motor_left_.connected()) {

        //ticks :the units are in tachometer pulse counts, so if you are not using a LEGO motor, then you will need to do some math to convert to/from degrees.
        _motor_left_.set_position_sp(ticks).set_speed_sp(ticks_per_second).run_to_rel_pos();
    }
}

void AsservDriver_LegoAngle::setMotorRightPosition(int ticks_per_second, long ticks)
{
    limit(ticks_per_second, MAXVALUE_speed_sp);
    if (_motor_right_.connected()) {
        //the units are in tachometer pulse counts, so if you are not using a LEGO motor, then you will need to do some math to convert to/from degrees.
        _motor_right_.set_position_sp(ticks).set_speed_sp(ticks_per_second).run_to_rel_pos();
    }
}

int AsservDriver_LegoAngle::limit(int power, int max)
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
void AsservDriver_LegoAngle::setMotorLeftPower(int value, int timems)
{
    if (_motor_left_.connected()) {
        if (timems == 0) {
            limit(value, MAXVALUE_speed_sp);
            _motor_left_.set_speed_sp(value).run_forever();
        } else if (timems > 0) {
            limit(value, MAXVALUE_speed_sp);
            //logger().debug() << "LEFT  value = " << value << logs::end;
            _motor_left_.set_speed_sp(value).set_time_sp(timems).run_timed();
        } else {
            limit(value, MAXVALUE_duty_cycle_sp);
            //logger().debug() << "LEFT  percent = " << value << logs::end;
            //_motor_left_.set_duty_cycle_sp(value).run_direct();
            _motor_left_.custom_set_duty_cycle_sp_run_direct(value);
        }
    }
}

void AsservDriver_LegoAngle::setMotorRightPower(int value, int timems)
{
    if (_motor_right_.connected()) {
        if (timems == 0) {
            limit(value, MAXVALUE_speed_sp);
            _motor_right_.set_speed_sp(value).run_forever();
        } else if (timems > 0) {
            limit(value, MAXVALUE_speed_sp);
            //logger().debug() << "RIGHT value = " << value << logs::end;
            _motor_right_.set_speed_sp(value).set_time_sp(timems).run_timed();
        } else {
            limit(value, MAXVALUE_duty_cycle_sp);
            //logger().error() << "RIGHT percent = " << value << logs::end;
            //_motor_right_.set_duty_cycle_sp(value).run_direct();
            _motor_right_.custom_set_duty_cycle_sp_run_direct(value);
        }
    }
}

long AsservDriver_LegoAngle::getLeftExternalEncoder()
{
    if (angleL_.connected()) {
        long ticks = -1 * angleL_.getValueDegrees();

        return ticks;
    } else
        return -999999;
}
long AsservDriver_LegoAngle::getRightExternalEncoder()
{
    if (angleR_.connected()) {
        long ticks = angleR_.getValueDegrees();

        return ticks;
    } else
        return -999999;
}

long AsservDriver_LegoAngle::getLeftInternalEncoder()
{
    if (_motor_left_.connected()) {
        //+/- 2,147,483,648
        return _motor_left_.custom_get_position();
    } else
        return 0;

}
long AsservDriver_LegoAngle::getRightInternalEncoder()
{
    if (_motor_right_.connected()) {

        return _motor_right_.custom_get_position();

    } else
        return 0;
}

void AsservDriver_LegoAngle::stopMotorLeft()
{
    setMotorLeftPower(0, 0);
    if (_motor_left_.connected()) {
        //_motor_left_.stop();
        _motor_left_.custom_set_command("stop");
    } else
        logger().error() << "NOT CONNECTED! NO stopMotorLeft" << logs::end;

}
void AsservDriver_LegoAngle::stopMotorRight()
{

    setMotorRightPower(0, 0);
    if (_motor_right_.connected()) {
        //_motor_right_.stop();
        _motor_right_.custom_set_command("stop");
    } else
        logger().error() << "NOT CONNECTED! NO stopMotorRight" << logs::end;
}

void AsservDriver_LegoAngle::resetEncoders()
{
    resetInternalEncoders();
    resetExternalEncoders();
}

void AsservDriver_LegoAngle::resetInternalEncoders()
{
    if (_motor_left_.connected()) {
        _motor_left_.set_position(0);
    }
    if (_motor_right_.connected()) {
        _motor_right_.set_position(0);
    }
}
void AsservDriver_LegoAngle::resetExternalEncoders()
{
    if (angleR_.connected()) {
        angleR_.reset();
    }

    if (angleL_.connected()) {
        angleL_.reset();
    }
}

int AsservDriver_LegoAngle::getMotorLeftCurrent()
{
    return 0;
}
int AsservDriver_LegoAngle::getMotorRightCurrent()
{
    return 0;
}

void AsservDriver_LegoAngle::odo_SetPosition(float x_m, float y_m, float angle_rad)
{
}
ROBOTPOSITION AsservDriver_LegoAngle::odo_GetPosition()
{

    ROBOTPOSITION p = {0.0, 0.0, 0.0 ,0 , 0};
        return p;
}
int AsservDriver_LegoAngle::path_GetLastCommandStatus()
{
    return 0;
}
void AsservDriver_LegoAngle::path_InterruptTrajectory()
{
}
void AsservDriver_LegoAngle::path_CollisionOnTrajectory()
{
}
void AsservDriver_LegoAngle::path_CollisionRearOnTrajectory()
{
}
void AsservDriver_LegoAngle::path_CancelTrajectory()
{
}
void AsservDriver_LegoAngle::path_ResetEmergencyStop()
{
}

TRAJ_STATE AsservDriver_LegoAngle::motion_DoFace(float x_m, float y_m)
{
    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver_LegoAngle::motion_DoLine(float dist_meters)
{

    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver_LegoAngle::motion_DoRotate(float angle_radians)
{
    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver_LegoAngle::motion_DoArcRotate(float angle_radians, float radius)
{
    return TRAJ_ERROR;
}
void AsservDriver_LegoAngle::motion_FreeMotion()
{
}
void AsservDriver_LegoAngle::motion_DisablePID()
{
}
void AsservDriver_LegoAngle::motion_AssistedHandling()
{
}
void AsservDriver_LegoAngle::motion_ActivateManager(bool enable)
{
}

void AsservDriver_LegoAngle::motion_setLowSpeedForward(bool enable, int)
{

}

void AsservDriver_LegoAngle::motion_setLowSpeedBackward(bool enable, int)
{

}

void AsservDriver_LegoAngle::motion_ActivateReguDist(bool enable)
{

}
void AsservDriver_LegoAngle::motion_ActivateReguAngle(bool enable)
{

}
void AsservDriver_LegoAngle::motion_ResetReguDist()
{

}
void AsservDriver_LegoAngle::motion_ResetReguAngle()
{

}
TRAJ_STATE AsservDriver_LegoAngle::motion_DoDirectLine(float dist_meters)
{
    return TRAJ_ERROR;
}
