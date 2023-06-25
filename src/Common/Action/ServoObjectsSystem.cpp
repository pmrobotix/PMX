#include "ServoObjectsSystem.hpp"

#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "Actions.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(std::string botId, Actions &actions) :
        AActionsElement(actions), botId_(botId)
{
    servodriver_ = AServoDriver::create();
    move_finished_ = false;
}

ServoObjectsSystem::~ServoObjectsSystem()
{
    delete servodriver_;
}

bool ServoObjectsSystem::is_connected()
{
    return servodriver_->is_connected();
}

bool ServoObjectsSystem::setup(int servo, AServoDriver::ServoType type, int valueMinPulse, int valueMidPulse,
        int valueMaxPulse, bool inversed)
{

    if (!servodriver_->is_connected()) {
        logger().error() << "SERVO NOT CONNECTED ! servo=" << servo << logs::end;
        return false;
    }

    if (type == AServoDriver::SERVO_DYNAMIXEL) {
        int p = servodriver_->ping(servo);
        //if (p < 0) logger().debug() << "servo=" << servo << " ERROR PING" << logs::end;
        if (p <= 0) {
            logger().error() << "servo=" << servo << " NO PING" << logs::end;
            return false;
        }
    }

    servodriver_->release(servo);

    if (type == AServoDriver::ServoType::SERVO_STANDARD) {

        servodriver_->setType(servo, type);
        servodriver_->setPolarity(servo, inversed);
        servodriver_->setMinPulse(servo, valueMinPulse);
        servodriver_->setMidPulse(servo, valueMidPulse);
        servodriver_->setMaxPulse(servo, valueMaxPulse);

    }
    return true;
}

//keep_millisec = -1 : wait moving until position
//keep_millisec > 0 : time to wait millisec
//keep_millisec = 0 : continue and hold
void ServoObjectsSystem::deployWithVelocity(int servo, int pos, int keep_millisec, int velocity)
{
    hold(servo);
    servodriver_->setPulsePos(servo, pos, velocity); // percentage ms0to90°
    if (keep_millisec > 0) {
        utils::sleep_for_micros(keep_millisec * 1000); //TODO verifier la torque
        release(servo);
    } else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
            //            if (r<0)
            //            {
            //                logger().info() << "servo=" << servo << " deploy getMoving break"  << logs::end;
            //                break;
            //            }
            r = servodriver_->getMoving(servo);

            //            if (r <= 0 || r >=200) {
            //                r = servodriver_->getMoving(servo);
            //            }
            //            if (r <= 0 || r >=200) {
            //                            r = servodriver_->getMoving(servo);
            //                        }
            //            if (r == 0) {
            //                int torque = servodriver_->getTorque(servo);
            //                if (torque > 600)
            //                {
            //                    logger().info() << "servo=" << servo << " percent= "<< percent << " TOO MUCH TORQUE torque= "<< torque << logs::end;
            //                    release(servo);
            //                    break; //TODO try a 2nde time
            //                }
            //            }

            //logger().info() << "wait "<< r << logs::end;
            utils::sleep_for_micros(2000);
            std::this_thread::yield();
        }
    }
    //int torque = getTorque(servo);
    logger().info() << "deployWithVelocity servo=" << servo << " pos=" << pos << logs::end;
}

//keep_millisec = -1 : wait moving until position
//keep_millisec > 0 : time ms to wait then release
//keep_millisec = 0 : continue and hold
void ServoObjectsSystem::deploy(int servo, int pos, int keep_millisec)
{
    hold(servo);
    servodriver_->setPulsePos(servo, pos); // percentage
    if (keep_millisec > 0) {
        utils::sleep_for_micros(keep_millisec * 1000); //TODO verifier la torque
        release(servo);
    } else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
//            if (r<0)
//            {
//                logger().info() << "servo=" << servo << " deploy getMoving break"  << logs::end;
//                break;
//            }
            r = servodriver_->getMoving(servo);

//            if (r <= 0 || r >=200) {
//                r = servodriver_->getMoving(servo);
//            }
//            if (r <= 0 || r >=200) {
//                            r = servodriver_->getMoving(servo);
//                        }
//            if (r == 0) {
//                int torque = servodriver_->getTorque(servo);
//                if (torque > 600)
//                {
//                    logger().info() << "servo=" << servo << " percent= "<< percent << " TOO MUCH TORQUE torque= "<< torque << logs::end;
//                    release(servo);
//                    break; //TODO try a 2nde time
//                }
//            }

            //logger().info() << "wait "<< r << logs::end;
            utils::sleep_for_micros(2000);
            std::this_thread::yield();
        }
    }
    //int torque = getTorque(servo);
    logger().debug() << "servo=" << servo << " pos= " << pos << logs::end;
}

void ServoObjectsSystem::setTorque(int servo, int torque)
{
    servodriver_->setRate(servo, torque); //torque
}

int ServoObjectsSystem::getTorque(int servo)
{
    return servodriver_->getTorque(servo);
}

