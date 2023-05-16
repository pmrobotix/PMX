/*
 *
 l * CONFIG_MOTORCTRL_BOTMOTORS need to be initialised in Eclipse param build
 */

#include "AsservEsialR.hpp"

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>

#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "codeurs/BotCodeurs.h"
#include "commandManager/CommandManagerA.h"
#include "config/config.h"
#include "consignController/ConsignController.h"
#include "motorsController/BotMotors.h"
#include "odometrie/Odometrie.h"
#include "Utils/Utils.h"

class Robot;

AsservEsialR::AsservEsialR(Robot *robot) :
        chronoTimer_("AsservEsialR")
{
    robot_ = robot; //Reference vers le robot
    loop_finished_ = false;
    asservdriver = AAsservDriver::create(robot_->getID());

    periodNb_ = 0;
    loopDelayInMillisec_ = 0;
    pathStatus_ = TRAJ_OK;

    p_ = { 0, 0, 0, 0 };

    codeurs_ = NULL;
    odo_ = NULL;
    motorC_ = NULL;
    consignC_ = NULL;
    commandM_ = NULL;
    run_ = false;

    last_ = 0;

    //première ligne du fichier csv
    loggerFile().debug() << "nb " << ", usec" << ", time-last" << ", work_time" << ", odo_getDeltaDist"
            << ", getVitesseG" << ", getVitesseD" << ", xmm " << ", ymm " << ", degrees" << logs::end;

}

void AsservEsialR::startAsserv(int freqHz)
{
    logger().debug() << "startAsserv..." << logs::end;
    setSamplingFrequency(freqHz); //Hz

    //create motion control task thread execute function
    start("AsservEsialR", 2);
}
void AsservEsialR::initAsserv()
{
    run_ = false;

#if CONFIG_MOTORCTRL_BOTMOTORS
    if (codeurs_ == NULL)
        codeurs_ = new BotCodeurs(robot_);
    if (motorC_ == NULL)
        motorC_ = new BotMotors(robot_); //CONFIG_MOTORCTRL_BOTMOTORS à activer dans eclipse pour la config de l'asserv
    if (odo_ == NULL)
        odo_ = new Odometrie(codeurs_);
    if (consignC_ == NULL)
        consignC_ = new ConsignController(odo_, motorC_);
    if (commandM_ == NULL)
        commandM_ = new CommandManagerA(50, consignC_, odo_);
    logger().debug() << "objects created..." << logs::end;
    run_ = true;
#endif

}

void AsservEsialR::endWhatTodo()
{
    stopAsserv();
}

void AsservEsialR::stopAsserv()
{
    //On arrête le traitement de l'asserv
    run_ = false; //arret de la boucle d'asserv afin de pouvoir supprimer les objets

    utils::sleep_for_micros(10000);

    // On détruit tout les objets
    delete odo_;
    odo_ = NULL;

    delete consignC_;
    consignC_ = NULL;

    delete commandM_;
    commandM_ = NULL;

    delete codeurs_;
    codeurs_ = NULL;

    delete motorC_;
    motorC_ = NULL;

}

void AsservEsialR::resetAsserv()
{
    //printf("Réinitialisation de l'asserv...\r\n");
    logger().debug() << "Réinitialisation de l'asserv..." << logs::end;
    stopAsserv();
    //On reprend l'asserv
    initAsserv();
}
/*
 void AsservEsialR::onTimer(utils::Chronometer chrono) {
 logs::Logger::LoggerBuffer info = logger().info();
 //info << "onTimer... every " << loopDelayInMillisec_ << " " << chrono.getElapsedTimeInMicroSec() << logs::flush;

 RobotPosition p;
 unsigned long long current = 0;

 if (run_) {
 current = chrono.getElapsedTimeInMicroSec();
 periodNb_++;
 //info << "loop_not_finished_=" << loop_not_finished_ << " " << logs::flush;

 //        if (loop_not_finished_) {
 //            info << "OVERLAPPING !!" << logs::flush;
 //        }else
 //        {
 //            loop_not_finished_ = true;
 //        }

 utils::Thread::sleep_for_micros(20000);
 long end = chrono.getElapsedTimeInMicroSec();
 //        info << periodNb_
 //                <<" last_=" << last_
 //                <<" current=" << current
 //                <<" jobtime=" << (long) current - last_
 //                <<" tasktime=" << end - current
 //                << logs::flush;
 //
 if ((end - current) > (loopDelayInMillisec_ * 1000 )) {
 info << "OVERLAPPING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << logs::flush;

 }
 //loop_not_finished_ = false;
 last_ = current;
 }
 }
 */


