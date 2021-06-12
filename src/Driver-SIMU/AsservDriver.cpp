#include "AsservDriver.hpp"

#include <cmath>
#include <cstdint>

#include "../Log/Logger.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create(std::string botid)
{
    static AsservDriver *instance = new AsservDriver(botid);
    return instance;
}

AsservDriver::AsservDriver(std::string botid) :
        chrono_("AsservDriver.SIMU")
{
    botid_ = botid;

    //config des moteurs et codeurs en SIMU
    inverseCodeurG_ = 1.0; //1.0 or -1.0
    inverseCodeurD_ = 1.0;
    inverseMoteurG_ = 1.0;
    inverseMoteurD_ = 1.0;

    if (botid == "APF9328Robot") {
        //printf("--- AsservDriver - botid == APF9328Robot\n");
        //CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------
        simuTicksPerMeter_ = 1470.0; //nb ticks for 1000mm
        simuMaxSpeed_ = 1.0; //m/s
        simuMaxPower_ = 127.0;
        //CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------

    } else if (botid == "LegoEV3Robot") {
        //printf("--- AsservDriver - botid == LegoEV3Robot\n");
        //CONFIGURATION EV3 SIMULATEUR CONSOLE --------------------------------------------
        simuTicksPerMeter_ = 4100.0f; //nb ticks for 1000mm
        simuMaxSpeed_ = 0.5; //m/s
        simuMaxPower_ = 100.0;
        //CONFIGURATION EV3 SIMULATEUR CONSOLE --------------------------------------------

    } else if (botid == "OPOS6UL_Robot") {
        //printf("--- AsservDriver - botid == OPOS6UL_Robot\n");
        //CONFIGURATION OPOS6UL_Robot SIMULATEUR CONSOLE  --------------------------------------------
        simuTicksPerMeter_ = 1470.0; //nb ticks for 1000mm
        simuMaxSpeed_ = 0.5; //m/s
        simuMaxPower_ = 127.0;
        //CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------
    } else {
        logger().error() << "NO BOT ID!!  botid_=" << botid_ << logs::end;
        exit(-1);
    }

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

    asservStarted_ = false;
    //resetEncoders();

    //on demarre le thread
    this->start("AsservDriver::AsservDriver()" + botid);

    chrono_.start();
    logger().debug() << "BOT ID started !!  botid_=" << botid_ << logs::end;

}

AsservDriver::~AsservDriver()
{
    if (twLeft_.joinable())
        twLeft_.join();
    if (twRight_.joinable())
        twRight_.join();
}

void AsservDriver::execute()
{
    int periodTime_us = 10000;
    utils::Chronometer chrono("AsservDriver::execute().SIMU");
    chrono.setTimer(periodTime_us);
    RobotPosition p;
    while (1) {

        if (asservStarted_) {
            //logger().error() << "nb=" << nb << " chrono=" << chrono.getElapsedTimeInMicroSec()	<< logs::end;
            m_pos.lock();
            p = odo_GetPosition();
            m_pos.unlock();

            loggerSvg().info() << "<circle cx=\"" << p.x  << "\" cy=\"" << -p.y
                    << "\" r=\"1\" fill=\"blue\" />" << "<line x1=\"" << p.x  << "\" y1=\"" << -p.y
                    << "\" x2=\"" << p.x  + cos(p.theta) * 25 << "\" y2=\"" << -p.y - sin(p.theta) * 25
                    << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::end;
        }
        chrono.waitTimer();
    }
}

void AsservDriver::endWhatTodo()
{
    if (!this->isFinished())
            this->cancel();
}

////conversion 1 meter = n ticks
//float AsservDriver::convertMetersToTicks(float meters)
//{
//    float ticks = (float) std::rint(meters * simuTicksPerMeter_);
//    logger().debug() << " meters=" << meters << " ticks=" << ticks << " simuTicksPerMeter_=" << simuTicksPerMeter_
//            << logs::end;
//    return ticks;
//}

float AsservDriver::convertMmToTicks(float mm)
{
    float ticks = (float) std::rint(mm * 1000.0f * simuTicksPerMeter_);
    logger().debug() << " mm=" << mm << " ticks=" << ticks << " simuTicksPerMeter_=" << simuTicksPerMeter_
            << logs::end;
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
    } else {
        logger().error() << "NO BOT ID!!  botid_=" << botid_ << logs::end;
        exit(-1);
    }

    float speed = ((float) power * simuMaxSpeed_) / simuMaxPower_; //vitesse max = 250mm/s pour 860 de power

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
    currentLeftCounter_ = convertMmToTicks(currentLeftMm);	//conversion Metre  n Ticks/metres
    leftMm_ += currentLeftMm;
    leftCounter_ = convertMmToTicks(leftMm_);
    mutexL_.unlock();

