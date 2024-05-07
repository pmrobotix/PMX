#include "AsservDriver.hpp"

#include <cmath>
#include <cstdint>

#include "../Log/Logger.hpp"

using namespace std;

AAsservDriver* AAsservDriver::create(std::string botid, ARobotPositionShared *aRobotPositionShared)
{
    static AsservDriver *instance = new AsservDriver(botid, aRobotPositionShared);
    return instance;
}

AsservDriver::AsservDriver(std::string botid, ARobotPositionShared *aRobotPositionShared) :
        chrono_("AsservDriver.SIMU")
{
    botid_ = botid;
    robotPositionShared_ = aRobotPositionShared;

    //config des moteurs et codeurs en SIMU
    inverseCodeurG_ = 1.0; //1.0 or -1.0
    inverseCodeurD_ = 1.0;
    inverseMoteurG_ = 1.0;
    inverseMoteurD_ = 1.0;

    emergencyStop_ = false;

    if (botid == "APF9328Robot") {
        //printf("--- AsservDriver - botid == APF9328Robot\n");
        //CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------
        simuTicksPerMeter_ = 1470.0; //nb ticks for 1000mm
        simuMaxSpeed_ = 1.0; //m/s
        simuMaxPower_ = 127.0;
        periodTime_us_ = 15000;
        //CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------

    } else if (botid == "LegoEV3Robot") {
        //printf("--- AsservDriver - botid == LegoEV3Robot\n");
        //CONFIGURATION EV3 SIMULATEUR CONSOLE --------------------------------------------
        simuTicksPerMeter_ = 130566.0f; //nb ticks for 1000mm
        simuMaxSpeed_ = 0.5; //m/s
        simuMaxPower_ = 100.0;
        periodTime_us_ = 20000;
        asservSimuStarted_ = false;
        //CONFIGURATION EV3 SIMULATEUR CONSOLE --------------------------------------------

    } else if (botid == "OPOS6UL_Robot") {
        //printf("--- AsservDriver - botid == OPOS6UL_Robot\n");
        //CONFIGURATION OPOS6UL_Robot SIMULATEUR CONSOLE  --------------------------------------------
        simuTicksPerMeter_ = 130566.0f; //nb ticks for 1000mm
        simuMaxSpeed_ = 0.5; //m/s
        simuMaxPower_ = 100.0; //127.0;
        periodTime_us_ = 2000;
        asservSimuStarted_ = true;
        //CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------
    } else {
        logger().error() << "NO BOT ID!! => EXIT botid_=" << botid_ << logs::end;
        exit(-1);
    }

    simuCurrentSpeed_ = simuMaxSpeed_;

    //reset position
    p_.x = 0.0;
    p_.y = 0.0;
    p_.theta = 0.0;
    p_.asservStatus = 2;

    tLeft_ms_ = 0.0;
    tRight_ms_ = 0.0;

    leftSpeed_ = 0.0; // m/s
    rightSpeed_ = 0.0;

    wantedRightSpeed_ = 0.0;
    wantedLeftSpeed_ = 0.0;

    currentLeftCounter_ = 0.0;
    currentRightCounter_ = 0.0;

    rightCounter_ = 0.0;
    leftCounter_ = 0.0;
    rightMm_ = 0.0;
    leftMm_ = 0.0;

    //timesMultiplicator_ = 1000.0;

    resetEncoders();

    chrono_.start();
    if (asservSimuStarted_) {
        //on demarre le check de positionnement...
        //asservSimuStarted_ = true;
        this->start("AsservDriver::AsservDriver()" + botid_, 3);
    }
//    else {
//        //stop the thread
//        endWhatTodo();
//        asservSimuStarted_ = false;
//        this->cancel();
//    }
}

AsservDriver::~AsservDriver()
{
    //endWhatTodo();
    asservSimuStarted_ = false;
    if (twLeft_.joinable())
        twLeft_.join();
    if (twRight_.joinable())
        twRight_.join();

    this->cancel();
}

