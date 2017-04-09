/*!
 * \file
 * \brief Implémentation de la classe HostI2cBus.
 */

#include "HostI2cBus.hpp"

#include <as_devices/as_i2c.h>
#include <sstream>

HostI2cBus::HostI2cBus()
{
	int i2c_id = 1; //Bus I2C disponible sur la carte OPOS6UL (UART5).

	mDev = as_i2c_open(i2c_id);
	if (mDev == NULL)
	{
		std::cerr << "HostI2cBus::HostI2cBus() error : mDev is null !" << std::endl;
		//throw new I2cException("HostI2cBus::open, can't open i2c bus !");
	}
}

HostI2cBus::~HostI2cBus()
{
	int ret;

	if (mDev != NULL)
	{
		ret = as_i2c_close(mDev);
		if (ret < 0)
		{
			std::cerr << "HostI2cBus destruction error" << std::endl;
			//throw new I2cException("HostI2cBus destruction error !");
		}
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
	}
}

long HostI2cBus::setSlaveAddr(unsigned char aAddr)
{
	if (mDev != NULL)
	{
		return as_i2c_set_slave_addr(mDev, aAddr);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::getSlaveAddr() const
{
	if (mDev != NULL)
	{
		return as_i2c_get_slave_addr(mDev);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::read(unsigned char *aData, size_t aSize) const
{
	if (mDev != NULL)
	{
		return as_i2c_read(mDev, aData, aSize);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::write(unsigned char *aData, size_t aSize)
{
	if (mDev != NULL)
	{
		return as_i2c_write(mDev, aData, aSize);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::readReg(unsigned char aReg, unsigned char *aData, size_t aSize) const
{
	if (mDev != NULL)
	{
		return as_i2c_read_reg(mDev, aReg, aData, aSize);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::writeReg(unsigned char aReg, unsigned char *aData, size_t aSize)
{
	if (mDev != NULL)
	{
		return as_i2c_write_reg(mDev, aReg, aData, aSize);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::readMsg(unsigned char *aWData, unsigned char aWriteSize, unsigned char *aRData, size_t aReadSize)
{
	if (mDev != NULL)
	{
		return as_i2c_read_msg(mDev, aWData, aWriteSize, aRData, aReadSize);
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::readRegByte(unsigned char aReg) const
{
	if (mDev != NULL)
	{
		return as_i2c_read_reg_byte(mDev, aReg);
	}
	else
	{
		std::cerr << "HostI2cBus::readRegByte device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

long HostI2cBus::writeRegByte(unsigned char aReg, unsigned char aVal)
{
	if (mDev != NULL)
	{
		return as_i2c_write_reg_byte(mDev, aReg, aVal);
	}
	else
	{
		std::cerr << "HostI2cBus::writeRegByte device structure not allocated" << std::endl;
		//throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}

