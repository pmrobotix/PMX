#include "AsservDriver.hpp"

#include <cmath>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create(std::string botid)
{
	static AsservDriver *instance = new AsservDriver(botid);
	return instance;
}

AsservDriver::AsservDriver(std::string botid)
{
	botid_ = botid;

	if (botid == "APF9328Robot")
	{
		//printf("--- AsservDriver - botid == APF9328Robot\n");
		//CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------
		simuTicksPerMeter_ = 1470.0; //nb ticks for 1000mm
		simuMaxSpeed_ = 1.0; //m/s
		simuMaxPower_ = 127.0;
		//CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------

	}
	else if (botid == "LegoEV3Robot")
	{
		//printf("--- AsservDriver - botid == LegoEV3Robot\n");
		//CONFIGURATION EV3 SIMULATEUR CONSOLE --------------------------------------------
		simuTicksPerMeter_ = 1398.0; //3000.0; //nb ticks for 1000mm
		simuMaxSpeed_ = 0.6; //m/s
		simuMaxPower_ = 860.0;
		//CONFIGURATION EV3 SIMULATEUR CONSOLE --------------------------------------------

	}
	else if (botid == "OPOS6UL_Robot")
	{
		//printf("--- AsservDriver - botid == OPOS6UL_Robot\n");
		//CONFIGURATION OPOS6UL_Robot SIMULATEUR CONSOLE  --------------------------------------------
		simuTicksPerMeter_ = 1470.0; //nb ticks for 1000mm
		simuMaxSpeed_ = 0.5; //m/s
		simuMaxPower_ = 127.0;
		//CONFIGURATION APF9328 SIMULATEUR CONSOLE  --------------------------------------------
	}
	else
	{
		logger().error() << "NO BOT ID!!  botid_=" << botid_ << logs::end;
		exit(-1);
	}

	resetEncoders();

	//reset position
	p_.x = 0.0;
	p_.y = 0.0;
	p_.theta = 0.0;

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

	chrono_.start();

}

AsservDriver::~AsservDriver()
{
	if (twLeft_.joinable()) twLeft_.join();
	if (twRight_.joinable()) twRight_.join();
}

//conversion 1 meter = n ticks
float AsservDriver::convertMetersToTicks(float meters)
{
	float ticks = std::rint(meters * simuTicksPerMeter_);
//	logger().error() << " meters=" << meters
//			<< " ticks=" << ticks
//			<< " simuTicksPerMeter_=" << simuTicksPerMeter_
//			<< logs::end;
	return ticks;
}

float AsservDriver::convertPowerToSpeed(int power)
{
	if (botid_ == "APF9328Robot")
	{
//		if (power < 7 && power > -7) //simule que le robot n'avance pas à tres faible puissance
//			return 0.0;
	}
	else if (botid_ == "LegoEV3Robot")
	{
//		if (power < 15 && power > -15) //simule que le robot n'avance pas à tres faible puissance
//			return 0.0;
	}
	else
	{
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
	float tps = chrono_.getElapsedTimeInMilliSec();
	float deltaT_ms = tps - tLeft_ms_;
	tLeft_ms_ = tps;

	float currentLeftMeters = (deltaT_ms * leftSpeed_) / 1000.0f;
	mutexL_.lock();
	currentLeftCounter_ = convertMetersToTicks(currentLeftMeters);//conversion Metre  n Ticks/metres
	leftMeters_ += currentLeftMeters;
	leftCounter_ = convertMetersToTicks(leftMeters_);
	mutexL_.unlock();

//	logger().debug() << "computeCounterL "
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
//
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

	float tps = chrono_.getElapsedTimeInMilliSec();
	float deltaT_ms = tps - tRight_ms_;
	tRight_ms_ = tps;
	float currentRightMeters = (deltaT_ms * rightSpeed_) / 1000.0f;

	mutexR_.lock();
	currentRightCounter_ = convertMetersToTicks(currentRightMeters);//conversion 1 meter = n ticks
	rightMeters_ += currentRightMeters;
	rightCounter_ = convertMetersToTicks(rightMeters_);
	mutexR_.unlock();

//	loggerM().debug() << "computeCounterR "
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
//
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
	if (twLeft_.joinable()) twLeft_.join();

	int sens = 0;
	if (power < 0)
		sens = -1;
	else if (power > 0) sens = 1;

	computeCounterL();
	mutexL_.lock();
	wantedLeftSpeed_ = convertPowerToSpeed(power);
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexL_.unlock();

	logger().debug() << "setMotorLeftPosition    power=" << power << " ticksToDo=" << ticksToDo
			<< " wantedLeftSpeed_=" << wantedLeftSpeed_ << logs::end;

	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twLeft_ = w_->positionLeftThread("setMotorLeftPosition", leftCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorRightPosition(int power, long ticksToDo)
{
	if (twRight_.joinable()) twRight_.join();

	int sens = 0;
	if (power < 0)
		sens = -1;
	else if (power > 0) sens = 1;

	computeCounterR();
	mutexR_.lock();
	wantedRightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();
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
	wantedLeftSpeed_ = convertPowerToSpeed(power);
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexL_.unlock();
	logger().debug() << "setMotorLeftPower power=" << power << " leftSpeed_=" << leftSpeed_
			<< logs::end;
	//computeCounterL();

	if (time_ms > 0)
	{
		if (twLeft_.joinable()) twLeft_.join();
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twLeft_ = w_->memberLeftThread("setMotorLeftPower", time_ms);
	}
}
void AsservDriver::setMotorRightPower(int power, int time_ms)
{
	computeCounterR();
	mutexR_.lock();
	wantedRightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexR_.unlock();
	logger().debug() << "setMotorRightPower power=" << power << " rightSpeed_=" << rightSpeed_
			<< logs::end;

	//computeCounterR();
	if (time_ms > 0)
	{
		if (twRight_.joinable()) twRight_.join();
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twRight_ = w_->memberRightThread("setMotorRightPower", time_ms);
	}
}

long AsservDriver::getLeftExternalEncoder()
{
	computeCounterL();
	//logger().debug() << "getLeftExternalEncoder=" << leftCounter_ << logs::end;
	return (long) leftCounter_;
}
long AsservDriver::getRightExternalEncoder()
{
	computeCounterR();
	//logger().debug() << "getRightExternalEncoder=" << rightCounter_ << logs::end;

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
}
void AsservDriver::resetExternalEncoders()
{
	//TODO
}

void AsservDriver::stopMotorLeft()
{
	computeCounterL();
	mutexL_.lock();
	currentLeftCounter_ = 0.0;
	wantedLeftSpeed_ = 0.0;
	mutexL_.unlock();
	computeCounterL();
	logger().debug() << "stopMotorLeft !!!!!" << logs::end; //TODO plante
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
	return 0;

}
int AsservDriver::getMotorRightCurrent()
{
	return 0;
}


void AsservDriver::odo_SetPosition(double x_m, double y_m, double angle_rad)
{
	p_.x = x_m;
	p_.y = y_m;
	p_.theta = angle_rad;
}
RobotPosition AsservDriver::odo_GetPosition()
{
	return p_;
}
int AsservDriver::path_GetLastCommandStatus()
{
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
TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters)
{

	return TRAJ_OK;
}
TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
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