void AsservDriver::execute()
{

    //int periodTime_us = 15000; //15MS FOR THE LEGO ROBOT, 2ms for opos6ul
    utils::Chronometer chrono("AsservDriver::execute().SIMU");
    chrono.setTimer(periodTime_us_);
    ROBOTPOSITION p;
    ROBOTPOSITION pp; //position précédente

    m_pos.lock();
    p = odo_GetPosition();
    robotPositionShared_->setRobotPosition(p);
    m_pos.unlock();
    pp = p;

    while (1) {
//        if (asservSimuStarted_) {
            m_pos.lock();
            p = odo_GetPosition();
            robotPositionShared_->setRobotPosition(p); //retourne 0 en position ????
            m_pos.unlock();
//logger().error() << "execute() p.x=" << p.x << " p.y=" << p.y << " chrono=" << chrono.getElapsedTimeInMicroSec() << logs::end;

            //TODO avoir accès au robot pour afficher du SVG
            //robot_->svgw().writePosition_BotPos(p.x, p.y, p.theta);

            //si different du precedentx
            if (!(p.x == pp.x && p.y == pp.y && p.theta == pp.theta)) {

                loggerSvg().info() << "<circle cx=\"" << p.x << "\" cy=\"" << -p.y << "\" r=\"1\" fill=\"blue\" />"
                        << "<line x1=\"" << p.x << "\" y1=\"" << -p.y << "\" x2=\"" << p.x + cos(p.theta) * 25
                        << "\" y2=\"" << -p.y - sin(p.theta) * 25 << "\" stroke-width=\"0.1\" stroke=\"grey\"  />"
                        << logs::end;
            }
            pp = p;
//        }
        chrono.waitTimer();
    }
}

//void AsservDriver::execute() {
//    //start the timer in this thread
//
//}
//
//void AsservDriver::onTimer(utils::Chronometer chrono) {
//    RobotPosition p;
//    if (asservStarted_) {
//        //logger().error() << "nb=" << nb << " chrono=" << chrono.getElapsedTimeInMicroSec()    << logs::end;
//        m_pos.lock();
//        p = odo_GetPosition();
//        m_pos.unlock();
//
//        loggerSvg().info() << "<circle cx=\""
//                << p.x
//                << "\" cy=\""
//                << -p.y
//                << "\" r=\"1\" fill=\"blue\" />"
//                << "<line x1=\""
//                << p.x
//                << "\" y1=\""
//                << -p.y
//                << "\" x2=\""
//                << p.x + cos(p.theta) * 25
//                << "\" y2=\""
//                << -p.y - sin(p.theta) * 25
//                << "\" stroke-width=\"0.1\" stroke=\"grey\"  />"
//                << logs::end;
//    }
//}
//
//void AsservDriver::onTimerEnd(utils::Chronometer chrono) {
//
//}

void AsservDriver::endWhatTodo()
{
    motion_FreeMotion();
    if (!this->isFinished())
        this->cancel();
    chrono_.stop();
}

float AsservDriver::convertMmToTicks(float mm)
{

    float ticks = (float) std::rint((mm * simuTicksPerMeter_) / 1000.0f);
    logger().debug() << " mm=" << mm << " ticks=" << ticks << " simuTicksPerMeter_=" << simuTicksPerMeter_ << logs::end;
    return ticks;
}

float AsservDriver::convertPowerToSpeed(int power)
{
    if (botid_ == "APF9328Robot") {
//		if (power < 7 && power > -7) //simule que le robot n'avance pas à tres faible puissance
//			return 0.0;
    } else if (botid_ == "LegoEV3Robot") {
//		if (power < 15 && power > -15) //simule que le robot n'avance pas à tres faible puissance
//			return 0.0;
    } else if (botid_ == "OPOS6UL_Robot") {
    } else {
        logger().error() << "NO BOT ID!! => EXIT botid_=" << botid_ << logs::end;
        exit(-1);
    }

    float speed = ((float) power * simuCurrentSpeed_) / simuMaxPower_; //vitesse max = 250mm/s pour 860 de power

//	logger().error() << " power=" << power
//			<< " speed=" << speed
//			<< " simuMaxSpeed_=" << simuMaxSpeed_
//			<< " simuMaxPower_=" << simuMaxPower_
//			<< logs::end;

    return speed;
}