/*
void AsservEsialR::onTimer(utils::Chronometer chrono)
{
    RobotPosition p;
    unsigned long long current = 0;
    current = chrono.getElapsedTimeInMicroSec();

    logs::Logger::LoggerBuffer info = logger().info();
    logs::Logger::LoggerBuffer debug = logger().debug();

    //info << "AsservEsialR::onTimer... every " << loopDelayInMillisec_ << " " << chrono.getElapsedTimeInMilliSec() <<  logs::flush;

    //logs::Logger::LoggerBuffer debugfile = loggerFile().debug();

    if (run_) {
        periodNb_++;
        if (!loop_finished_) {
            info << "OVERLAPPING !!" << logs::flush;
        }
        loop_finished_ = false;

        odo_->refresh();
        p = odo_GetPosition(); //maj de la position avec l'odo

        if (!Config::disableAsserv) {
            consignC_->perform();
            commandM_->perform();
        }

        //svg log
        //if (nb % 4 == 0) {
//                        info << periodNb_ << " us=" << (long) (current - last_)
//                                << " xmm=" << p.x
//                                << std::setw(10) << " ymm=" << p.y
//                                << std::setw(10) << std::fixed << std::setprecision(3) << " deg="<< p.theta * 180 / M_PI
//                                << std::setw(10) << " s=" << p.asservStatus << logs::flush;

        robot_->svgw().writePosition_BotPos(p.x, p.y, p.theta);


//         //file log for asserv
//         debugfile << periodNb_
//         << ", "
//         << current
//         << ", "
//         << (long) (current - last_)
//         << ", "
//         << (long) (chrono.getElapsedTimeInMicroSec() - current)
//         << ", "
//         << odo_->getDeltaDist()
//         // distance entre 2
//         << ", "
//         << motorC_->getVitesseG()
//         //-100 à 100
//         << ", "
//         << motorC_->getVitesseD()
//         //-100 à 100
//         << ", "
//         << p.x
//         << ", "
//         << p.y
//         << ", "
//         << p.theta * 180.0 / M_PI
//         << logs::flush;

//        unsigned long long t6 = chrono.getElapsedTimeInMicroSec();
        //            if (periodNb_ % 40 == 0) {
//        info << "___p=" << periodNb_ << " (current-last_)=" << (long) (current - last_)
        //                    << " => ODOt2-current=" << t2 - current
        //                               << " => consMt3-t2=" << t3 - t2
        //                               << " => cmdMt4-t3=" << t4 - t3
        //                               << " => svgt5-t4="  << t5 - t4
        //                               << " => excelt6-t5="  << t6 - t5
//                << " => worktime=" << (long) (t6 - current) << logs::flush;
//                    }
        last_ = current;
        loop_finished_ = true;

    } else {
        utils::Thread::sleep_for_micros(loopDelayInMillisec_ * 1000);
    }

}

void AsservEsialR::onTimerEnd(utils::Chronometer chrono)
{
}

void AsservEsialR::execute()
{
    logger().info() << "execute()::execute demarrage du timer => loopDelayInMillisec_=" << loopDelayInMillisec_
            << logs::end;

    init("AsservEsialR", loopDelayInMillisec_ * 1000);

    periodNb_ = 0;

    last_ = chrono.getElapsedTimeInMicroSec();

    startTimer();
    while (1) {
        //utils::Thread::sleep_for_millis(1);
        utils::Thread::yield();
    }
//    while (getRunning()) {
//        logger().debug() << "execute()::getRunning()" << getRunning() << logs::end;
//
//        utils::Thread::sleep_for_millis(2);
//        yield();
//    }

}
*/
//Execute qui fonctionne avec une simple boucle
void AsservEsialR::execute()
{
    logs::Logger::LoggerBuffer info = logger().info();
    logs::Logger::LoggerBuffer debug = logger().debug();
    logs::Logger::LoggerBuffer debugfile = loggerFile().debug();
    debug << "executing... every " << loopDelayInMillisec_ << logs::flush;

    chronoTimer_.setTimer(loopDelayInMillisec_ * 1000);
    RobotPosition p;
    unsigned long long current = 0;
    unsigned long long last = 0;
    long nb = 0;

    while (1) {
        if (run_) {
            current = chronoTimer_.getElapsedTimeInMicroSec();
            nb++;
            odo_->refresh();
            p = odo_GetPosition(); //maj de la position avec l'odo

            //long t2 = chronoTimer_.getElapsedTimeInMicroSec();

            if (!Config::disableAsserv) {
                consignC_->perform();
                commandM_->perform();
            }

            //            long t4 = chronoTimer_.getElapsedTimeInMicroSec();

            //if (nb % 4 == 0) {
            //                debug << nb << " us=" << (long) (current - last_) << " xmm=" << p.x
            //                        << std::setw(10) << " ymm=" << p.y
            //                        << std::setw(10) << std::fixed << std::setprecision(3) << " deg="<< p.theta * 180 / M_PI
            //                        << std::setw(10) << " s=" << p.asservStatus << logs::flush;
            //svg log
            robot_->svgw().writePosition_BotPos(p.x, p.y, p.theta);
            //}
            //long t5 = chronoTimer_.getElapsedTimeInMicroSec();

            //file log for asserv
            debugfile << nb << ", " << current << ", " << (long) (current - last) << ", "
                    << (long) (chronoTimer_.getElapsedTimeInMicroSec() - current) << ", " << odo_->getDeltaDist()
                    // distance entre 2
                    << ", " << motorC_->getVitesseG()
                    //-100 à 100
                    << ", " << motorC_->getVitesseD()
                    //-100 à 100
                    << ", " << p.x << ", " << p.y << ", " << p.theta * 180.0 / M_PI << logs::flush;

            //if (nb % 4 == 0) {
            debug << std::setw(4) << nb << " " << p.asservStatus << " us=" << (int) (current - last) << " t="
                    << std::setw(3) << (int) (chronoTimer_.getElapsedTimeInMicroSec() - current) << " GD="
                    << std::setw(3) << motorC_->getVitesseG() << "," << std::setw(3) << motorC_->getVitesseD() << " x="
                    << std::setw(4) << (int) p.x << " y=" << std::setw(4) << (int) p.y
                    //<< std::fixed
                    << " deg=" << std::setw(6) << std::setprecision(3) << p.theta * 180 / M_PI << logs::flush;
        }
        // }

        if (run_) {
            chronoTimer_.waitTimer(150);
            last = current;
        } else {
            utils::Thread::sleep_for_micros(20000);
        }
    }
}

