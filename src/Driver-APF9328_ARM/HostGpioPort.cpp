/*!
 * \file
 * \brief Implémentation de la classe GpioPort.
 */

#include "HostGpioPort.hpp"

#include <as_devices/as_gpio.h>
#include <string.h>
#include <iostream>
#include <sstream>

#define GPIORDDIRECTION	_IOR(PP_IOCTL, 0xF0, int)
#define GPIOWRDIRECTION	_IOW(PP_IOCTL, 0xF1, int)
#define GPIORDDATA	_IOR(PP_IOCTL, 0xF2, int)
#define GPIOWRDATA	_IOW(PP_IOCTL, 0xF3, int)
#define GPIORDMODE	_IOR(PP_IOCTL, 0xF4, int)
#define GPIOWRMODE	_IOW(PP_IOCTL, 0xF5, int)

//Definition de l'attribut de classe
int* HostGpioPort::portA_opened_ = NULL;
int* HostGpioPort::portB_opened_ = NULL;
int* HostGpioPort::portC_opened_ = NULL;
int* HostGpioPort::portD_opened_ = NULL;

HostGpioPort::HostGpioPort()
		: device_(NULL), fd_gpio_(0), port_letter_(0), pin_number_(0)
{
	if (HostGpioPort::portA_opened_ == NULL)
	{
		HostGpioPort::portA_opened_ = (int *) malloc(32 * sizeof(int));
		memset(HostGpioPort::portA_opened_, 0, 32 * sizeof(int));
	}
	if (HostGpioPort::portB_opened_ == NULL)
	{
		HostGpioPort::portB_opened_ = (int *) malloc(32 * sizeof(int));
		memset(HostGpioPort::portB_opened_, 0, 32 * sizeof(int));
	}
	if (HostGpioPort::portC_opened_ == NULL)
	{
		HostGpioPort::portC_opened_ = (int *) malloc(32 * sizeof(int));
		memset(HostGpioPort::portC_opened_, 0, 32 * sizeof(int));
	}
	if (HostGpioPort::portD_opened_ == NULL)
	{
		HostGpioPort::portD_opened_ = (int *) malloc(32 * sizeof(int));
		memset(HostGpioPort::portD_opened_, 0, 32 * sizeof(int));
	}
}

HostGpioPort::~HostGpioPort()
{
}

/*
//TODO
void HostGpioPort::openAs(int aGpioNum) //char portLetter, int pinNum
{

	char aPortChar;
	int aPinNum;

	aPortChar = (aGpioNum / 32) + 'A';
	aPinNum = aGpioNum - (aPortChar - 'A');

	std::cout << "aPortChar=" << aPortChar << " aPinNum=" << aPinNum;
	port_letter_ = aPortChar;
	pin_number_ = aPinNum;

	checkIf(1); //check if gpio is not opened

	//device_ = as_gpio_open(portLetter, pinNum);
	device_ = as_gpio_open(4);
	if (device_ == NULL)
	{
		throw new HostGpioException("Error gpio openAs, can't open gpio port. Have you run loadgpio.sh ?");
	}
	else
	{
		setData(1);
	}
}

//TODO
void HostGpioPort::closeAs(void)
{
	checkIf(0); //check if gpio is opened

	int ret = as_gpio_close(device_);
	if (ret < 0)
	{
		throw new HostGpioException("Error gpio closeAs, can't close gpio");
	}
	setData(0);
}

//TODO
void HostGpioPort::setDirectionAs(int aDirection)
{
	checkIf(0); //check if gpio is opened

	char *dir = NULL;
	*dir = '1'; //todo
	int ret = as_gpio_set_pin_direction(device_, dir);
	if (ret < 0)
	{
		std::ostringstream msg;
		msg << "Error gpio setDirectionAs, can't change direction on " << port_letter_ << pin_number_ << " !";
		throw new HostGpioException(msg.str());
	}
}

//TODO
void HostGpioPort::setValueAs(int aValue)
{
	checkIf(0); //check if gpio is opened

	int ret = as_gpio_set_pin_value(device_, aValue);
	if (ret < 0)
	{
		throw new HostGpioException("Error gpio setValueAs, can't change pin value");
	}
}
*/