void AsservDriver::computeCounterL()
{
    // MAJ real speed
    leftSpeed_ = (leftSpeed_ + wantedLeftSpeed_) / 2.0f;
    if (std::abs(leftSpeed_ - wantedLeftSpeed_) < 0.0001f)
        leftSpeed_ = wantedLeftSpeed_;
    //or perfect virtual motor
    //leftSpeed_ = wantedLeftSpeed_;

    // utilise la real speed
    float tps = chrono_.getElapsedTimeInMilliSec();	//* timesMultiplicator_;
    float deltaT_ms = tps - tLeft_ms_;
    tLeft_ms_ = tps;

    float currentLeftMm = inverseCodeurG_ * (deltaT_ms * leftSpeed_);
    mutexL_.lock();
    currentLeftCounter_ = convertMmToTicks(currentLeftMm);	//conversion mm to ticks
    leftMm_ += currentLeftMm;
    leftCounter_ = convertMmToTicks(leftMm_);
    mutexL_.unlock();

//    logger().info() << "computeCounterL "
//            << " ms="
//            << deltaT_ms
//            << " LSpeed_="
//            << leftSpeed_
//            << " wantedLSpeed_="
//            << wantedLeftSpeed_
//            << " LCounter_="
//            << leftCounter_
//            << " currLCounter_="
//            << currentLeftCounter_
//            << " currentLeftMm="
//            << currentLeftMm
//            << logs::end;

//	loggerM().debug() << "computeCounterL "
//			<< " ms="
//			<< deltaT_ms
//			<< " LSpeed_="
//			<< leftSpeed_
//			<< " wantedLSpeed_="
//			<< wantedLeftSpeed_
//			<< " LCounter_="
//			<< leftCounter_
//			<< " currLCounter_="
//			<< currentLeftCounter_
//			<< " currentMeters="
//			<< currentLeftMeters
//			<< logs::end;

}

void AsservDriver::computeCounterR()
{
    // MAJ real speed
    rightSpeed_ = (rightSpeed_ + wantedRightSpeed_) / 2.0f;
    if (std::abs(rightSpeed_ - wantedRightSpeed_) < 0.0001f)
        rightSpeed_ = wantedRightSpeed_;
    //or perfect virtual motor
    //rightSpeed_ = wantedRightSpeed_;

    float tps = chrono_.getElapsedTimeInMilliSec();	//* timesMultiplicator_;
    float deltaT_ms = tps - tRight_ms_;
    tRight_ms_ = tps;
    float currentRightMm = inverseCodeurD_ * (deltaT_ms * rightSpeed_);

    mutexR_.lock();
    currentRightCounter_ = convertMmToTicks(currentRightMm);	//conversion mm to Tick = n ticks
    //logger().error() <<  "cho currentRightCounter_=" << currentRightCounter_ << " currentRightMeters=" << currentRightMeters << " rightMeters_="<< rightMeters_ <<logs::end;

    rightMm_ += currentRightMm;

    rightCounter_ = convertMmToTicks(rightMm_);
    mutexR_.unlock();

//	logger().info() << "computeCounterR "
//			<< " ms="
//			<< deltaT_ms
//			<< " RSpeed_="
//			<< rightSpeed_
//			<< " wantedRSpeed_="
//			<< wantedRightSpeed_
//			<< " RCounter_="
//			<< rightCounter_
//			<< " currRCounter_="
//			<< currentRightCounter_
//			<< " currentRightMm="
//                              << currentRightMm
//                              << logs::end;

//	loggerM().debug() << "computeCounterR "
//			<< " rightCounter_="
//			<< rightCounter_
//			<< " currentRightCounter_="
//			<< currentRightCounter_
//			<< " currentMeters="
//			<< currentRightMeters
//			<< logs::end;

}