void AsservEsialR::setSamplingFrequency(uint frequencyHz)
{
    if (frequencyHz == 0) {
        logger().error() << " frequency= " << frequencyHz << logs::end;
        frequencyHz = 1;
    }

    loopDelayInMillisec_ = 1000.0f / (float) frequencyHz;
    logger().debug() << "loopDelayInMillisec_= " << loopDelayInMillisec_ << logs::end;

}

void AsservEsialR::loadConfig()
{
    Config::loadFile("/local/config.txt");
    logger().info() << "Version configuration : " << Config::configVersion << logs::end;
    //printf("Version configuration : %ld\r\n", Config::configVersion);
}

void AsservEsialR::setMotorLeftPosition(int power, long ticks)
{
    logger().error() << "TODO setMotorLeftPosition !!!" << logs::end;
}
void AsservEsialR::setMotorRightPosition(int power, long ticks)
{
    logger().error() << "TODO setMotorRightPosition !!!" << logs::end;
}
void AsservEsialR::setMotorLeftPower(int power, int timems)
{
    logger().error() << "TODO setMotorLeftPower !!!" << logs::end;
}
void AsservEsialR::setMotorRightPower(int power, int timems)
{
    logger().error() << "TODO setMotorRightPower !!!" << logs::end;
}
void AsservEsialR::stopMotorLeft()
{
    logger().error() << "TODO stopMotorLeft !!!" << logs::end;
}
void AsservEsialR::stopMotorRight()
{
    logger().error() << "TODO stopMotorRight !!!" << logs::end;
}
int AsservEsialR::getMotorLeftCurrent()
{
    logger().error() << "TODO getMotorLeftCurrent !!!" << logs::end;
    return 0;
}
int AsservEsialR::getMotorRightCurrent()
{
    logger().error() << "TODO getMotorRightCurrent !!!" << logs::end;
    return 0;
}
long AsservEsialR::getLeftExternalEncoder()
{
    logger().error() << "TODO getLeftExternalEncoder !!!" << logs::end;
    return -99;
}
void AsservEsialR::getCountsExternal(int32_t *countR, int32_t *countL)
{
    logger().error() << "TODO getCountsExternal !!!" << logs::end;
}

