/*!
 * \file
 * \brief Implémentation de la classe HostSerialBus.
 */

#include "HostSerialBus.hpp"

#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

int HostSerialBus::connect()
{
	return connect("//dev//ttySMX1");
}

int HostSerialBus::connect(const char *device)
{
	struct termios terminalAttributes;

	/*
	 * http://linux.die.net/man/2/open
	 *
	 * Open the serial port
	 * read/write
	 * not become the process's controlling terminal
	 * When possible, the file is opened in nonblocking mode
	 *
	 */
	fileDescriptor_ = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_FSYNC);

	if (fileDescriptor_ < 0)
		throw new HostSerialException("Error HostSerialBus::connect, can't connect with open function");

	// clear terminalAttributes data
	memset(&terminalAttributes, 0, sizeof(struct termios));

	/*	http://linux.die.net/man/3/termios
	 *
	 *  control modes: c_cflag flag constants:
	 *
	 * 115200 bauds
	 * 8 bits per word
	 * Ignore modem control lines.
	 * Enable receiver.
	 */

	terminalAttributes.c_cflag = B115200 | CS8 | CLOCAL | CREAD;

	/*
	 * input modes: c_iflag flag constants:
	 *
	 * Ignore framing errors and parity errors.
	 * (XSI) Map NL to CR-NL on output.
	 */
	terminalAttributes.c_iflag = IGNPAR | ONLCR;

	/*
	 * output modes: flag constants defined in POSIX.1
	 *
	 * Enable implementation-defined output processing.
	 */

	terminalAttributes.c_oflag = OPOST;

	/*
	 * Canonical and noncanonical mode
	 *
	 * min time
	 * min bytes to read
	 */

	//terminalAttributes.c_lflag = ICANON;
	terminalAttributes.c_cc[VTIME] = 0;
	terminalAttributes.c_cc[VMIN] = 1;

	/*
	 * http://linux.die.net/man/3/tcsetattr
	 * Set the port to our state
	 *
	 * the change occurs immediately
	 */

	tcsetattr(fileDescriptor_, TCSANOW, &terminalAttributes);

	/*
	 * http://linux.die.net/man/3/tcflush
	 *
	 * flushes data written but not transmitted.
	 * flushes data received but not read.
	 */
	clear();

	return fileDescriptor_;
}

void HostSerialBus::disconnect(void)
{
	close(fileDescriptor_);
}

int HostSerialBus::sendArray(unsigned char *buffer, int len)
{
	tcdrain(fileDescriptor_);
	int err = write(fileDescriptor_, buffer, len);

	if (err < 0 || err != len)
	{
		throw new HostSerialException("ERROR setInfo serial_.sendArray()");
	}

	return err;
}

int HostSerialBus::getArray(unsigned char *buffer, int len)
{
	int n = 0;
	int readOk = false;
	for (int i = 0; i < 200; i++) //tempo to wait data in the buffer
	{
		n = bytesToRead();
		if (n >= len)
		{
			readOk = true;
			if (n > len)
			{
				std::cout << "HostSerialBus::getArray warning by to read  "
						<< n
						<< " instead of  "
						<< len
						<< std::endl;
			}
			break;
		}
		else
		{
			usleep(10);
		}
	}
	if (!readOk)
	{
		std::cout << "HostSerialBus::getArray giving up "
				<< n
				<< " instead of  "
				<< len
				<< std::endl;
	}
	n = read(fileDescriptor_, buffer, len);

	return n;
}

void HostSerialBus::clear()
{
	tcflush(fileDescriptor_, TCIFLUSH);
	tcflush(fileDescriptor_, TCOFLUSH);
}

int HostSerialBus::bytesToRead()
{
	int bytes = 0;
	int err = ioctl(fileDescriptor_, FIONREAD, &bytes);
	if (err < 0)
		throw new HostSerialException("Error HostSerialBus::bytesToRead on ioctl");

	return bytes;
}