void AsservDriver::setMotorLeftPosition(int power, long ticksToDo)
{

    if (emergencyStop_)
        return;

    if (twLeft_.joinable())
        twLeft_.join();

    int sens = 0;
    if (power < 0)
        sens = -1;
    else if (power > 0)
        sens = 1;

    computeCounterL();
    mutexL_.lock();
    wantedLeftSpeed_ = inverseMoteurG_ * convertPowerToSpeed(power);
    tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();	//* timesMultiplicator_;
    mutexL_.unlock();

//    logger().debug() << "setMotorLeftPosition    power="
//            << power
//            << " ticksToDo="
//            << ticksToDo
//            << " wantedLeftSpeed_="
//            << wantedLeftSpeed_
//            << logs::end;

    AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
    twLeft_ = w_->positionLeftThread("setMotorLeftPosition", (int) leftCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorRightPosition(int power, long ticksToDo)
{
    if (emergencyStop_)
        return;

    if (twRight_.joinable())
        twRight_.join();

    int sens = 0;
    if (power < 0)
        sens = -1;
    else if (power > 0)
        sens = 1;

    computeCounterR();
    mutexR_.lock();
    wantedRightSpeed_ = inverseMoteurD_ * convertPowerToSpeed(power);
    tRight_ms_ = chrono_.getElapsedTimeInMilliSec();	//* timesMultiplicator_;
    mutexR_.unlock();

//    logger().debug() << "setMotorRightPosition    power="
//            << power
//            << " ticksToDo="
//            << ticksToDo
//            << " wantedRightSpeed_="
//            << wantedRightSpeed_
//            << logs::end;

    AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
    twRight_ = w_->positionRightThread("setMotorRightPosition", (int) rightCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorLeftPower(int power, int time_ms) //in ticks per sec
{
    if (emergencyStop_)
        return;
    //computeCounterL();
    mutexL_.lock();
    wantedLeftSpeed_ = inverseMoteurG_ * convertPowerToSpeed(power);
    tLeft_ms_ = chrono_.getElapsedTimeInMilliSec(); //* timesMultiplicator_;
    mutexL_.unlock();
    //logger().debug() << "setMotorLeftPower power=" << power << " leftSpeed_=" << leftSpeed_ << logs::end;

    if (time_ms > 0) {
        if (twLeft_.joinable())
            twLeft_.join();
        AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
        twLeft_ = w_->memberLeftThread("setMotorLeftPower", time_ms);
    }
}
void AsservDriver::setMotorRightPower(int power, int time_ms)
{
    if (emergencyStop_)
        return;
    //logger().info() << "!!!! setMotorRightPower rightMeters_=" << rightMeters_ << logs::end;
    //computeCounterR();
    mutexR_.lock();
    wantedRightSpeed_ = inverseMoteurD_ * convertPowerToSpeed(power);
    tRight_ms_ = chrono_.getElapsedTimeInMilliSec(); //* timesMultiplicator_;
    mutexR_.unlock();
    //logger().debug() << "setMotorRightPower power=" << power << " rightSpeed_=" << rightSpeed_ << logs::end;

    if (time_ms > 0) {
        if (twRight_.joinable())
            twRight_.join();
        AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
        twRight_ = w_->memberRightThread("setMotorRightPower", time_ms);
    }
}

//recupere les ticks codeurs cumulés
void AsservDriver::getCountsExternal(int32_t *countR, int32_t *countL)
{

    computeCounterL();
    computeCounterR();

    *countR = (int) rightCounter_;
    *countL = (int) leftCounter_;
}

//recupere les ticks codeurs depuis le derniers appels
void AsservDriver::getDeltaCountsExternal(int32_t *deltaR, int32_t *deltaL)
{

    computeCounterL();
    computeCounterR();

    *deltaR = (int) currentRightCounter_;
    *deltaL = (int) currentLeftCounter_;
}

//recupere les ticks codeur accummulés
void AsservDriver::getCountsInternal(int32_t *countR, int32_t *countL)
{
    //TODO getCountsInternal
    logger().error() << "TODO getCountsInternal !!!!!" << logs::end;
}

//recupere les ticks codeur accummulés
long AsservDriver::getLeftExternalEncoder()
{
    computeCounterL();
//    logger().debug() << "getLeftExternalEncoder=" << leftCounter_ << logs::end;
    return (long) leftCounter_; //ticks
}
//+/- 2,147,483,648
long AsservDriver::getRightExternalEncoder()
{
    computeCounterR();
//    logger().debug() << "getRightExternalEncoder=" << rightCounter_ << logs::end;
    return (long) rightCounter_; //ticks
}

long AsservDriver::getLeftInternalEncoder()
{
    return (long) getLeftExternalEncoder();
}

//+/- 2,147,483,648
long AsservDriver::getRightInternalEncoder()
{
    return (long) getRightExternalEncoder();
}

void AsservDriver::resetEncoders()
{
    mutexL_.lock();
    leftCounter_ = 0.0;
    leftMm_ = 0.0;
    currentLeftCounter_ = 0.0;
    mutexL_.unlock();
    computeCounterL();

    mutexR_.lock();
    rightCounter_ = 0.0;
    rightMm_ = 0.0;
    currentRightCounter_ = 0.0;
    mutexR_.unlock();
    computeCounterR();
}
void AsservDriver::resetInternalEncoders()
{
    //TODO
    logger().error() << "TODO resetInternalEncoders !!!!!" << logs::end;
}
void AsservDriver::resetExternalEncoders()
{
    //TODO
    logger().error() << "TODO resetExternalEncoders !!!!!" << logs::end;
}

void AsservDriver::stopMotorLeft()
{
    //computeCounterL();
    mutexL_.lock();
    currentLeftCounter_ = 0.0;
    wantedLeftSpeed_ = 0.0;
    mutexL_.unlock();
    computeCounterL();
    logger().debug() << "stopMotorLeft !!!!!" << logs::end;
}

void AsservDriver::stopMotorRight()
{
    //computeCounterR();
    mutexR_.lock();
    currentRightCounter_ = 0.0;
    wantedRightSpeed_ = 0.0;
    mutexR_.unlock();
    computeCounterR();
    logger().debug() << "stopMotorRight !!!!!" << logs::end;
}

int AsservDriver::getMotorLeftCurrent()
{
    logger().error() << "TODO getMotorLeftCurrent !!!!!" << logs::end;
    return 0;

}
int AsservDriver::getMotorRightCurrent()
{
    logger().error() << "TODO getMotorRightCurrent !!!!!" << logs::end;
    return 0;
}

void AsservDriver::odo_SetPosition(float x_mm, float y_mm, float angle_rad)
{
    m_pos.lock();
    p_.x = x_mm;
    p_.y = y_mm;
    p_.theta = angle_rad;
    m_pos.unlock();
}
ROBOTPOSITION AsservDriver::odo_GetPosition()
{
    return p_;
}
int AsservDriver::path_GetLastCommandStatus()
{
    return -1;
}
void AsservDriver::path_InterruptTrajectory()
{
    logger().error() << "TODO path_InterruptTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CollisionOnTrajectory()
{
    emergencyStop_ = true;
    stopMotorLeft();
    stopMotorRight();

}
void AsservDriver::path_CollisionRearOnTrajectory()
{
    emergencyStop_ = true;
    stopMotorLeft();
    stopMotorRight();

}
void AsservDriver::path_CancelTrajectory()
{
    logger().error() << "TODO path_CancelTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_ResetEmergencyStop()
{
    emergencyStop_ = false;
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    m_pos.lock();
    float x_init = p_.x;
    float y_init = p_.y;
    float t_init = p_.theta;
    m_pos.unlock();

    float deltaX = x_mm - x_init;
    float deltaY = y_mm - y_init;

    // La différence entre le thetaCible (= cap à atteindre) et le theta (= cap actuel du robot) donne l'angle à parcourir

    // Cap que doit atteindre le robot
    float thetaCible = atan2f(deltaY, deltaX);

    // La différence entre le thetaCible (= cap à atteindre) et le theta (= cap actuel du robot) donne l'angle à parcourir
    float deltaTheta = thetaCible - t_init;

    // On ajuste l'angle à parcourir pour ne pas faire plus d'un demi-tour
    // Exemple, tourner de 340 degrés est plus chiant que de tourner de -20 degrés
//    if (deltaTheta > M_PI) {
//        deltaTheta -= 2.0 * M_PI;
//    } else if (deltaTheta < -M_PI) {
//        deltaTheta += 2.0 * M_PI;
//    }

    deltaTheta = std::fmod(deltaTheta, 2.0 * M_PI);
    if (deltaTheta < -M_PI)
        deltaTheta += (2.0 * M_PI);
    if (deltaTheta > M_PI)
        deltaTheta -= (2.0 * M_PI);

    logger().debug() << "t_init=" << (t_init * 180.0f) / M_PI << " deltaTheta deg=" << (deltaTheta * 180.0f) / M_PI
            << " thetaCible=" << (thetaCible * 180.0f) / M_PI << logs::end;

    motion_DoRotate(deltaTheta);

    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoFaceReverse(float x_mm, float y_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    m_pos.lock();
    float x_init = p_.x;
    float y_init = p_.y;
    float t_init = p_.theta;
    m_pos.unlock();

    float deltaX = -(x_mm - x_init);
    float deltaY = -(y_mm - y_init);

    // La différence entre le thetaCible (= cap à atteindre) et le theta (= cap actuel du robot) donne l'angle à parcourir

    // Cap que doit atteindre le robot
    float thetaCible = atan2f(deltaY, deltaX);

    // La différence entre le thetaCible (= cap à atteindre) et le theta (= cap actuel du robot) donne l'angle à parcourir
    float deltaTheta = thetaCible - t_init;

    // On ajuste l'angle à parcourir pour ne pas faire plus d'un demi-tour
    // Exemple, tourner de 340 degrés est plus chiant que de tourner de -20 degrés
//    if (deltaTheta > M_PI) {
//        deltaTheta -= 2.0 * M_PI;
//    }
//    else if (deltaTheta < -M_PI) {
//        deltaTheta += 2.0 * M_PI;
//    }

    deltaTheta = std::fmod(deltaTheta, 2.0 * M_PI);
    if (deltaTheta < -M_PI)
        deltaTheta += (2.0 * M_PI);
    if (deltaTheta > M_PI)
        deltaTheta -= (2.0 * M_PI);

    logger().debug() << "t_init=" << (t_init * 180.0f) / M_PI << " deltaTheta deg=" << (deltaTheta * 180.0f) / M_PI
            << " thetaCible=" << (thetaCible * 180.0f) / M_PI << logs::end;

    motion_DoRotate(deltaTheta);

    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoLine(float dist_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
//calcul du point d'arrivé
    m_pos.lock();
    float x_init = p_.x;
    float y_init = p_.y;
    float t_init = p_.theta;
    m_pos.unlock();

    //cas du reverse
    float inv = 1.0;
    if (dist_mm < 0)
        inv = -inv;

//le delta
    float deltaXmm = cos(t_init) * dist_mm;
    if (abs(deltaXmm) < 0.0001)
        deltaXmm = 0;

    float deltaYmm = sin(t_init) * dist_mm;
    if (abs(deltaYmm) < 0.0001)
        deltaYmm = 0;

//Ax+b
    float a = 0, b = 0;
    //cas droite verticale
    if (deltaXmm != 0) {
        a = deltaYmm / deltaXmm;
        b = y_init - (a * x_init);
    }

    logger().debug() << "dist_mm=" << dist_mm << "deltaXmm=" << deltaXmm << " deltaYmm=" << deltaYmm << " a=" << a
            << " b=" << b << "  !!!!!" << logs::end;

    int increment_time_us = periodTime_us_ * 4; //us // on affiche que toutes les 4 periodes d'asserv

    float tps_sec = fabs(dist_mm / simuCurrentSpeed_ * 1000.0);
    float increment_mm = fabs((increment_time_us / 1000.0) * simuCurrentSpeed_);
    int nb_increment = (int) fabs(dist_mm / increment_mm);

    logger().debug() << "tps(ms)=" << tps_sec * 1000.0 << " increment_mm=" << increment_mm << " nb_increment="
            << nb_increment << "  !!!!!" << logs::end;

    for (int nb = 0; nb < nb_increment; nb++) {

        if (emergencyStop_)
            return TRAJ_NEAR_OBSTACLE;

        m_pos.lock();
        //cas droite verticale
        if (deltaXmm == 0) {
            if (deltaYmm > 0)
                p_.y += increment_mm;
            else if (deltaYmm < 0)
                p_.y -= increment_mm;
        } else {
            float x_increment_m = cos(t_init) * increment_mm;
            p_.x += inv * x_increment_m;
            p_.y = ((a * p_.x) + b);
        }
        m_pos.unlock();

        utils::sleep_for_micros(increment_time_us);
    }

    m_pos.lock();
    //cas droite verticale
    if (deltaXmm == 0) {
        p_.y = y_init + deltaYmm;
    } else {
        p_.x = x_init + deltaXmm;
        p_.y = y_init + deltaYmm;
    }
    m_pos.unlock();

    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;

    utils::sleep_for_micros(increment_time_us * 5); //fois 5 non necessaire ?
    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    //int increment_time_us = 5000;
    int increment_time_us = periodTime_us_ * 4; //us

    float theta_init = p_.theta;
    int nb_increment = 20;
    float temp_angle = angle_radians / nb_increment;
    for (int nb = 0; nb < nb_increment; nb++) {

        if (emergencyStop_)
            return TRAJ_NEAR_OBSTACLE;

        m_pos.lock();

        float temp = p_.theta + temp_angle;

        temp = std::fmod(temp, 2.0 * M_PI);
        if (temp < -M_PI)
            temp += (2.0 * M_PI);
        if (temp > M_PI)
            temp -= (2.0 * M_PI);

        p_.theta = temp;
        m_pos.unlock();
        utils::sleep_for_micros(increment_time_us);
    }

    m_pos.lock();
    float temp = theta_init + angle_radians;

//    if (temp >= M_PI) {
//        temp -= 2.0 * M_PI;
//    }
//    else if (temp < -M_PI) {
//        temp += 2.0 * M_PI;
//    }

    temp = std::fmod(temp, 2.0 * M_PI);
    if (temp < -M_PI)
        temp += (2.0 * M_PI);
    if (temp > M_PI)
        temp -= (2.0 * M_PI);

    p_.theta = temp;
    m_pos.unlock();

    if (emergencyStop_) {
        logger().error() << " AsservDriver::motion_DoRotate emergencyStop_ !!!!!" << logs::end;
        return TRAJ_NEAR_OBSTACLE;
    }

    utils::sleep_for_micros(increment_time_us * 7); //fois 7 non necessaire ?

    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    logger().error() << "TODO motion_DoArcRotate !!!!!" << logs::end;
    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_Goto(float x_mm, float y_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    motion_DoFace(x_mm, y_mm);

    m_pos.lock();
    float dx = x_mm - p_.x;
    float dy = y_mm - p_.y;
    m_pos.unlock();

    float dist = sqrt(dx * dx + dy * dy);
    return motion_DoLine(dist);
}

TRAJ_STATE AsservDriver::motion_GotoReverse(float x_mm, float y_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    motion_DoFaceReverse(x_mm, y_mm);

    m_pos.lock();
    float dx = x_mm - p_.x;
    float dy = y_mm - p_.y;
    m_pos.unlock();
    float dist = sqrt(dx * dx + dy * dy);
    return motion_DoLine(-dist);

}

TRAJ_STATE AsservDriver::motion_GotoChain(float x_mm, float y_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    return motion_Goto(x_mm, y_mm);
}

TRAJ_STATE AsservDriver::motion_GotoReverseChain(float x_mm, float y_mm)
{
    if (emergencyStop_)
        return TRAJ_NEAR_OBSTACLE;
    return motion_GotoReverse(x_mm, y_mm);
}

void AsservDriver::motion_FreeMotion()
{
    stopMotorLeft();
    stopMotorRight();
}
void AsservDriver::motion_DisablePID()
{ //DEPRECATED
    motion_FreeMotion();
}
void AsservDriver::motion_AssistedHandling()
{
    stopMotorLeft();
    stopMotorRight();
}
void AsservDriver::motion_ActivateManager(bool enable)
{
    //automatiquement activé.
}

void AsservDriver::motion_setLowSpeedForward(bool enable, int percent)
{
    logger().debug() << " motion_setLowSpeedForward !!!!!" << logs::end;
    if (enable)
        simuCurrentSpeed_ = simuMaxSpeed_ * percent / 100.0;
    else
        simuCurrentSpeed_ = simuMaxSpeed_;
}
void AsservDriver::motion_setLowSpeedBackward(bool enable, int percent)
{

    logger().debug() << " motion_setLowSpeedBackward !!!!!" << logs::end;
    if (enable)
        simuCurrentSpeed_ = -simuMaxSpeed_ * percent / 100.0;
    else
        simuCurrentSpeed_ = -simuMaxSpeed_;
}

void AsservDriver::motion_setMaxSpeed(bool enable, int speed_dist_m_sec, int speed_angle_rad_sec)
{
	logger().debug() << " motion_setLowSpeedForward !!!!!" << logs::end;
	    if (enable)
	        simuCurrentSpeed_ = simuMaxSpeed_ * (speed_dist_m_sec/12) / 100.0;
	    else
	        simuCurrentSpeed_ = simuMaxSpeed_;
}

//functions deprecated
void AsservDriver::motion_ActivateReguDist(bool enable)
{
    logger().error() << "motion_ActivateReguDist NOT IMPLEMENTED !!!!!" << logs::end;
}
void AsservDriver::motion_ActivateReguAngle(bool enable)
{
    logger().error() << "motion_ActivateReguAngle NOT IMPLEMENTED !!!!!" << logs::end;
}
/*
void AsservDriver::motion_ResetReguDist()
{
    logger().error() << "motion_ResetReguDist NOT IMPLEMENTED  !!!!!" << logs::end;
}
void AsservDriver::motion_ResetReguAngle()
{
    logger().error() << "motion_ResetReguAngle NOT IMPLEMENTED  !!!!!" << logs::end;
}
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_mm)
{
    logger().error() << "motion_DoDirectLine NOT IMPLEMENTED  !!!!!" << logs::end;
    return TRAJ_ERROR;
}*/
