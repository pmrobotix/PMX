#include "EncoderControl.hpp"

#include "../Asserv.Driver/AAsservDriver.hpp"
//#include "AAsservElement.hpp"

using namespace std;

EncoderControl::EncoderControl(std::string botid)//Asserv & asserv) :	AAsservElement(asserv)
{
	asservdriver = AAsservDriver::create(botid);
}

EncoderControl::~EncoderControl()
{
}

long EncoderControl::getLeftEncoder()
{
	return asservdriver->getLeftInternalEncoder();
}

long EncoderControl::getRightEncoder()
{
	return asservdriver->getRightInternalEncoder();
}

void EncoderControl::reset()
{
	return asservdriver->resetEncoder();
}
