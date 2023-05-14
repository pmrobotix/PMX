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
        logger().error() << "SERVO NOT CONNECTED !" << logs::end;
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
        }
    }
    //int torque = getTorque(servo);
    logger().info() << "servo=" << servo << " pos= " << pos << logs::end;
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

ServoObjectsTimer::ServoObjectsTimer(ServoObjectsSystem & sOsS, int number_servos, uint timeSpan_us, int eta_ms, int servo1, int cur_pos1, int goal_pos1, int velocity) :
        servoObjectsSystem_(sOsS), name_(number_servos), eta_ms_(eta_ms), servo_(servo1), cur_pos_(cur_pos1), goal_pos_(goal_pos1), velocity_(velocity)
{
    std::string name = std::to_string(number_servos);
    //init du timer POSIX associé ITimerPosixListener ou ITimerListener
    this->init(name, timeSpan_us);

}

//void ServoObjectsSystem::deployByTimerTask(int servo, int percent, int keep_millisec)
//{
// //add the timer to move and wait until arrive at position.
//
//}
void ServoObjectsSystem::move_1_servo(int servo1, int pos1, int torque1, int time_eta_ms, bool keep_torque,
        int escape_torque)
{

}

// lego pos: percentage velocity:ms0to90°
//keep_torque_ms
void ServoObjectsSystem::move_2_servos(bool waitornot, int time_eta_ms, int servo1, int pos1, int torque1, int servo2, int pos2, int torque2, int keep_torque_extratimems, int escape_torque)
{
    move_finished_ = false;

    logger().debug() << "move_2_servos create start"  << logs::end;
    hold(servo1);
    hold(servo2);
    //mouvement a faire en time_eta_ms
    //conversion velocity pour 0to90° //correspond à 1000pulses_pwm environ sur un servo std
    //           time_eta_ms            diff en pulses_pwm
    int cur_pos1 = servodriver_->getPulsePos(servo1);
    int cur_pos2 = servodriver_->getPulsePos(servo2);

    logger().debug() << "cur_pos1=" << cur_pos1 << " cur_pos2=" << cur_pos2  << logs::end;
    int diff1 = std::abs(pos1 - cur_pos1);
    int diff2 = std::abs(pos2 - cur_pos2);
    int velocity_ms_0to90_1 = time_eta_ms * 1000.0 / diff1;
    int velocity_ms_0to90_2 = time_eta_ms * 1000.0 / diff2;

    logger().debug() << "velocity_ms_0to90_1=" << velocity_ms_0to90_1 << " velocity_ms_0to90_2=" << velocity_ms_0to90_2  << logs::end;


    logger().debug() << "move_2_servos create timer"  << logs::end;
    this->actions().addTimer(new ServoObjectsTimer(*this, 1, 10000, time_eta_ms, servo1, cur_pos1, pos1 , velocity_ms_0to90_1));

    //wait or not wait ?
    if (waitornot)
    {
        logger().debug() << "wait for end of servo move..."  << logs::end;
        while (!move_finished_)
        {
            utils::sleep_for_micros(10000);
        }
    }

}

void ServoObjectsTimer::onTimer(utils::Chronometer chrono)
{
    //logger().debug() << "onTimer start chrono_ms=" << chrono.getElapsedTimeInMilliSec() << logs::end;
    servoObjectsSystem_.move_finished(false);
    long t = chrono.getElapsedTimeInMilliSec();
    int new_cur_pos_index = 0 ;
    int pos2apply = 0;
    if (t <= eta_ms_)
    {
        switch (name_) {
        case 1:

            //calcul de la position pour t qui varie de 0 à eta_ms
            new_cur_pos_index = t * 1000.0 / velocity_;

            if (cur_pos_ <= goal_pos_) {
                pos2apply = cur_pos_ + new_cur_pos_index;
            } else {
                pos2apply = cur_pos_ - new_cur_pos_index;
            }
            logger().debug() << "t_ms= " << t
                    << " cur_pos_= " << cur_pos_
                    << " goal_pos_= " << goal_pos_
                    << " velocity_= " << velocity_
                    << " new_cur_pos= " << new_cur_pos_index
                    << " pos2apply= " << pos2apply
                    << " eta_ms_= " << eta_ms_
                    << logs::end;

            servoObjectsSystem_.servodriver()->setPulsePos(servo_, pos2apply);
            break;

        default:
            break;
        }

    }else
    {
        logger().debug() << "requestToStop_ = true; t=" << chrono.getElapsedTimeInMilliSec() << logs::end;
        requestToStop_ = true;
    }
}

void ServoObjectsTimer::onTimerEnd(utils::Chronometer chrono)
{
    servoObjectsSystem_.move_finished(true);
}

std::string ServoObjectsTimer::info()
{
    std::ostringstream oss;
    oss << "ServoObjectsTimer [" << name() << "] for " << servoObjectsSystem_.id();
    return oss.str();
}

