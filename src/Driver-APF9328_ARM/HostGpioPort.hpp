/*!
 * \file
 * \brief Définition de la classe HostGpioPort, gestion des ports gpio pour la carte APF9328.
 */

#ifndef HOSTGPIOPORT_H_
#define HOSTGPIOPORT_H_

#include <string>

#include "../Log/Exception.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/ppdev.h>

/*
struct as_gpio_device;


#define IN (char*)0
#define OUT (char*)1
*/

/*!
 * \brief Exception lancée s'il y a une erreur avec les GPIO.
 */
class HostGpioException: public logs::Exception
{
public:
	HostGpioException(const std::string & message)
			: Exception(message)
	{
	}

	virtual ~ HostGpioException() throw ()
	{
	}
};

class HostGpioPort
{
private:
	struct as_gpio_device *device_;
	int fd_gpio_; //pour ioctl
	char port_letter_;
	int pin_number_;
	static int* portA_opened_;
	static int* portB_opened_;
	static int* portC_opened_;
	static int* portD_opened_;

public:
	/*!
	 * \brief Constructeur de la classe.
	 */
	HostGpioPort();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~HostGpioPort();


	//version as_devices
	//void openAs(int pinNum);//char portLetter,
	//void closeAs();

	/*!
	 * \brief Set the GPIO Direction by AsDevices.
	 * \param aDirection (0:in, 1:out)
	 */
	//void setDirectionAs(int aDirection);
	//void setValueAs(int aValue);



	//version ioctl
	void openIoctl(char portLetter, int pinNum);
	void closeIoctl();

	/*!
	 * \brief Set the GPIO Direction by ioctl.
	 * \param aDirection (0:in, 1:out)
	 */
	void setDirIoctl(int aDirection);
	void setValueIoctl(bool aValue);

private:

	void checkIf(int value);
	void setData(int value);

};


#endif
