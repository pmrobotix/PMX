/*!
 * \file
 * \brief Implémentation de la classe HostSpiBus.
 */

#include "HostSpiBus.hpp"

#include <as_devices/as_spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <cstring>

HostSpiBus::HostSpiBus()
		: fd_spi_(0), opened_(0)
{
}

void HostSpiBus::open(void)
{
	const char *aSpidev_name = APF9328_SPI;
	unsigned char *d = (unsigned char*) aSpidev_name;
	fd_spi_ = as_spi_open(d);
	if (fd_spi_ < 0)

	{
		throw new HostSpiException("Error HostSpiException::open, can't open SPI \nHave you run modprobe spidev ?");
	}
}

void HostSpiBus::close(void)
{
	as_spi_close(fd_spi_);
}

long HostSpiBus::getSpeed(void)
{
	long speed = as_spi_get_speed(fd_spi_);
	return speed;
}

unsigned long long HostSpiBus::spiTransfer(char data)
{
	//unsigned long long result =0;
	unsigned long long result = as_spi_msg(fd_spi_, (unsigned long long) data, 8, CLOCKSPEED);
	return result;
}

unsigned long long HostSpiBus::as_spi_msg_pmx(int aFd,
		unsigned long long aMsg,
		size_t aLen,
		uint32_t aSpeed)
{
	uint32_t msg;
	// int len;
	size_t len;

	struct spi_ioc_transfer xfer[1];
	unsigned char buf[64];
	unsigned char buf_read[64];

	int status = 0;

	msg = aMsg;
	len = aLen;

	memset(xfer, 0, sizeof(xfer));
	memset(buf, 0, sizeof(buf));
	memset(buf_read, 0, sizeof(buf_read));

	if (len >= sizeof(buf))
	{
		len = sizeof(buf) - 1;
	}

	for (int i = len; i > 0; i--)
	{
		buf[i - 1] = msg & 0x01;
		msg = msg >> 1;
	}

	xfer[0].tx_buf = (__u64 ) buf;
	xfer[0].len = len;
	xfer[0].rx_buf = (__u64 ) buf_read;
	xfer[0].speed_hz = aSpeed;
	xfer[0].bits_per_word = 1;

	status = ioctl(aFd, SPI_IOC_MESSAGE(1), xfer);
	if (status < 0)
	{
		//ERROR("SPI_IOC_MESSAGE");
		throw new HostSpiException("Error HostSpiException::as_spi_msg_pmx, SPI_IOC_MESSAGE");
		return 0;
	}

	msg = msg | buf_read[0];
	for (int i = 1; i < (int) len; i++)
	{
		msg = msg << 1;
		msg = msg | buf_read[i];
	}
	return msg;
}
