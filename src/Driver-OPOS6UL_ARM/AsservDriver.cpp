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
void AsservDriver::odo_SetPosition(double x_m, double y_m, float angle_rad)
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
	//printf("S12 %lf %lf %lf \n", x_m, y_m, angle_rad);
	//printf("S12 %lf %lf %lf \n", x_mm.f, y_mm.f, a.f);
	//printf("%d %d %d %d\n", x_mm.b[0], x_mm.b[1], x_mm.b[2], x_mm.b[3]);
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
	unsigned char data[12];

	if (int r = mbed_readI2c('P', 12, data) < 0)
	{
		logger().error() << "odo_GetPosition - P12 - ERROR " << r << logs::end;
		p.x = -1;
		p.y = -1;
		p.theta = -1;
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

		RobotPosition p;
		p.x = x_mm.f / 1000.0;
		p.y = y_mm.f / 1000.0;
		p.theta = rad.f;

		return p;
	}
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
void AsservDriver::motion_ManagerActivate(bool enable)
{

	if (enable)
	{
		mbed_writeI2c('I', 0, NULL);
	}
	else
	{
		mbed_writeI2c('!', 0, NULL);
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
	/*
	 //ACK
	 memset(ack, 0, sizeof(ack));
	 if (int r = mbedI2c_.read(ack, 1) < 0)
	 {
	 printf("ack1  error! %d\n", r);
	 }
	 printf("ack 0x%02hhX\n", *ack); // hh pour indiquer que c'est un char (pas int)
	 */
	float2bytes_t x;
	x.f = 100.1230f;
	float2bytes_t y;
	y.f = 99990000.0f;
	float2bytes_t t;
	t.f = 3.55221f;

	//while (1)
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
		rx.b[0] = cmd[0];
		rx.b[1] = cmd[1];
		rx.b[2] = cmd[2];
		rx.b[3] = cmd[3];

		float2bytes_t ry;
		ry.b[0] = cmd[4];
		ry.b[1] = cmd[5];
		ry.b[2] = cmd[6];
		ry.b[3] = cmd[7];

		float2bytes_t rt;
		rt.b[0] = cmd[8];
		rt.b[1] = cmd[9];
		rt.b[2] = cmd[10];
		rt.b[3] = cmd[11];
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
		cmd[0] = x.b[0];
		cmd[1] = x.b[1];
		cmd[2] = x.b[2];
		cmd[3] = x.b[3];

		cmd[4] = y.b[0];
		cmd[5] = y.b[1];
		cmd[6] = y.b[2];
		cmd[7] = y.b[3];

		cmd[8] = t.b[0];
		cmd[9] = t.b[1];
		cmd[10] = t.b[2];
		cmd[11] = t.b[3];

		if (int r = mbedI2c_.write(cmd, sizeof(cmd)) < 0)
		{
			printf("S12.Write data error %d !\n", r);
		}

	}

}
