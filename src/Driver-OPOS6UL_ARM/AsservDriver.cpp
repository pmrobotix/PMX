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
				, connected_(false)
{
	if (mbedI2c_.setSlaveAddr(MBED_ADDRESS) < 0) //0xAA>>1 = 0x55
	{
		printf("error setSlaveAddr");
	}
	if (mbed_ack() == 0)
	{
		connected_ = true;
	}
	else
	{
		logger().error() << "AsservDriver() : MBED is NOT CONNECTED !" << logs::end;
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

	//i2cExample();
	unsigned char cmd[12];
	mbed_readI2c('P', 12, cmd);
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

}
void AsservDriver::stopMotorRight()
{

}

int AsservDriver::getMotorLeftCurrent()
{
	return 0;
}
int AsservDriver::getMotorRightCurrent()
{
	return 0;
}

void AsservDriver::enableHardRegulation(bool enable)
{

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
void AsservDriver::odo_SetPosition(float x_m, float y_m, float angle_rad)
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
TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters)
{
}
TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
}
void AsservDriver::motion_FreeMotion(void)
{
}
void AsservDriver::motion_DisablePID(void)
{
}
void AsservDriver::motion_AssistedHandling(void)
{
}
void AsservDriver::motion_StopManager(void)
{
}



void AsservDriver::mbed_writeI2c()
{

}

void AsservDriver::mbed_readI2c(unsigned char command, unsigned char nbBytes2Read,
		unsigned char* cmd)
{

	//P12
	if (mbedI2c_.writeRegByte(command, nbBytes2Read) < 0)
	{
		printf("writeRegByte error!\n");
	}

	//Read the data back from the slave
	memset(cmd, 0, sizeof(cmd));
	if (mbedI2c_.read(cmd, sizeof(cmd)) < 0)
	{
		printf("Read error!\n");
	}
	float2bytes_t rx;
	rx.bytes[0] = cmd[0];
	rx.bytes[1] = cmd[1];
	rx.bytes[2] = cmd[2];
	rx.bytes[3] = cmd[3];

	float2bytes_t ry;
	ry.bytes[0] = cmd[4];
	ry.bytes[1] = cmd[5];
	ry.bytes[2] = cmd[6];
	ry.bytes[3] = cmd[7];

	float2bytes_t rt;
	rt.bytes[0] = cmd[8];
	rt.bytes[1] = cmd[9];
	rt.bytes[2] = cmd[10];
	rt.bytes[3] = cmd[11];
	printf("P12: %f %f %f\n", rx.f, ry.f, rt.f);
}

int AsservDriver::mbed_ack()
{
	unsigned char ack[1];

	//ACK
	memset(ack, 0, sizeof(ack));
	if (int r = mbedI2c_.read(ack, 1) < 0)
	{
		printf("ack1  error! %d\n", r);
		return -1;
	}
	printf("ack 0x%02hhX\n", ack[0]); // hh pour indiquer que c'est un char (pas int)
	if (ack[0] == MBED_ADDRESS)
		return 0;
	else
		return -1;
}

void AsservDriver::i2cExample()
{
	unsigned char cmd[12];
	unsigned char ack[1];

	//ACK
	memset(ack, 0, sizeof(ack));
	if (int r = mbedI2c_.read(ack, 1) < 0)
	{
		printf("ack1  error! %d\n", r);
	}
	printf("ack 0x%02hhX\n", *ack); // hh pour indiquer que c'est un char (pas int)

	float2bytes_t x;
	x.f = 100.1230f;
	float2bytes_t y;
	y.f = 99990000.0f;
	float2bytes_t t;
	t.f = 3.55221f;

	while (1)
	{

		//P12
		if (mbedI2c_.writeRegByte('P', 12) < 0)
		{
			printf("writeRegByte error!\n");
		}
		//Read the data back from the slave
		memset(cmd, 0, sizeof(cmd));
		if (mbedI2c_.read(cmd, sizeof(cmd)) < 0)
		{
			printf("Read error!\n");
		}
		float2bytes_t rx;
		rx.bytes[0] = cmd[0];
		rx.bytes[1] = cmd[1];
		rx.bytes[2] = cmd[2];
		rx.bytes[3] = cmd[3];

		float2bytes_t ry;
		ry.bytes[0] = cmd[4];
		ry.bytes[1] = cmd[5];
		ry.bytes[2] = cmd[6];
		ry.bytes[3] = cmd[7];

		float2bytes_t rt;
		rt.bytes[0] = cmd[8];
		rt.bytes[1] = cmd[9];
		rt.bytes[2] = cmd[10];
		rt.bytes[3] = cmd[11];
		printf("P12: %f %f %f\n", rx.f, ry.f, rt.f);

		//ACK
		memset(ack, 0, sizeof(ack));
		if (int r = mbedI2c_.read(ack, 1) < 0)
		{
			printf("ack1  error! %d\n", r);
		}
		printf("ack 0x%02hhX\n", *ack); // hh pour indiquer que c'est un char (pas int)

		//S12
		if (int r = mbedI2c_.writeRegByte('S', 12) < 0)
		{
			printf("S12 writeRegByte error %d !\n", r);
		}
		memset(cmd, 0, sizeof(cmd));
		printf("S12 %f %f %f \n", x.f, y.f, t.f);
		//printf("%d %d %d %d\n", x.bytes[0], x.bytes[1], x.bytes[2], x.bytes[3]);
		cmd[0] = x.bytes[0];
		cmd[1] = x.bytes[1];
		cmd[2] = x.bytes[2];
		cmd[3] = x.bytes[3];

		cmd[4] = y.bytes[0];
		cmd[5] = y.bytes[1];
		cmd[6] = y.bytes[2];
		cmd[7] = y.bytes[3];

		cmd[8] = t.bytes[0];
		cmd[9] = t.bytes[1];
		cmd[10] = t.bytes[2];
		cmd[11] = t.bytes[3];

		if (int r = mbedI2c_.write(cmd, sizeof(cmd)) < 0)
		{
			printf("S12.Write data error %d !\n", r);
		}

	}

}
