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

class AsI2c;




/*!
 * \brief Exception lancée s'il y a une erreur avec l'I2C.
 */
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
};

class I2cWarning: public logs::Exception
{
public:
	I2cWarning(const std::string & message)
			: logs::Exception(message)
	{
	}

	virtual ~ I2cWarning() throw ()
	{
	}
};



//new class using cpp
class HostI2cBus: public utils::Mutex
{

private:


	/*!
	 * \brief Stored i2c parameters.
	 */
	//struct as_i2c_device *device_;
	static AsI2c *asI2c;

	/*!
	 * \brief true if the i2c is opened.
	 */
	int opened_;

public:
	/*!
	 * \brief Constructeur de la classe.
	 */
	HostI2cBus();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~HostI2cBus()
	{
	}

	long setSlaveAddr(unsigned char aAddr);

	long getSlaveAddr() const;

	long read(unsigned char *aData, size_t aSize) const;

	long write(unsigned char *aData, size_t aSize);

	long readReg(unsigned char aReg, unsigned char *aData, size_t aSize) const;

	long writeReg(unsigned char aReg, unsigned char *aData, size_t aSize);

	long readMsg(unsigned char *aWData, unsigned char aWriteSize, unsigned char *aRData, size_t aReadSize);

	long readRegByte(unsigned char aReg) const;

	long writeRegByte(unsigned char aReg, unsigned char aVal);





};





class HostI2cBusOld: public utils::Mutex
{

private:
	/*!
	 * \brief Stored i2c parameters.
	 */
	struct as_i2c_device *device_;

	/*!
	 * \brief true if the i2c is opened.
	 */
	int opened_;

public:
	/*!
	 * \brief Constructeur de la classe.
	 */
	HostI2cBusOld();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~HostI2cBusOld()
	{
		try
		{
			close_i2c();
		} catch (logs::Exception * e)
		{

			std::cout << "HostI2cBusOld()::Exception - close_i2c :" << e->what()
					<< std::endl;
		}
	}

	int isOpened(void);

	/*!
	 * \brief Read a byte from the given register.
	 *
	 * \param   reg   the given register.
	 * \param   data   the read value.
	 *
	 * \return return 0 on success, -1 on write error (\e reg byte), -2 on read error.
	 */
	int readRegValue(unsigned char  slave_addr, unsigned char  reg, unsigned char * data);

	/*!
	 * \brief Write a byte to the I2C given register.
	 *
	 * \param   reg   register to write.
	 * \param   value   value to apply.
	 *
	 * \return 0 on success, -1 on error.
	 */
	int writeRegValue(unsigned char  slave_addr, unsigned char  reg, unsigned char  value);

	int readI2cSize(unsigned char  slave_addr, char *buf, size_t size);

	int writeI2cSize(unsigned char  slave_addr, const char *buf, size_t size);

	/*!
	 * \brief Read 4 bytes from the given register.
	 *
	 * \param   reg   the given register.
	 * \param   data   the read value.
	 *
	 * \return return 0 on success, -1 on write error (\e reg byte), -2 on read error.
	 */
//int readReg_nValue(uchar reg, uchar* data, size_t size);
	/*!
	 * \brief Open i2c.
	 */
	void open(unsigned char  slave_addr);

	/*!
	 * \brief Close i2c.
	 */
	void close_i2c();

private:
	/*!
	 * \brief Set the slave by his address.
	 * \param   slaveAddr	slave address.
	 */
	void setSlave(unsigned char  slaveAddr);

};


#endif