//Pour oposul , milli0to90 est la vitesse de 0 à 1023
int ServoObjectsSystem::setPos(int servo, int pos, int milli0to90)
{

    hold(servo);
    servodriver_->setPulsePos(servo, pos, milli0to90);
    return 0; //TODO return utile ?
}

int ServoObjectsSystem::getPulseWidth(int servo)
{
    //Pour les servos STD, permet de simplement lire la valeur de goal
    return servodriver_->getPulsePos(servo);
}

void ServoObjectsSystem::release(int servo)
{
    servodriver_->release(servo);
}

void ServoObjectsSystem::hold(int servo)
{
    servodriver_->hold(servo);
}
int ServoObjectsSystem::getSpeed(int servo)
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    return -1;
}
void ServoObjectsSystem::setSpeed(int servo, int speed)
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    //servodriver_->setRate(servo, speed); //torque //TODO le vrai setspeed
}

void ServoObjectsSystem::turn(int servo, int speed_percent, int keep_millisec)
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
}

void ServoObjectsSystem::detectAll()
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    for (int i = 0; i < 255; i++) {
        if (int err = servodriver_->ping(i) == 0) {
            logger().info() << i << " err=" << err << logs::end;
        }
    }
}

void ServoObjectsSystem::detect()
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
//    if (servodriver_->ping(51) != 0) {
//        logger().error() << "ERROR AX12 CONNECTION !!!" << logs::end;
//    }
}

ServoObjectsTimer::ServoObjectsTimer(ServoObjectsSystem &sOsS, int number_servos, uint timeSpan_us, int eta_ms,
        int servo1, int cur_pos1, int goal_pos1, int velocity1) :
        servoObjectsSystem_(sOsS), name_(number_servos), eta_ms_(eta_ms), servo1_(servo1), cur_pos1_(cur_pos1), goal_pos1_(
                goal_pos1), velocity1_(velocity1)
{
    std::string name = std::to_string(number_servos);
    //init du timer POSIX associé ITimerPosixListener ou ITimerListener
    this->init(name, timeSpan_us);

    first_exe_ = true;
    move_starttime_ms_ = 0;

    servo2_ = 0;
    cur_pos2_ = 0;
    goal_pos2_ = 0;
    velocity2_ = 0;
}

ServoObjectsTimer::ServoObjectsTimer(ServoObjectsSystem &sOsS, int number_servos, uint timeSpan_us, int eta_ms,
        int servo1, int cur_pos1, int goal_pos1, int velocity1, int servo2, int cur_pos2, int goal_pos2, int velocity2) :
        servoObjectsSystem_(sOsS), name_(number_servos), eta_ms_(eta_ms), servo1_(servo1), cur_pos1_(cur_pos1), goal_pos1_(
                goal_pos1), velocity1_(velocity1), servo2_(servo2), cur_pos2_(cur_pos2), goal_pos2_(goal_pos2), velocity2_(
                velocity2)
{
    std::string name = std::to_string(number_servos);
    //init du timer POSIX associé ITimerPosixListener ou ITimerListener
    this->init(name, timeSpan_us);

    first_exe_ = true;
    move_starttime_ms_ = 0;
}

void ServoObjectsSystem::stopTimers()
{
    this->actions().stopPTimer("1");
    this->actions().stopPTimer("2");
}

//void ServoObjectsSystem::deployByTimerTask(int servo, int percent, int keep_millisec)
//{
// //add the timer to move and wait until arrive at position.
//
//}
void ServoObjectsSystem::move_1_servo(bool waitornot, int time_eta_ms, int servo1, int pos1, int torque1,
        int keep_torque_extratimems, int escape_torque)
{
    move_finished_ = false;

    logger().debug() << "move_1_servo create start" << logs::end;
    hold(servo1);
    //mouvement a faire en time_eta_ms
    //conversion velocity pour 0to90° //correspond à 1000pulses_pwm environ sur un servo std
    //           time_eta_ms            diff en pulses_pwm
    int cur_pos1 = servodriver_->getPulsePos(servo1);

    logger().debug() << "cur_pos1=" << cur_pos1 << logs::end;
    int diff1 = std::abs(pos1 - cur_pos1);

    int velocity_ms_0to90_1 = time_eta_ms * 1000.0 / diff1;

    logger().debug() << "move_1_servo create timer" << logs::end;
    this->actions().addTimer(
            new ServoObjectsTimer(*this, 1, TIMER_SERVO_PERIOD_US, time_eta_ms, servo1, cur_pos1, pos1,
                    velocity_ms_0to90_1));

    //wait or not wait ?
    if (waitornot) {
        logger().debug() << "wait for end of servo move...NO ESCAPE" << logs::end;
        while (!move_finished_) {
            utils::sleep_for_micros(TIMER_SERVO_PERIOD_US);
            std::this_thread::yield();
        }
    }
}