void HostGpioPort::checkIf(int value)
{
	std::ostringstream msg;
	int err = 0;
	switch (port_letter_)
	{
	case 'A':
		if (HostGpioPort::portA_opened_[pin_number_] == value)
		{
			err = 1;
		}
		break;
	case 'B':
		if (HostGpioPort::portB_opened_[pin_number_] == value)
		{
			err = 1;
		}
		break;
	case 'C':
		if (HostGpioPort::portC_opened_[pin_number_] == value)
		{
			err = 1;
		}
		break;
	case 'D':
		if (HostGpioPort::portD_opened_[pin_number_] == value)
		{
			err = 1;
		}
		break;
	default:
		throw new HostGpioException("--Error HostGpioPort check, port not yet defined");
	}
	if (err == 1)
	{
		if (value == 0)
			msg << "Error gpio " << port_letter_ << pin_number_ << " not opened !";
		else
			msg << "Error gpio " << port_letter_ << pin_number_ << " already opened !";
		throw new HostGpioException(msg.str());
	}
}

void HostGpioPort::setData(int value)
{
	switch (port_letter_)
	{
	case 'A':
		HostGpioPort::portA_opened_[pin_number_] = value;
		break;
	case 'B':
		HostGpioPort::portB_opened_[pin_number_] = value;
		break;
	case 'C':
		HostGpioPort::portC_opened_[pin_number_] = value;
		break;
	case 'D':
		HostGpioPort::portD_opened_[pin_number_] = value;
		break;
	default:
		throw new HostGpioException("---Error gpio setData, port not yet defined");
	}
}




void HostGpioPort::openIoctl(char portLetter, int pinNum)
{
	port_letter_ = portLetter;
	pin_number_ = pinNum;
	checkIf(1); //check if gpio is not opened
	std::ostringstream oss;
	oss << "/dev/gpio/P" << portLetter << pinNum;
	std::string result = oss.str();
	const char* cchar = result.c_str();
	if ((fd_gpio_ = open(cchar, O_RDWR)) < 0)
	{
		throw new HostGpioException("ERROR gpio openIoctl, can't open gpio port. Have you run loadgpio.sh ?");
	}
	//Set LED PIN as GPIO mode;
	int portval = 1;
	int err = ioctl(fd_gpio_, GPIOWRMODE, &portval);
	if (err < 0)
	{
		throw new HostGpioException("ERROR gpio openIoctl, unable to set gpio mode !");
	}
	setData(1); //gpio opened
}

void HostGpioPort::closeIoctl(void)
{
	checkIf(0); //check if gpio is opened
	close(fd_gpio_);
	setData(0); //close gpio
}

void HostGpioPort::setDirIoctl(int aDirection)
{
	checkIf(0); //check if gpio is opened

	if (aDirection != 0 and aDirection != 1)
	{
		throw new HostGpioException("Error gpio setDirIoctl, bad aDirection !");
	}
	int err = ioctl(fd_gpio_, GPIOWRDIRECTION, &aDirection); //OUT => 1 IN =>0
	if (err < 0)
	{
		throw new HostGpioException("ERROR setDirIoctl, unable to set gpio direction !");
	}
}

void HostGpioPort::setValueIoctl(bool aValue)
{
	checkIf(0); //check if gpio is opened

	int portval = 1;
	if (aValue == false)
		portval = 0;
	else
		portval = 1;
//printf("portval=%d fd_gpio_=%d port_letter_=%c pin_number_=%d\n", portval, fd_gpio_, port_letter_, pin_number_);
	int err = ioctl(fd_gpio_, GPIOWRDATA, &portval);
	if (err < 0)
	{
		throw new HostGpioException("ERROR gpio setValueIoctl, unable to set gpio value !");
	}
}
