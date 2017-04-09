/*!
 * \file
 * \brief Définition de la classe HostI2cBus, gestion du bus i2c pour APF9328.
 */

#ifndef HOSTI2CBUS_HPP
#define HOSTI2CBUS_HPP

#include <stddef.h>
#include <iostream>
#include <string>

#include "../Log/Exception.hpp"
#include "../Thread/Mutex.hpp"


/*!
 * \brief Exception lancée s'il y a une erreur avec l'I2C.
 */
/*
class I2cException: public logs::Exception
{
public:
	I2cException(const std::string & message)
			: logs::Exception(message)
	{
		std::cout << "I2cException=====" << message << std::endl;
	}

	virtual ~ I2cException() throw ()
	{
	}
};*/

//new class using cpp
class HostI2cBus: public utils::Mutex
{

private:

protected:
	/**
	 * I2C bus device C structure
	 */
	mutable struct as_i2c_device *mDev;

public:
	/*!
	 * \brief Constructeur de la classe.
	 */
	HostI2cBus();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~HostI2cBus();

	/** @brief Set chip's I2C slave address
	 *
	 * @param aAddr		slave's address
	 *
	 * @return error if negative
	 */
	long setSlaveAddr(unsigned char aAddr);

	/** @brief Get chip's I2C slave address
	 *
	 * @return slave's address
	 */
	long getSlaveAddr() const;

	/** @brief Read several bytes (ioctl() method) from given chip.
	 *
	 * @param aData		read data
	 * @param aSize		data size
	 *
	 * @return error if negative
	 */
	long read(unsigned char *aData, size_t aSize) const;

	/** @brief Write several bytes (ioctl() method) to given chip.
	 *
	 * @param aData		data to write
	 * @param aSize		data size
	 *
	 * @return error if negative
	 */
	long write(unsigned char *aData, size_t aSize);

	/** @brief Read from given chip at a given register address (ioctl() method).
	 *
	 * @param aReg		register address
	 * @param aData		read data
	 * @param aSize		data size
	 *
	 * @return error if negative
	 */
	long readReg(unsigned char aReg, unsigned char *aData, size_t aSize) const;

	/** @brief Write to given chip at a given register address (ioctl() method).
	 *
	 * @param aReg		register address
	 * @param aData		data to write
	 * @param aSize		data size
	 *
	 * @return error if negative
	 */
	long writeReg(unsigned char aReg, unsigned char *aData, size_t aSize);

	/** @brief forge a read message like this:
	 * S Addr[W] wdata0 [A] wdata1 [A] ... RS Addr R [rdata0] A [rdata1] A ... P
	 *
	 * @param aWData	    data to write
	 * @param aWriteSize    written data size
	 * @param aRData	    read data
	 * @param aReadSize	    read data size
	 *
	 * @return error if negative
	 */
	long readMsg(unsigned char *aWData, unsigned char aWriteSize, unsigned char *aRData, size_t aReadSize);

	/** @brief Read a byte from the given register.
	 *
	 * @param aReg		register address
	 *
	 * @return read byte
	 */
	long readRegByte(unsigned char aReg) const;

	/** @brief Write a byte to the given register.
	 *
	 * @param aReg		register address
	 * @param aVal		byte to write
	 *
	 * @return error if negative
	 */
	long writeRegByte(unsigned char aReg, unsigned char aVal);

};

#endif
