/*!
 * \file
 * \brief Définition de la classe HostSpiBus, gestion du port SPI pour la carte APF9328.
 */

#ifndef HOSTSPIBUS_HPP
#define HOSTSPIBUS_HPP

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <string>

#include "../Log/Exception.hpp"
#include "../Thread/Mutex.hpp"

#define CLOCKSPEED 6000000
#define APF9328_SPI "/dev/spidev1.1"

/*!
 * \brief Exception lancée s'il y a une erreur avec le bus SPI.
 */
class HostSpiException: public logs::Exception
{
public:
	HostSpiException(const std::string & message)
			: Exception(message)
	{
		std::cout << "HostSpiException=====" << message << std::endl;
	}

	virtual ~ HostSpiException() throw ()
	{
	}
};

class HostSpiBus: utils::Mutex
{
public:

	/*!
	 * \brief Cette méthode statique retourne l'instance unique de la classe HostSpiBus.
	 * \return L'instance unique de la classe.
	 *
	 */
	static HostSpiBus & instance()
	{
		static HostSpiBus instance;
		return instance;
	}

private:

	int fd_spi_;

	/*!
	 * \brief true if the SPI bus is opened.
	 */
	int opened_;

private:
	/*!
	 * \brief Constructeur de la classe.
	 */
	HostSpiBus();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~HostSpiBus()
	{
	}

public:
	/*!
	 * \brief Method for talking to IC over SPI.
	 * Used for transferring all data along the SPI
	 */
	unsigned long long spiTransfer(char data);

	/*!
	 * \brief Open SPI.
	 */
	void open();

	/*!
	 * \brief Close SPI.
	 */
	void close();

	/*!
	 * \brief Get SPI bus speed.
	 */
	long getSpeed();

	unsigned long long as_spi_msg_pmx(int aFd,
			unsigned long long aMsg,
			size_t aLen,
			uint32_t aSpeed);

};

#endif
