//drivers...OPOS

#include "AsservDriver.hpp"

#include <cstdio>
#include <cstring>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create(std::string)
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver() :
		mbedI2c_(1) //UART5=>1 ; UART4=>0
				, connected_(false), asservStarted_(false)
{
	if (mbedI2c_.setSlaveAddr(MBED_ADDRESS) < 0) //0xAA>>1 = 0x55
	{
		//printf("error setSlaveAddr");
		logger().error() << "AsservDriver() : ERROR setSlaveAddr !" << logs::end;
		connected_ = false;
	}
	else
	{

		if (mbed_ack() == 0)
		{
			connected_ = true;
			asservStarted_ = true;
		}
		else
		{
			logger().error() << "AsservDriver() : MBED is NOT CONNECTED !" << logs::end;
		}
	}
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
	if (!connected_) return;

}

void AsservDriver::setMotorRightPower(int power, int timems)
{

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
	return 0;
}
long AsservDriver::getRightInternalEncoder()
{
	return 0;
}

void AsservDriver::resetEncoders()
{

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
	//TODO
}
void AsservDriver::stopMotorRight()
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

//deprecated
//void AsservDriver::enableHardRegulation(bool enable) //legoEV3 configuration to be removed
//{
//}
/*
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
 }*/
void AsservDriver::odo_SetPosition(double x_m, double y_m, double angle_rad)
{
	if (!connected_) return;
	//P12
	unsigned char d[12];
	float2bytes_t x_mm;
	x_mm.f = x_m * 1000.0f;
	float2bytes_t y_mm;
	y_mm.f = y_m * 1000.0f;
	float2bytes_t a;
	a.f = angle_rad;

	d[0] = x_mm.b[0];
	d[1] = x_mm.b[1];
	d[2] = x_mm.b[2];
	d[3] = x_mm.b[3];

	d[4] = y_mm.b[0];
	d[5] = y_mm.b[1];
	d[6] = y_mm.b[2];
	d[7] = y_mm.b[3];

	d[8] = a.b[0];
	d[9] = a.b[1];
	d[10] = a.b[2];
	d[11] = a.b[3];
	if (int r = mbed_writeI2c('S', 12, d) < 0)
	{
		logger().error() << "odo_SetPosition - S12 - ERROR " << r << logs::end;
	}
	else
		logger().info() << "odo_SetPosition S12 " << x_mm.f << " " << y_mm.f << " " << a.f
				<< logs::end;

}
RobotPosition AsservDriver::odo_GetPosition() //en metre
{
	RobotPosition p;
	p.x = -1;
	p.y = -1;
	p.theta = -1;
	if (!connected_) return p;

	unsigned char data[12];

	if (int r = mbed_readI2c('P', 12, data) < 0)
	{
		logger().error() << "odo_GetPosition - P12 - ERROR " << r << logs::end;

		return p;
	}
	else
	{
		//printf("read %d %d %d %d\n", data[0], data[1], data[2], data[3]);
		float2bytes_t x_mm;
		x_mm.b[0] = data[0];
		x_mm.b[1] = data[1];
		x_mm.b[2] = data[2];
		x_mm.b[3] = data[3];

		float2bytes_t y_mm;
		y_mm.b[0] = data[4];
		y_mm.b[1] = data[5];
		y_mm.b[2] = data[6];
		y_mm.b[3] = data[7];

		float2bytes_t rad;
		rad.b[0] = data[8];
		rad.b[1] = data[9];
		rad.b[2] = data[10];
		rad.b[3] = data[11];

		//printf("P12: %f %f %f\n", x_mm.f, y_mm.f, rad.f);
		logger().info() << "odo_GetPosition P12 " << x_mm.f << " " << y_mm.f << " " << rad.f
				<< logs::end;

		RobotPosition p; //in m
		p.x = x_mm.f / 1000.0;
		p.y = y_mm.f / 1000.0;
		p.theta = rad.f;

		return p;
	}
}
int AsservDriver::path_GetLastCommandStatus()
{
	//TODO
	return 0;
}
void AsservDriver::path_InterruptTrajectory()
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "path_InterruptTrajectory() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('h', 0, NULL);
}
void AsservDriver::path_CollisionOnTrajectory()
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "path_CollisionOnTrajectory() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('h', 0, NULL);
}
void AsservDriver::path_CollisionRearOnTrajectory()
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "path_CollisionRearOnTrajectory() ERROR MBED NOT STARTED "
				<< asservStarted_ << logs::end;
	else
		mbed_writeI2c('h', 0, NULL);
}
void AsservDriver::path_CancelTrajectory()
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "path_CancelTrajectory() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('h', 0, NULL);
}
void AsservDriver::path_ResetEmergencyStop()
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "path_ResetEmergencyStop() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('r', 0, NULL);
}
TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters) //v4 +d
{
	if (!connected_) return TRAJ_ERROR;
	if (!asservStarted_)
	{
		logger().error() << "motion_DoLine() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
		return TRAJ_ERROR;
	}
	else
	{
		unsigned char d[4];
		float2bytes_t mm;
		mm.f = (dist_meters * 1000.0);
		d[0] = mm.b[0];
		d[1] = mm.b[1];
		d[2] = mm.b[2];
		d[3] = mm.b[3];
		mbed_writeI2c('v', 4, d);
		return TRAJ_OK;
	}
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm) // f8 +x+y
{
	if (!connected_) return TRAJ_ERROR;
	if (!asservStarted_)
	{
		logger().error() << "motion_DoFace() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
		return TRAJ_ERROR;
	}
	else
	{
		unsigned char d[8];
		float2bytes_t x, y;
		x.f = x_mm;
		d[0] = x.b[0];
		d[1] = x.b[1];
		d[2] = x.b[2];
		d[3] = x.b[3];
		y.f = y_mm;
		d[4] = y.b[0];
		d[5] = y.b[1];
		d[6] = y.b[2];
		d[7] = y.b[3];
		mbed_writeI2c('f', 8, d);
		return TRAJ_OK;
	}
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_degrees) //t4 +d
{
	if (!connected_) return TRAJ_ERROR;
	if (!asservStarted_)
	{
		logger().error() << "motion_DoRotate() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
		return TRAJ_ERROR;
	}
	else
	{
		unsigned char d[4];
		float2bytes_t deg;
		deg.f = angle_degrees;
		d[0] = deg.b[0];
		d[1] = deg.b[1];
		d[2] = deg.b[2];
		d[3] = deg.b[3];
		mbed_writeI2c('t', 4, d);
		return TRAJ_OK;
	}
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
//TODO motion_DoArcRotate
	return TRAJ_ERROR;
}
void AsservDriver::motion_FreeMotion(void)
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "motion_FreeMotion() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('K', 0, NULL);
}
void AsservDriver::motion_DisablePID() //TODO mm chose que Freemotion ???
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "motion_DisablePID() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('K', 0, NULL);
}
void AsservDriver::motion_AssistedHandling(void)
{
	if (!connected_) return;
	if (!asservStarted_)
		logger().error() << "motion_AssistedHandling() ERROR MBED NOT STARTED " << asservStarted_
				<< logs::end;
	else
		mbed_writeI2c('J', 0, NULL);
}
void AsservDriver::motion_ActivateManager(bool enable)
{
	if (!connected_) return;
	if (enable)
	{
		mbed_writeI2c('I', 0, NULL);
		asservStarted_ = true;
	}
	else
	{
		mbed_writeI2c('!', 0, NULL);
		asservStarted_ = false;
	}
}