// lego pos: percentage velocity:ms0to90°
//keep_torque_ms
void ServoObjectsSystem::move_2_servos(bool waitornot, int time_eta_ms, int servo1, int pos1, int torque1, int servo2,
        int pos2, int torque2, int keep_torque_extratimems, int escape_torque)
{
    //WARNING SI actionmanager is not started


    move_finished_ = false;

    logger().debug() << "move_2_servos create start" << logs::end;
    hold(servo1);
    hold(servo2);
    //mouvement a faire en time_eta_ms
    //conversion velocity pour 0to90° //correspond à 1000pulses_pwm environ sur un servo std
    //           time_eta_ms            diff en pulses_pwm
    int cur_pos1 = servodriver_->getPulsePos(servo1);
    int cur_pos2 = servodriver_->getPulsePos(servo2);

    logger().debug() << "cur_pos1=" << cur_pos1 << " cur_pos2=" << cur_pos2 << logs::end;
    int diff1 = std::abs(pos1 - cur_pos1);
    int diff2 = std::abs(pos2 - cur_pos2);
    int velocity_ms_0to90_1 = time_eta_ms * 1000.0 / diff1;
    int velocity_ms_0to90_2 = time_eta_ms * 1000.0 / diff2;

    logger().debug() << "velocity_ms_0to90_1=" << velocity_ms_0to90_1 << " velocity_ms_0to90_2=" << velocity_ms_0to90_2
            << logs::end;

    logger().debug() << "move_2_servos create timer" << logs::end;
    this->actions().addTimer(
            new ServoObjectsTimer(*this, 2, TIMER_SERVO_PERIOD_US, time_eta_ms, servo1, cur_pos1, pos1,
                    velocity_ms_0to90_1, servo2, cur_pos2, pos2, velocity_ms_0to90_2));

    //wait or not wait ?
    if (waitornot) {
        logger().debug() << "wait for end of servo move...NO ESCAPE" << logs::end;
        while (!move_finished_) {
            utils::sleep_for_micros(TIMER_SERVO_PERIOD_US);
            std::this_thread::yield();
        }
    }
}

void ServoObjectsTimer::onTimer(utils::Chronometer chrono)
{
    //logger().debug() << "onTimer start chrono_ms=" << chrono.getElapsedTimeInMilliSec() << logs::end;
    if (first_exe_) {
        move_starttime_ms_ = chrono.getElapsedTimeInMilliSec(); //depuis le début du programme
        first_exe_ = false;
    }
    long tms = chrono.getElapsedTimeInMilliSec() - move_starttime_ms_;
    servoObjectsSystem_.move_finished(false);

    float new_cur_pos_index1 = 0;
    int pos2apply1 = 0;

    float new_cur_pos_index2 = 0;
    int pos2apply2 = 0;

    if (name_ >= 1) {
        if (tms < eta_ms_) {
            //calcul de la position pour t qui varie de 0 à eta_ms
            new_cur_pos_index1 = tms * 1000.0 / velocity1_;
            if (cur_pos1_ <= goal_pos1_) {
                pos2apply1 = cur_pos1_ + (int)new_cur_pos_index1;
            } else {
                pos2apply1 = cur_pos1_ - (int)new_cur_pos_index1;
            }
        } else {
            pos2apply1 = goal_pos1_;
        }

        logger().debug() << "t_ms= " << tms << " cur_pos_= " << cur_pos1_ << " goal_pos_= " << goal_pos1_
                << " velocity_= " << velocity1_ << " new_cur_pos= " << (int)new_cur_pos_index1 << " pos2apply= "
                << pos2apply1 << " eta_ms_= " << eta_ms_ << logs::end;

        servoObjectsSystem_.servodriver()->setPulsePos(servo1_, pos2apply1, 0);
    }

    if (name_ >= 2) {

        if (tms < eta_ms_) {
            //calcul de la position pour t qui varie de 0 à eta_ms
            new_cur_pos_index2 = tms * 1000.0 / velocity2_;
            if (cur_pos2_ <= goal_pos2_) {
                pos2apply2 = cur_pos2_ + (int)new_cur_pos_index2;
            } else {
                pos2apply2 = cur_pos2_ - (int)new_cur_pos_index2;
            }
        } else {
            pos2apply2 = goal_pos2_;
        }

        logger().debug() << "t_ms= " << tms << " cur_pos_= " << cur_pos2_ << " goal_pos_= " << goal_pos2_
                << " velocity2_= " << velocity2_ << " new_cur_pos2= " << (int)new_cur_pos_index2 << " pos2apply2= "
                << pos2apply2 << " eta_ms_= " << eta_ms_ << logs::end;

        servoObjectsSystem_.servodriver()->setPulsePos(servo2_, pos2apply2, 0);

    }
    if (tms >= eta_ms_) {

        logger().debug() << "requestToStop_ = true; t=" << chrono.getElapsedTimeInMilliSec() << logs::end;
        requestToStop_ = true;
        first_exe_ = true;
    }
}

void ServoObjectsTimer::onTimerEnd(utils::Chronometer chrono)
{
    logger().debug() << "onTimerEnd move_finished t=" << chrono.getElapsedTimeInMilliSec() << logs::end;
    servoObjectsSystem_.move_finished(true);
}

std::string ServoObjectsTimer::info()
{
    std::ostringstream oss;
    oss << "ServoObjectsTimer [" << name() << "] for " << servoObjectsSystem_.id();
    return oss.str();
}