void AsservEsialR::getDeltaCountsExternal(int32_t *countR, int32_t *countL)
{
    logger().error() << "TODO getDeltaCountsExternal !!!" << logs::end;
}

long AsservEsialR::getRightExternalEncoder()
{
    logger().error() << "TODO getRightExternalEncoder !!!" << logs::end;
    return -99;
}
long AsservEsialR::getLeftInternalEncoder()
{
    logger().error() << "TODO getLeftInternalEncoder !!!" << logs::end;
    return -99;
}
long AsservEsialR::getRightInternalEncoder()
{
    logger().error() << "TODO getRightInternalEncoder !!!" << logs::end;
    return -99;
}
void AsservEsialR::getCountsInternal(int32_t *countR, int32_t *countL)
{
    logger().error() << "TODO getCountsInternal !!!" << logs::end;
}
void AsservEsialR::resetEncoders()
{
    logger().error() << "TODO resetEncoders !!!" << logs::end;
}
void AsservEsialR::resetInternalEncoders()
{
    logger().error() << "TODO resetInternalEncoders !!!" << logs::end;
}
void AsservEsialR::resetExternalEncoders()
{
    logger().error() << "TODO resetExternalEncoders !!!" << logs::end;
}

void AsservEsialR::odo_SetPosition(float x_mm, float y_mm, float angle_rad)
{
    if (odo_ != NULL) {
        //logger().debug() << "BEFORE odo_SetPosition x_mm=" << x_mm << " y_mm=" << y_mm << " angle_rad=" << angle_rad << logs::end;

        lock();
        odo_->setX(Utils::mmToUO(odo_, (int) floor(x_mm))); //UO
        odo_->setY(Utils::mmToUO(odo_, (int) floor(y_mm)));
        odo_->setTheta(angle_rad);

        p_.x = x_mm;
        p_.y = y_mm;
        p_.theta = angle_rad;
        p_.asservStatus = commandM_->getCommandStatus();

        unlock();
    }
}
RobotPosition AsservEsialR::odo_GetPosition()
{
    if (odo_ != NULL) {
        lock();
        p_.x = (float) (odo_->getXmm());
        p_.y = (float) (odo_->getYmm());
        p_.theta = (float) odo_->getTheta();
        p_.asservStatus = commandM_->getCommandStatus();
        unlock();
//        logger().debug() << "odo_GetPosition p_.x="
//                << p_.x
//                << " p_.y="
//                << p_.y
//                << " p_.theta="
//                << p_.theta
//                << " p_.asservStatus="
//                << p_.asservStatus
//                << " "
//                << odo_->getX()
//                << " "
//                << odo_->getY()
//                << logs::end;
    } else
        logger().error() << "odo_GetPosition odo_ is NULL!!!!!!!!!!!" << logs::end;
    return p_;
}

