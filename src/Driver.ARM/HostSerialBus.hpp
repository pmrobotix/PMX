/*!
 * \file
 * \brief Définition de la classe HostSerialBus, gestion du bus serial 2 pour APF9328.
 */

#ifndef HOSTSERIALBUS_HPP
#define HOSTSERIALBUS_HPP

#include <string>

#include "../Log/Exception.hpp"

/*!
 * \brief Exception lancée s'il y a une erreur serial.
 */
class HostSerialException: public logs::Exception
{
public:
	HostSerialException(const std::string & message)
			: Exception(message)
	{
	}

	virtual ~ HostSerialException() throw ()
	{
	}
};

class HostSerialBus
{
private:

	int fileDescriptor_;

public:

	int connect();

	int connect(const char * device);

	void disconnect(void);

	int sendArray(unsigned char *buffer, int len);

	int getArray(unsigned char *buffer, int len);

	int bytesToRead();

	void clear();
};

#endif
