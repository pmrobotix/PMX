/*!
 * \file
 * \brief Implémentation de la classe HostI2cBus.
 */

#include "HostI2cBus.hpp"

#include <as_devices/as_i2c.h>
#include <sstream>

HostI2cBus::HostI2cBus()
{
	int i2c_id = 1; //Bus I2C disponible sur la carte OPOS6UL (UART5)

	mDev = as_i2c_open(i2c_id);
	if (mDev == NULL)
	{
		throw new I2cException("HostI2cBus::open, can't open i2c bus !");
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
			throw new I2cException("HostI2cBus destruction error !");
		}
	}
	else
	{
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		throw new I2cException("HostI2cBus device structure not allocated !");
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
		std::cerr << "HostI2cBus device structure not allocated" << std::endl;
		throw new I2cException("HostI2cBus device structure not allocated !");
		return -1;
	}
}



//______________________________________________

HostI2cBusOld::HostI2cBusOld()
		: device_(NULL), opened_(0)
{
}

int HostI2cBusOld::isOpened(void)
{
	return opened_;
}

void HostI2cBusOld::open(unsigned char slave_addr)
{
	if (opened_ == 0)
	{
		int i2c_id = 0; //Bus I2C sur la carte APF9328
		device_ = as_i2c_open(i2c_id);

		if (device_ == NULL)
		{
			throw new I2cException("Error HostI2cBus::open, can't open i2c port !");
		}
		else
		{
			opened_ = 1;
			setSlave(slave_addr);
		}
	}
	else //i2c already opened
	{
		throw new I2cWarning("Warning HostI2cBus::open, i2c bus already opened !");
	}
}

void HostI2cBusOld::close_i2c(void)
{
	if (opened_ == 1)
	{
		int ret = as_i2c_close(device_);

		if (ret < 0)
		{
			throw new I2cWarning("Error HostI2cBus::close, can't close I2C on close_i2c()!");
		}
		opened_ = 0;
	}
	else
	{
		//TODO en simulation, cette exception bloque !
		//throw new I2cWarning("Error HostI2cBus::close, i2c already closed !");
	}
}

void HostI2cBusOld::setSlave(unsigned char slaveAddr)
{
	if (opened_ == 1)
	{
		int err = as_i2c_set_slave_addr(device_, slaveAddr);
		if (err < 0)
		{
			std::ostringstream msg;
			msg << "Error HostI2cBus::setSlave, can't set slave n°"
					<< slaveAddr
					<< "("
					<< reinterpret_cast<void*>(slaveAddr)
					<< "), is the slave connected ?";
			throw new I2cException(msg.str());
		}
	}
	else
	{
		throw new I2cException("Error HostI2cBus::setSlave, i2c not opened !");
	}
}

int HostI2cBusOld::readRegValue(unsigned char, unsigned char reg, unsigned char* data)
{
	if (opened_ == 1)
	{
		lock();
		int ret = 0;
		ret = as_i2c_read_reg(device_, reg, data, 1);
		if (ret < 0)
		{
			if (ret == -1)
			{
				unlock();
				throw new I2cException("Error HostI2cBus::readRegValue, WRITE error !");
			}
			if (ret == -2)
			{
				unlock();
				throw new I2cException("Error HostI2cBus::readRegValue, READ error !");
			}
			unlock();
			throw new I2cException("Error HostI2cBus::readRegValue, i2c error !");
		}
		//usleep(2000); //1000us si fréq i2c à 100kHz : attente de l'application complète de la trame
		unlock();
		return ret;
	}
	else
	{
		throw new I2cException("Error HostI2cBus::readRegValue, i2c not opened !");
	}
}

int HostI2cBusOld::writeRegValue(unsigned char, unsigned char reg, unsigned char value)
{
	if (opened_ == 1)
	{
		lock();
		int result = 0;
		result = as_i2c_write_reg_byte(device_, reg, value);
		if (result < 0)
		{
			unlock();
			throw new I2cException("Error HostI2cBus::writeRegValue, i2c error !");
		}
		//usleep(2000); //fréq i2c à 100kHz : attente de l'application complète de la trame
		unlock();
		return result;
	}
	else
	{
		throw new I2cException("Error HostI2cBus::writeRegValue, i2c not opened !");
	}
}

int HostI2cBusOld::readI2cSize(unsigned char, char *buf, size_t size) //TODO remove uchar
{
	if (opened_ == 1)
	{
		lock();
		int ret = 0;
		ret = as_i2c_read(device_, (unsigned char*) buf, size);
		if (ret < 0)
		{
			unlock();
			throw new I2cException("Error HostI2cBus::readI2cSize, i2c read error !");
		}
		unlock();
		return ret;
	}
	else
	{
		throw new I2cException("Error HostI2cBus::readI2cSize, i2c not opened !");
	}
}

int HostI2cBusOld::writeI2cSize(unsigned char, const char *buf, size_t size) //TODO remove uchar
{
	if (opened_ == 1)
	{
		lock();
		int result = 0;
		result = as_i2c_write(device_, (unsigned char*) buf, size);
		if (result < 0)
		{
			unlock();
			throw new I2cException("Error HostI2cBus::writeI2cSize, i2c write error !");
		}
		unlock();
		return result;
	}
	else
	{
		throw new I2cException("Error HostI2cBus::writeI2cSize, i2c not opened !");
	}
}
