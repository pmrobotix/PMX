#include "ExtEncoderControl.hpp"

#include <string>

#include "../Asserv.Driver/AAsservDriver.hpp"

using namespace std;

ExtEncoderControl::ExtEncoderControl(std::string botid)
{
	asservdriver = AAsservDriver::create(botid);
}

ExtEncoderControl::~ExtEncoderControl()
{
}

long ExtEncoderControl::getLeftEncoder()
{
	return asservdriver->getLeftExternalEncoder();
}

long ExtEncoderControl::getRightEncoder()
{
	return asservdriver->getRightExternalEncoder();
}

void ExtEncoderControl::reset()
{
	return asservdriver->resetEncoders(); //todo reset Extencoder
}
