/*!
 * \file
 * \brief Implémentation de la classe ServoMotorStd.
 */

#include "ServoMotorStd.hpp"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <fcntl.h>

#include "../Log/Logger.hpp"

//Definition de l'attribut de classe
int* ServoMotorStd::servoEnableBuffer_ = NULL;
int* ServoMotorStd::servoMinBuffer_ = NULL;
int* ServoMotorStd::servoMaxBuffer_ = NULL;
int* ServoMotorStd::servoOffsetBuffer_ = NULL;
int* ServoMotorStd::servoPositionBuffer_ = NULL;
int* ServoMotorStd::servoSpeedBuffer_ = NULL;
int* ServoMotorStd::servoInvertedBuffer_ = NULL;

ServoMotorStd::ServoMotorStd(int ID)
		: IDServo_(ID)
{

	if (IDServo_ < 0)
	{
		logger().error() << "ERROR servoID<0 " << IDServo_ << logs::end;
	}
	else if (IDServo_ >= NbMaxServo)
	{
		logger().error() << "ERROR servoID>NbMaxServo "
				<< IDServo_
				<< ">"
				<< NbMaxServo
				<< logs::end;
	}

	servoEnableBuffer_ = new int[NbMaxServo];
	servoMinBuffer_ = new int[NbMaxServo];
	servoMaxBuffer_ = new int[NbMaxServo];
	servoOffsetBuffer_ = new int[NbMaxServo];
	servoPositionBuffer_ = new int[NbMaxServo];
	servoSpeedBuffer_ = new int[NbMaxServo];
	servoInvertedBuffer_ = new int[NbMaxServo];
	for (int i = 0; i < NbMaxServo; i++)
	{
		servoEnableBuffer_[i] = 0;
		servoMinBuffer_[i] = 0;
		servoMaxBuffer_[i] = 0;
		servoOffsetBuffer_[i] = 0;
		servoPositionBuffer_[i] = 0;
		servoSpeedBuffer_[i] = 0;
		servoInvertedBuffer_[i] = 0;
	}

	//LEFT inverted
	//servoInvertedBuffer_[2] = 1;
	//servoInvertedBuffer_[6] = 1;
	//servoInvertedBuffer_[7] = 1;

	enableOpFileName_ = getFilename(SERVO_DRIVER_SERVO_ENABLE_FILE);
	logger().debug() << " ServoMotorStd  "
			<< " operationFileName="
			<< enableOpFileName_
			<< logs::end;
	offsetOpFileName_ = getFilename(SERVO_DRIVER_SERVO_OFFSET_FILE);
	positionOpFileName_ = getFilename(SERVO_DRIVER_SERVO_POSITION_FILE);
	currentPosOpFileName_ = getFilename(SERVO_DRIVER_SERVO_CURRENT_POS);
	speedOpFileName_ = getFilename(SERVO_DRIVER_SERVO_SPEED_FILE);
}

char* ServoMotorStd::getFilename(std::string type)
{
	char* name = new char[1024];
	memset(name, 0, 1024);
	sprintf(name,
			"%s%s%d/%s",
			SERVO_DRIVER_SYSFS_BASE,
			SERVO_DRIVER_SERVO_FILE,
			IDServo_,
			type.c_str());
	return name;
}

void ServoMotorStd::setServoEnable(int value)
{
	char data[16];
	int file = open(enableOpFileName_, O_WRONLY);
	if (file == -1)
	{
		logger().error() << "MotionServer setServoEnable: failed to open  "
				<< enableOpFileName_
				<< logs::end;
	}
	else
	{
		sprintf(data, "%d", value);
		int ret = write(file, data, strlen(data));
		if (ret == -1)
		{
			logger().error() << "MotionServer setServoEnable: failed to write in  "
					<< enableOpFileName_
					<< logs::end;
		}
		else
			servoEnableBuffer_[IDServo_] = value; //Keep the value
		close(file);
	}
}

