/*!
 * \file
 * \brief Implémentation de la classe ExtEncoder.
 * Interface library for chip LS7366R that does quadrature decoding of rotary encoders.
 */

#include "ExtEncoder.hpp"

#include <unistd.h>

#include "HostSpiBus.hpp"

/*!
 * External Encoder constructor.
 */
ExtEncoder::ExtEncoder(char slave_select_port_letter, int slave_select_pin)
		: ss_port_(slave_select_port_letter), ss_pin_(slave_select_pin)
{
	try
	{
		/*
		gpio_ = new HostGpioPort();

		logger().debug() << "ss_port_=" << ss_port_ << " ss_pin_=" << ss_pin_ << logs::end;

		gpio_->openIoctl(ss_port_, ss_pin_);
		gpio_->setDirIoctl(1);

		this->counterSize = 4; //n-byte counter
		this->prevStatus = 0; //status register from previous read
		this->prevCount = 0; //count from previous read
		this->prevDiff = 0; //difference in coundt from previous read

		ss_pin_set(1);

		HostSpiBus::instance().open();
		long speed = HostSpiBus::instance().getSpeed();
		logger().debug() << " Encoder spi speed = " << speed << logs::end;

		//singleByteWrite(WRITE_MDR0, QUADRX4|FREE_RUN|INDX_LOADC|SYNCH_INDX|FILTER_2);
		//singleByteWrite(WRITE_MDR1, IDX_FLAG|CMP_FLAG|BYTE_2|EN_CNTR);

		//init MDR0  and MDR1
		int mdr0 = QUADRX1 | FREE_RUN | DISABLE_INDX | DISABLE_INDX | ASYNCH_INDX | FILTER_1;
		int mdr1 = NO_FLAGS | EN_CNTR | BYTE_4;
		logger().debug() << "MDR0="
				<< reinterpret_cast<void*>(mdr0)
				<< "\t MDR1="
				<< reinterpret_cast<void*>(mdr1)
				<< logs::end;
		//logger().error() << "RESETING ENCODER" << logs::end;
		this->setup(mdr0, mdr1);
		logger().error() << "RESETING ENCODER to " << readCounter() << logs::end;
		*/
	} catch (logs::Exception * e)
	{
		logger().error() << "Exception ExtEncoder::ExtEncoder: " << e->what() << logs::end;
	}
	usleep(1000);

}

void ExtEncoder::ss_pin_set(int value)
{/*
	try
	{
		gpio_->setValueIoctl(value);
	} catch (logs::Exception * e)
	{
		logger().error() << "Exception ExtEncoder::ss_pin_set: " << e->what() << logs::end;
	}*/
}

/*!
 Initialize the encoder to the SPI with your own desired parameters
 */
void ExtEncoder::setup(int setup_mdr0, int setup_mdr1)
{/*
	this->counterSize = (4 - (((BYTE) setup_mdr1) & 0x03)); //n-byte counter

	logger().error() << "counterSize= " << reinterpret_cast<void*>(counterSize) << logs::end;

	lock();
	ss_pin_set(1);
	ss_pin_set(0);
	this->spiTransfer(WRITE_MODE0);
	this->spiTransfer((char) setup_mdr0);
	ss_pin_set(1);
	usleep(100);

	ss_pin_set(1);
	ss_pin_set(0);
	this->spiTransfer(WRITE_MODE1);
	this->spiTransfer((char) setup_mdr1);
	ss_pin_set(1);
	unlock();

	//Clear LS7366
	this->clearCounter(); //clear counter
	//this->clearStatus(); //clear status
*/
}

/*!
 Used for transferring all data along the SPI
 */
unsigned long long ExtEncoder::spiTransfer(char data)
{
	unsigned long long result = HostSpiBus::instance().spiTransfer(data);
	return result;
}

/*!
 Used for clearing the counter
 */
void ExtEncoder::clearCounter(void)
{
//printf("ExtEncoder::clearCounter\n");
	/*
	lock();
	ss_pin_set(1);
	ss_pin_set(0); //enable device
	this->spiTransfer(CLEAR_COUNTER); //transmit clear opcode
	ss_pin_set(1); //release device
	unlock();*/
}

/*!
 Used for clearing the Status
 */
void ExtEncoder::clearStatus(void)
{/*
	lock();
	ss_pin_set(1);
	ss_pin_set(0); //enable device
	this->spiTransfer(CLEAR_STATUS); //transmit clear opcode
	ss_pin_set(1); //release device
	unlock();*/
}

/*!
 Used for reading the counter
 */
long long ExtEncoder::readCounter(void)
{
	readStatus(); //read status before read otherwise it doesn't work
	long long counter = 0;
	unsigned long long data = 0;
	unsigned long long fulldata = 0;
unsigned char cmd = 0x00;

	lock();
	ss_pin_set(1);
	ss_pin_set(0);


	/*
	this->spiTransfer(READ_COUNTER);

	for (int i = (int) this->counterSize; i > 0; i--)
	{
		data = this->spiTransfer(0x00);
		fulldata = fulldata * 255 + data;
	}*/


		cmd = spiTransfer(cmd); // fake read
		counter |= spiTransfer(cmd) << 24;
		counter |= spiTransfer(cmd) << 16;
		counter |= spiTransfer(cmd) << 8;
		counter |= spiTransfer(cmd);




	ss_pin_set(1); //release device
	unlock();
//printf("ExtEncoder::readCounter fulldata : %lld\n", fulldata);
//logger().debug() << "fulldata =  " << fulldata << utils::end;
/*
	if (fulldata > 4244897280 / 2)
	{
		counter = -(((long long) 4244897280) - fulldata);
	}
	else
	{
		counter = fulldata;
	}
	*/

//printf("ExtEncoder::readCounter : %lld\n", counter);
//logger().error() << "counter"<<ss_pin_<<"=" << counter << utils::end;
	return counter;
}

/*!
 Used for reading the status
 */
unsigned char ExtEncoder::readStatus(void)
{
	lock();
	unsigned char data;
	ss_pin_set(1);
	ss_pin_set(0); //enable device
	this->spiTransfer(READ_STATUS); //transmit read opcode
	data = this->spiTransfer(0xFF); //get data byte
	ss_pin_set(1); //release device
	unlock();
	return data;
}

/*
 unsigned long long as_spi_read4byte(int aFd,
 unsigned long long aMsg,
 size_t aLen,
 uint32_t aSpeed)
 {
 struct spi_ioc_transfer xfer;
 memset(&xfer, 0, sizeof(xfer));
 char dataBuffer[3];
 char rxBuffer[3];
 dataBuffer[0] = 0x0B;
 dataBuffer[1] = 0x02;
 dataBuffer[2] = 0x00;
 xfer.tx_buf = (unsigned long)dataBuffer;
 xfer.rx_buf = (unsigned long)rxBuffer;
 xfer.len = 3;
 xfer.speed_hz = 500000;
 xfer.cs_change = 1;
 xfer.bits_per_word = 8;
 res = ioctl(spiDev, SPI_IOC_MESSAGE(1), &xfer);
 printf("SPI result: %d\n", res);
 printf("Device ID: %d - %d - %d\n", rxBuffer[2], rxBuffer[1], rxBuffer[0]);
 }
 */
