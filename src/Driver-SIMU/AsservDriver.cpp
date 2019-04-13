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

AsservDriver::AsservDriver(std::string botid) : chrono_("AsservDriver.SIMU")
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
    rightMeters_= 0.0;
    leftMeters_= 0.0;

    //timesMultiplicator_ = 1000.0;

    asservStarted_ = false;
    //resetEncoders();

    //on demarre le thread
    this->start("AsservDriver::AsservDriver()" + botid);

    chrono_.start();
    logger().info() << "BOT ID started !!  botid_=" << botid_ << logs::end;

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

            /*
             loggerSvg().info() << "<circle cx=\"" << p.x * 1000.0 << "\" cy=\"" << -p.y * 1000.0
             << "\" r=\"1\" fill=\"blue\" />" << "<line x1=\"" << p.x * 1000.0 << "\" y1=\"" << -p.y * 1000.0
             << "\" x2=\"" << p.x * 1000.0 + cos(p.theta) * 25 << "\" y2=\"" << -p.y * 1000.0 - sin(p.theta) * 25
             << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::end;*/
        }
        chrono.waitTimer();
    }
}

//conversion 1 meter = n ticks
float AsservDriver::convertMetersToTicks(float meters)
{
    float ticks = (float)std::rint(meters * simuTicksPerMeter_);
	logger().debug() << " meters=" << meters
			<< " ticks=" << ticks
			<< " simuTicksPerMeter_=" << simuTicksPerMeter_
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
//	leftSpeed_ = (leftSpeed_ + wantedLeftSpeed_) / 2.0f;
//	if (std::abs(leftSpeed_ - wantedLeftSpeed_) < 0.0001f)
//		leftSpeed_ = wantedLeftSpeed_;
    //or perfect virtual motor
    leftSpeed_ = wantedLeftSpeed_;

    // utilise la real speed
    float tps = chrono_.getElapsedTimeInMilliSec();	//* timesMultiplicator_;
    float deltaT_ms = tps - tLeft_ms_;
    tLeft_ms_ = tps;



    float currentLeftMeters = inverseCodeurG_ * (deltaT_ms * leftSpeed_) / 1000.0f;
    mutexL_.lock();
    currentLeftCounter_ = convertMetersToTicks(currentLeftMeters);	//conversion Metre  n Ticks/metres
    leftMeters_ += currentLeftMeters;
    leftCounter_ = convertMetersToTicks(leftMeters_);
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
//	rightSpeed_ = (rightSpeed_ + wantedRightSpeed_) / 2.0f;
//	if (std::abs(rightSpeed_ - wantedRightSpeed_) < 0.0001f)
//		rightSpeed_ = wantedRightSpeed_;
    //or perfect virtual motor
    rightSpeed_ = wantedRightSpeed_;

    float tps = chrono_.getElapsedTimeInMilliSec();	//* timesMultiplicator_;
    float deltaT_ms = tps - tRight_ms_;
    tRight_ms_ = tps;
    float currentRightMeters = inverseCodeurD_ * (deltaT_ms * rightSpeed_) / 1000.0f;

    mutexR_.lock();
    currentRightCounter_ = convertMetersToTicks(currentRightMeters);	//conversion 1 meter = n ticks
    //logger().error() <<  "cho currentRightCounter_=" << currentRightCounter_ << " currentRightMeters=" << currentRightMeters << " rightMeters_="<< rightMeters_ <<logs::end;

    rightMeters_ += currentRightMeters;

    rightCounter_ = convertMetersToTicks(rightMeters_);
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
    leftMeters_ = 0.0;
    currentLeftCounter_ = 0.0;
    mutexL_.unlock();
    computeCounterL();

    mutexR_.lock();
    rightCounter_ = 0.0;
    rightMeters_ = 0.0;
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

void AsservDriver::odo_SetPosition(double x_m, double y_m, double angle_rad)
{
    m_pos.lock();
    p_.x = x_m;
    p_.y = y_m;
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
    logger().error() << "TODO path_InterruptTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CollisionOnTrajectory()
{
    //TODO interrupt move
    logger().error() << "TODO path_CollisionOnTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CollisionRearOnTrajectory()
{
    logger().error() << "TODO path_CollisionRearOnTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_CancelTrajectory()
{
    logger().error() << "TODO path_CancelTrajectory !!!!!" << logs::end;
}
void AsservDriver::path_ResetEmergencyStop()
{
    logger().error() << "TODO path_ResetEmergencyStop !!!!!" << logs::end;
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm)
{
    m_pos.lock();
    float x_init = p_.x;
    float y_init = p_.y;
    float t_init = p_.theta;
    m_pos.unlock();
//motion_DoRotate(x_init +);
    //TODO motion_DoFace

    return TRAJ_OK;
}

TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters)
{

//calcul du point d'arrivé
    m_pos.lock();
    float x_init = p_.x;
    float y_init = p_.y;
    float t_init = p_.theta;
    m_pos.unlock();

//le delta
    double deltaXm = cos(t_init) * dist_meters;
    if (abs(deltaXm) < 0.0001)
        deltaXm = 0;

    double deltaYm = sin(t_init) * dist_meters;
    if (abs(deltaYm) < 0.0001)
        deltaYm = 0;

//Ax+b
    double a = 0, b = 0;
    if (deltaXm != 0) //cas droite verticale
            {
        a = deltaYm / deltaXm;
        b = y_init - (a * x_init);
    }

    logger().debug() << "dist_meters=" << dist_meters << "deltaXm=" << deltaXm << " deltaYm=" << deltaYm << " a=" << a
            << " b=" << b << "  !!!!!" << logs::end;

//While à une certaine vitesse
//set des valeurs
//TODO break si path_collision
    float speedms = 0.3; //m/s
    int increment_time_us = 5000;
    double tps_sec = abs(dist_meters / speedms);
    float increment_m = ((increment_time_us / 1000000.0) * speedms);
    int nb_increment = dist_meters / increment_m;

    logger().debug() << "tps=" << tps_sec << " increment_m=" << increment_m << "  !!!!!" << logs::end;

    asservStarted_ = true;
    for (int nb = 0; nb < nb_increment; nb++) {

        m_pos.lock();
        if (deltaXm == 0) //cas droite verticale
                {
            if (deltaYm > 0)
                p_.y += increment_m;
            else if (deltaYm < 0)
                p_.y -= increment_m;
        } else {
            float x_increment_m = cos(t_init) * increment_m;
            p_.x += x_increment_m;
            p_.y = (a * p_.x) + b;
        }
        m_pos.unlock();
        usleep(increment_time_us);
    }

    m_pos.lock();
    if (deltaXm == 0) //cas droite verticale
            {
        p_.y = y_init + deltaYm;
    } else {
        p_.x = x_init + deltaXm;
        p_.y = y_init + deltaYm;
    }
    m_pos.unlock();
    usleep(increment_time_us * 5);

    asservStarted_ = false;

    return TRAJ_OK;
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
    return TRAJ_OK;
}

TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
    logger().error() << "TODO motion_DoArcRotate !!!!!" << logs::end;
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
    logger().error() << "TODO motion_ActivateManager !!!!!" << logs::end;
}

void AsservDriver::motion_setLowSpeed(bool enable)
{
    logger().error() << "TODO motion_setLowSpeed !!!!!" << logs::end;
}

void AsservDriver::motion_ActivateReguDist(bool enable)
{
    logger().error() << "TODO motion_ActivateReguDist !!!!!" << logs::end;
}
void AsservDriver::motion_ActivateReguAngle(bool enable)
{
    logger().error() << "TODO motion_ActivateReguAngle !!!!!" << logs::end;
}
void AsservDriver::motion_ResetReguDist()
{
    logger().error() << "TODO motion_ResetReguDist !!!!!" << logs::end;
}
void AsservDriver::motion_ResetReguAngle()
{
    logger().error() << "TODO motion_ResetReguAngle !!!!!" << logs::end;
}
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_meters)
{
    //TODO
    logger().error() << "TODO motion_DoDirectLine !!!!!" << logs::end;
    return TRAJ_ERROR;
}