//	logger().info() << "computeCounterL "
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
    currentRightCounter_ = convertMmToTicks(currentRightMm);	//conversion 1 meter = n ticks
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
//			<< logs::end;

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

    logger().debug() << "setMotorLeftPosition    power=" << power << " ticksToDo=" << ticksToDo << " wantedLeftSpeed_="
            << wantedLeftSpeed_ << logs::end;

    AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
    twLeft_ = w_->positionLeftThread("setMotorLeftPosition", leftCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorRightPosition(int power, long ticksToDo)
{
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

    logger().debug() << "setMotorRightPosition    power=" << power << " ticksToDo=" << ticksToDo
            << " wantedRightSpeed_=" << wantedRightSpeed_ << logs::end;

    AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
    twRight_ = w_->positionRightThread("setMotorRightPosition", rightCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorLeftPower(int power, int time_ms) //in ticks per sec
{
    computeCounterL();
    mutexL_.lock();
    wantedLeftSpeed_ = inverseMoteurG_ * convertPowerToSpeed(power);
    tLeft_ms_ = chrono_.getElapsedTimeInMilliSec(); //* timesMultiplicator_;
    mutexL_.unlock();
    logger().debug() << "setMotorLeftPower power=" << power << " leftSpeed_=" << leftSpeed_ << logs::end;

    if (time_ms > 0) {
        if (twLeft_.joinable())
            twLeft_.join();
        AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
        twLeft_ = w_->memberLeftThread("setMotorLeftPower", time_ms);
    }
}
void AsservDriver::setMotorRightPower(int power, int time_ms)
{
    //logger().info() << "!!!! setMotorRightPower rightMeters_=" << rightMeters_ << logs::end;
    computeCounterR();
    mutexR_.lock();
    wantedRightSpeed_ = inverseMoteurD_ * convertPowerToSpeed(power);
    tRight_ms_ = chrono_.getElapsedTimeInMilliSec(); //* timesMultiplicator_;
    mutexR_.unlock();
    logger().debug() << "setMotorRightPower power=" << power << " rightSpeed_=" << rightSpeed_ << logs::end;

    if (time_ms > 0) {
        if (twRight_.joinable())
            twRight_.join();
        AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
        twRight_ = w_->memberRightThread("setMotorRightPower", time_ms);
    }
}

long AsservDriver::getLeftExternalEncoder()
{
    computeCounterL();
    logger().debug() << "getLeftExternalEncoder=" << leftCounter_ << logs::end;
    return (long) leftCounter_;
}
long AsservDriver::getRightExternalEncoder()
{
    computeCounterR();
    logger().debug() << "getRightExternalEncoder=" << rightCounter_ << logs::end;

    return (long) rightCounter_;
}

//+/- 2,147,483,648
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
    computeCounterL();
    mutexL_.lock();
    currentLeftCounter_ = 0.0;
    wantedLeftSpeed_ = 0.0;
    mutexL_.unlock();
    computeCounterL();
    logger().debug() << "stopMotorLeft !!!!!" << logs::end;
}

void AsservDriver::stopMotorRight()
{
    computeCounterR();
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
RobotPosition AsservDriver::odo_GetPosition()
{
    return p_;
}
int AsservDriver::path_GetLastCommandStatus()
{
    return -1;
}
void AsservDriver::path_InterruptTrajectory()
{
    logger().debug() << "TODO path_InterruptTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CollisionOnTrajectory()
{
    logger().debug() << "TODO path_CollisionOnTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CollisionRearOnTrajectory()
{
    logger().debug() << "TODO path_CollisionRearOnTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CancelTrajectory()
{
    logger().debug() << "TODO path_CancelTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_ResetEmergencyStop()
{
    logger().debug() << "TODO path_ResetEmergencyStop !!!!!" << logs::end;
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm)
{
    logger().error() << "TODO motion_DoFace !!!!!" << logs::end;
//    m_pos.lock();
//    float x_init = p_.x;
//    float y_init = p_.y;
//    float t_init = p_.theta;
//    m_pos.unlock();
//motion_DoRotate(x_init +);
    //TODO motion_DoFace

    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoLine(float dist_mm)
{

//calcul du point d'arrivé
    m_pos.lock();
    float x_init = p_.x;
    float y_init = p_.y;
    float t_init = p_.theta;
    m_pos.unlock();

//le delta
    double deltaXmm = cos(t_init) * dist_mm;
    if (abs(deltaXmm) < 0.0001)
        deltaXmm = 0;

    double deltaYmm = sin(t_init) * dist_mm;
    if (abs(deltaYmm) < 0.0001)
        deltaYmm = 0;

//Ax+b
    double a = 0, b = 0;
    if (deltaXmm != 0) //cas droite verticale
            {
        a = deltaYmm / deltaXmm;
        b = y_init - (a * x_init);
    }

    logger().debug() << "dist_mm=" << dist_mm << "deltaXmm=" << deltaXmm << " deltaYmm=" << deltaYmm << " a=" << a
            << " b=" << b << "  !!!!!" << logs::end;

//While à une certaine vitesse
//set des valeurs
//TODO break si path_collision
    float speedm_s = 0.3; //m/s
    int increment_time_us = 5000;
    double tps_sec = abs(dist_mm / speedm_s * 1000.0f);
    float increment_mm = ((increment_time_us / 1000.0) * speedm_s);
    int nb_increment = dist_mm / increment_mm;

    logger().debug() << "tps=" << tps_sec << " increment_mm=" << increment_mm << "  !!!!!" << logs::end;

    asservStarted_ = true;
    for (int nb = 0; nb < nb_increment; nb++) {

        m_pos.lock();
        if (deltaXmm == 0) //cas droite verticale
                {
            if (deltaYmm > 0)
                p_.y += increment_mm;
            else if (deltaYmm < 0)
                p_.y -= increment_mm;
        } else {
            float x_increment_m = cos(t_init) * increment_mm;
            p_.x += x_increment_m;
            p_.y = (a * p_.x) + b;
        }
        m_pos.unlock();
        usleep(increment_time_us);
    }

    m_pos.lock();
    if (deltaXmm == 0) //cas droite verticale
            {
        p_.y = y_init + deltaYmm;
    } else {
        p_.x = x_init + deltaXmm;
        p_.y = y_init + deltaYmm;
    }
    m_pos.unlock();
    usleep(increment_time_us * 5);

    asservStarted_ = false;

    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{
    int increment_time_us = 5000;

    m_pos.lock();
    float temp = p_.theta + angle_radians;

//logger().error() << "angle_radians=" << angle_radians << " p_.theta=" << p_.theta << " temp= " << temp << "  !!!!!"	<< logs::end;

    if (temp >= M_PI) {
        temp -= 2.0 * M_PI;
    } else if (temp < -M_PI) {
        temp += 2.0 * M_PI;
    }

    p_.theta = temp;
    m_pos.unlock();
//logger().error() <<  " temp= " << temp << "  !!!!!"	<< logs::end;
    usleep(increment_time_us * 7);
    return TRAJ_FINISHED;
}

TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
    logger().error() << "TODO motion_DoArcRotate !!!!!" << logs::end;
    return TRAJ_ERROR;
}


TRAJ_STATE AsservDriver::motion_Goto(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_GotoReverse(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_GotoChain(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_GotoReverseChain(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

void AsservDriver::motion_FreeMotion()
{
    stopMotorLeft();
    stopMotorRight();
}
void AsservDriver::motion_DisablePID()
{
    stopMotorLeft();
    stopMotorRight();
}
void AsservDriver::motion_AssistedHandling()
{
    stopMotorLeft();
    stopMotorRight();
}
void AsservDriver::motion_ActivateManager(bool enable)
{
    logger().debug() << "TODO motion_ActivateManager !!!!!" << logs::end;
}

void AsservDriver::motion_setLowSpeedForward(bool enable, int percent)
{
    logger().debug() << "TODO motion_setLowSpeedForward !!!!!" << logs::end;
}
void AsservDriver::motion_setLowSpeedBackward(bool enable, int percent)
{
    logger().debug() << "TODO motion_setLowSpeedBackward !!!!!" << logs::end;
}

void AsservDriver::motion_ActivateReguDist(bool enable)
{
    logger().debug() << "TODO motion_ActivateReguDist !!!!!" << logs::end;
}
void AsservDriver::motion_ActivateReguAngle(bool enable)
{
    logger().debug() << "TODO motion_ActivateReguAngle !!!!!" << logs::end;
}
void AsservDriver::motion_ResetReguDist()
{
    logger().debug() << "TODO motion_ResetReguDist !!!!!" << logs::end;
}
void AsservDriver::motion_ResetReguAngle()
{
    logger().debug() << "TODO motion_ResetReguAngle !!!!!" << logs::end;
}
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_mm)
{
    //TODO
    logger().debug() << "TODO motion_DoDirectLine !!!!!" << logs::end;
    return TRAJ_ERROR;
}