//------------------------------------------------------------------------

int AsservDriver::mbed_writeI2c(unsigned char cmd, unsigned char nbBytes2Write,
		unsigned char * data)
{
	if (int r = mbedI2c_.writeRegByte(cmd, nbBytes2Write) < 0)
	{
		printf("ERROR mbed_writeI2c > writeRegByte > %c%d > %d!\n", cmd, nbBytes2Write, r);
		return -1;
	}
	if (nbBytes2Write != 0) if (int r = mbedI2c_.write(data, nbBytes2Write) < 0)
	{
		printf("ERROR mbed_writeI2c > write > %c%d > %d!\n", cmd, nbBytes2Write, r);
		return -1;
	}
	return 0;
}

int AsservDriver::mbed_readI2c(unsigned char command, unsigned char nbBytes2Read,
		unsigned char* data)
{
	if (mbedI2c_.writeRegByte(command, nbBytes2Read) < 0)
	{
		printf("mbed_readI2c > writeRegByte > %c%d > error!\n", command, nbBytes2Read);
		return -1;
	}

//Read the data back from the slave
	if (mbedI2c_.read(data, nbBytes2Read) < 0)
	{
		printf("mbed_readI2c > read > %c%d > error!\n", command, nbBytes2Read);
		return -1;
	}
	return 0;
}

int AsservDriver::mbed_ack()
{
	unsigned char ack[1];

//ACK
	memset(ack, 0, sizeof(ack));
	if (int r = mbedI2c_.read(ack, 1) < 0)
	{
		printf("ack1 read error! %d\n", r);
		return -1;
	}
//printf("mbed_ack 0x%02hhX\n", ack[0]); // hh pour indiquer que c'est un char (pas int)
	if (ack[0] == MBED_ADDRESS)
		return 0;
	else
		return -1;
}

