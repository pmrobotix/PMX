/*!
 * \file
 * \brief Implémentation de la classe HostI2cBus.
 */

#include "HostI2cBus.hpp"

#include <as_devices/as_i2c.h>
#include <sstream>



void HostI2cBus::open(unsigned char slave_addr)
{

}


void HostI2cBus::setSlave(unsigned char slaveAddr)
{

}
















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
			msg << "Error HostI2cBus::setSlave, can't set slave n°" << slaveAddr << "("
					<< reinterpret_cast<void*>(slaveAddr) << "), is the slave connected ?";
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