int AsservEsialR::path_GetLastCommandStatus()
{
    //TODO path_GetLastCommandStatus deprecated ?

    logger().error() << "TODO DEPRECATED ? path_GetLastCommandStatus !!!!!!!!!!!!!" << logs::end;
    return -1;
}
void AsservEsialR::path_InterruptTrajectory()
{
    //printf("path_InterruptTrajectory() sent !!!!!\n");
    commandM_->setEmergencyStop();
    pathStatus_ = TRAJ_INTERRUPTED;
}
void AsservEsialR::path_CollisionOnTrajectory()
{
    //printf("path_CollisionOnTrajectory() sent !!!!!\n");
    commandM_->setEmergencyStop();
    pathStatus_ = TRAJ_NEAR_OBSTACLE;
}
void AsservEsialR::path_CollisionRearOnTrajectory()
{
    //printf("path_CollisionRearOnTrajectory() sent !!!!!\n");
    commandM_->setEmergencyStop();
    pathStatus_ = TRAJ_NEAR_OBSTACLE;
}
void AsservEsialR::path_CancelTrajectory()
{
    //printf("path_CancelTrajectory() sent !!!!!\n");
    commandM_->setEmergencyStop();
    pathStatus_ = TRAJ_IMPOSSIBLE;
}
void AsservEsialR::path_ResetEmergencyStop()
{
    logger().debug() << "______________________path_ResetEmergencyStop() !!!!!!!!!!!!!! " << logs::end;
    commandM_->resetEmergencyStop();
    pathStatus_ = TRAJ_OK;
}

TRAJ_STATE AsservEsialR::waitEndOfTraj()
{
    //logger().debug() << "_______________________waitEndOfTraj() "<< logs::end;
    int timeout = 0;
    //attente du running status
    while (p_.asservStatus != 1) {
//        logger().info() << " 111 waitEndOfTraj()  xmm=" << p_.x * 1000 << std::setw(10) << " ymm=" << p_.y * 1000
//                << std::setw(10) << std::fixed << std::setprecision(3) << " deg=" << p_.theta * 180 / M_PI
//                << std::setw(10) << " s=" << p_.asservStatus << " timeout=" << timeout<< logs::end;

        utils::sleep_for_micros(10000);
        timeout++;
        if (timeout > 10) {
            logger().debug() << "_______________________waitEndOfTraj() => break" << logs::end;
            break;
        }
    }

    timeout = 0;
    //attente de l'interruption ou fin de trajectoire
    while (p_.asservStatus == 1) {
        //logger().info() << "222 waitEndOfTraj()  xmm=" << p_.x * 1000 << std::setw(10) << " ymm=" << p_.y * 1000 << std::setw(10) << std::fixed << std::setprecision(3) << " deg="<< p_.theta * 180 / M_PI << std::setw(10) << " s=" << p_.asservStatus << logs::end;
        utils::sleep_for_micros(1000);
        timeout++;
        if (timeout > 10 && p_.asservStatus != 1) {
            break;
        }
    }

//    logger().debug() << "END waitEndOfTraj()  xmm=" << p_.x * 1000 << std::setw(10) << " ymm=" << p_.y * 1000
//            << std::setw(10) << std::fixed << std::setprecision(3) << " deg=" << p_.theta * 180 / M_PI << std::setw(10)
//            << " s=" << p_.asservStatus << logs::end;

    //blocage!!
    if (p_.asservStatus == 3) {
        return TRAJ_COLLISION;
    } else if (p_.asservStatus == 0) {
        return TRAJ_FINISHED;
    } else if (p_.asservStatus == 2) {
        logger().debug() << "_______________________waitEndOfTraj() EMERGENCY STOP OCCURRED  pathStatus_= "
                << pathStatus_ << logs::end;
        return pathStatus_;
    } else
        return TRAJ_ERROR;
}

