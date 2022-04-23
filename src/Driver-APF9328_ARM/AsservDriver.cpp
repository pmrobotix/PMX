//drivers...ARM

#include "AsservDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create(std::string)
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver()
		: extRightEncoder_('D', 31), extLeftEncoder_('B', 17)
{
	md25_.begin();
	float volts = md25_.getBatteryVolts();
	logger().info() << "volts=" << volts << logs::end;

	//encoderLeft_(*this, 'B', 17), encoderRight_(*this, 'D', 31),
/*
	unsigned char status = extRightEncoder_.readStatus();
	logger().error() << "status spi=" << reinterpret_cast<void*>(status) << logs::end;
	extRightEncoder_.clearCounter();*/
}

AsservDriver::~AsservDriver()
{
}

void AsservDriver::setMotorLeftPosition(int power, long ticks)
{
}

void AsservDriver::setMotorRightPosition(int power, long ticks)
{
}

void AsservDriver::setMotorLeftPower(int power, int timems)
{
	power = power + 128;
	md25_.setSpeedReg(power, MD25_SPEED1_REG);
}

void AsservDriver::setMotorRightPower(int power, int timems)
{
	power = power + 128;
	md25_.setSpeedReg(power, MD25_SPEED2_REG);
}

void getCountsExternal(int* countR, int* countL){

}
void getDeltaCountsExternal(int* deltaR, int* deltaL){

}

long AsservDriver::getLeftExternalEncoder()
{
	return extLeftEncoder_.readCounter();
}
long AsservDriver::getRightExternalEncoder()
{
	long r = extRightEncoder_.readCounter();
	logger().error() << " r=" << r << logs::end;

	return r;
}

long AsservDriver::getLeftInternalEncoder()
{
//+/- 2,147,483,648

	return md25_.ensureGetEncoder(0, MD25_ENCODER1_REG);

}
long AsservDriver::getRightInternalEncoder()
{
	return md25_.ensureGetEncoder(0, MD25_ENCODER2_REG);
}

void AsservDriver::resetEncoders()
{
	md25_.resetEncoders();
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
	md25_.stopMotor(MD25_SPEED1_REG);
}
void AsservDriver::stopMotorRight()
{
	md25_.stopMotor(MD25_SPEED2_REG);
}

int AsservDriver::getMotorLeftCurrent()
{
	return 0;
}
int AsservDriver::getMotorRightCurrent()
{
	return 0;
}







float AsservDriver::odo_GetX_mm()
{
	return 0.0;
}
float AsservDriver::odo_GetY_mm()
{
}
float AsservDriver::odo_GetTheta_Rad()
{
}
float AsservDriver::odo_GetTheta_Degree()
{
}
void AsservDriver::odo_SetPosition(double x_m, double y_m, double angle_rad)
{
}
RobotPosition AsservDriver::odo_GetPosition()
{
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


TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm)
{
}
TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters)
{
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
void AsservDriver::motion_setLowSpeed(bool enable){
}
void AsservDriver::motion_ActivateReguDist(bool enable){
}
void AsservDriver::motion_ActivateReguAngle(bool enable){
}
void AsservDriver::motion_ResetReguDist(){
}
void AsservDriver::motion_ResetReguAngle(){
}
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_meters){
}