void ServoMotorStd::setServoOffset(int value)
{
	if (servoInvertedBuffer_[IDServo_] == 1)
	{
		value = 4095 - value;
	}
	char data[16];
	int file = open(offsetOpFileName_, O_WRONLY);
	if (file == -1)
	{
		logger().error() << "MotionServer setServoOffset: failed to open  "
				<< offsetOpFileName_
				<< logs::end;
	}
	else
	{
		sprintf(data, "%d", value);
		int ret = write(file, data, strlen(data));
		if (ret == -1)
		{
			logger().error() << "MotionServer setServoOffset: failed to write in "
					<< offsetOpFileName_
					<< logs::end;
		}
		else
			servoOffsetBuffer_[IDServo_] = value;
		close(file);
	}
}

void ServoMotorStd::setServoPosition(int value)
{
	//logger().debug() << "setServoPosition " << IDServo_<< " inverted = " << servoInvertedBuffer_[IDServo_] << utils::end;
	if (servoInvertedBuffer_[IDServo_] == 1)
	{
		value = 4095 - value;
	}

	char data[16];
	int file = open(positionOpFileName_, O_WRONLY);
	if (file == -1)
	{
		logger().error() << "MotionServer setServoPosition: failed to open "
				<< positionOpFileName_
				<< logs::end;
	}
	else
	{
		sprintf(data, "%d", value);
		int ret = write(file, data, strlen(data));
		if (ret == -1)
		{
			logger().error() << "MotionServer setServoPosition: failed to write in "
					<< positionOpFileName_
					<< logs::end;
		}
		else
		{

			servoPositionBuffer_[IDServo_] = value;
		}
		close(file);
	}
}

void ServoMotorStd::setServoSpeed(int value)
{
	char data[16];
	int file = open(speedOpFileName_, O_WRONLY);
	if (file == -1)
	{
		logger().error() << "MotionServer setServoSpeed: failed to open "
				<< speedOpFileName_
				<< logs::end;
	}
	else
	{
		sprintf(data, "%d", value);
		int ret = write(file, data, strlen(data));
		if (ret == -1)
		{
			logger().error() << "MotionServer setServoSpeed: failed to write in "
					<< speedOpFileName_
					<< logs::end;
		}
		else
			servoSpeedBuffer_[IDServo_] = value;
		close(file);
	}
}

int ServoMotorStd::getServoId()
{
	int result = -1;
	if (IDServo_ < 0)
	{
		logger().error() << "ERROR servoID<0 " << IDServo_ << logs::end;
	}
	else if (IDServo_ >= NbMaxServo)
	{
		logger().error() << "ERROR servoID>NbMaxServo "
				<< IDServo_
				<< ">"
				<< NbMaxServo
				<< logs::end;
	}
	else
	{
		result = servoPositionBuffer_[IDServo_];
	}
	return result;
}

int ServoMotorStd::getServoCurrentPosition()
{
	//cat /sys/class/servo/servo0/current_position
	int result = -1;
	int intValue = 0;
	std::ifstream file(currentPosOpFileName_);
	if (file.is_open()) // ce test échoue si le fichier n'est pas ouvert
	{
		if (file.good())
		{
			std::string line;
			std::getline(file, line);
			intValue = atoi(line.c_str());
			logger().debug() << "OK getServoCurrentPosition : ok " << logs::end;
		}
		else
		{
			logger().error() << "ERROR getServoCurrentPosition : file not good in "
					<< currentPosOpFileName_
					<< logs::end;
		}
		file.close();
	}
	else
	{
		logger().error() << "ERROR getServoCurrentPosition : No file found in "
				<< currentPosOpFileName_
				<< logs::end;
	}
	result = intValue;

	if (servoInvertedBuffer_[IDServo_] == 1)
	{
		result = 4095 - result;
	}
	return result;
}

void ServoMotorStd::isInverted()
{
	logger().debug() << "B servoInvertedBuffer_["
			<< IDServo_
			<< "]="
			<< servoInvertedBuffer_[IDServo_]
			<< logs::end;

	servoInvertedBuffer_[IDServo_] = 1;

	for (int i = 0; i < NbMaxServo; i++)
	{
		logger().debug() << "A servoInvertedBuffer_["
				<< i
				<< "]="
				<< servoInvertedBuffer_[i]
				<< logs::end;
	}
}