TRAJ_STATE AsservEsialR::motion_DoLine(float dist_mm)
{
    commandM_->addStraightLine(dist_mm);

    return waitEndOfTraj();
}
TRAJ_STATE AsservEsialR::motion_DoFace(float x_mm, float y_mm)
{
    commandM_->addGoToAngle(x_mm, y_mm);

    return waitEndOfTraj();
}
TRAJ_STATE AsservEsialR::motion_DoFaceReverse(float x_mm, float y_mm)
{
    commandM_->addGoToAngleReverse(x_mm, y_mm);

    return waitEndOfTraj();
}

TRAJ_STATE AsservEsialR::motion_DoRotate(float angle_radians)
{
    commandM_->addTurn((angle_radians * 180.0) / M_PI);

    return waitEndOfTraj();
}
TRAJ_STATE AsservEsialR::motion_DoArcRotate(float angle_radians, float radius)
{
    logger().error() << "motion_DoArcRotate TODO !" << logs::end;
    //TODO motion_DoArcRotate
    return TRAJ_ERROR;
}

TRAJ_STATE AsservEsialR::motion_DoDirectLine(float dist_mm)
{
    if (odo_ != NULL) {
        consignC_->add_dist_consigne(Utils::mmToUO(odo_, dist_mm));

        return waitEndOfTraj();
    } else
        return TRAJ_ERROR;
}

TRAJ_STATE AsservEsialR::motion_Goto(float x_mm, float y_mm)
{
    motion_DoFace(x_mm, y_mm);

    lock();
    float dx = x_mm - p_.x;
    float dy = y_mm - p_.y;
    unlock();

    float dist = sqrt(dx * dx + dy * dy);
    return motion_DoLine(dist);
}

TRAJ_STATE AsservEsialR::motion_GotoReverse(float x_mm, float y_mm)
{
    motion_DoFaceReverse(x_mm, y_mm);

    lock();
    float dx = x_mm - p_.x;
    float dy = y_mm - p_.y;
    unlock();

    float dist = sqrt(dx * dx + dy * dy);
    return motion_DoLine(-dist);
}

TRAJ_STATE AsservEsialR::motion_GotoChain(float x_mm, float y_mm)
{
    logger().error() << "motion_GotoChain TODO !" << logs::end;
    return TRAJ_ERROR;
}

TRAJ_STATE AsservEsialR::motion_GotoReverseChain(float x_mm, float y_mm)
{
    logger().error() << "motion_GotoReverseChain TODO !" << logs::end;
    return TRAJ_ERROR;
}

void AsservEsialR::motion_FreeMotion(void)
{
    consignC_->perform_On(false);
    commandM_->perform_On(false);
}

//DEPRECEATED
void AsservEsialR::motion_DisablePID()
{
    motion_FreeMotion();
}

void AsservEsialR::motion_AssistedHandling(void)
{
    consignC_->perform_On(true);
    commandM_->perform_On(true);
}
void AsservEsialR::motion_ActivateManager(bool enable)
{
    if (enable) {
        initAsserv();
        run_ = true;
    } else {
        stopAsserv();
        run_ = false;
    }
}
void AsservEsialR::motion_setLowSpeedForward(bool enable, int percent)
{
    consignC_->setLowSpeedForward(enable, percent);
}

void AsservEsialR::motion_setLowSpeedBackward(bool enable, int percent)
{
    consignC_->setLowSpeedBackward(enable, percent);
}

void AsservEsialR::motion_ActivateReguDist(bool enable)
{
    consignC_->dist_Regu_On(enable);
}
void AsservEsialR::motion_ActivateReguAngle(bool enable)
{
    consignC_->angle_Regu_On(enable);
}
void AsservEsialR::motion_ResetReguDist()
{
    consignC_->reset_regu_dist();
}
void AsservEsialR::motion_ResetReguAngle()
{
    consignC_->reset_regu_angle();
}

void AsservEsialR::motion_ActivateQuadRamp(bool enable)
{
    consignC_->setQuadRamp_Angle(enable);
    consignC_->setQuadRamp_Dist(enable);
}
