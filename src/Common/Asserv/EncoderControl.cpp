#include "EncoderControl.hpp"

#include "../Asserv.Driver/AAsservDriver.hpp"
//#include "AAsservElement.hpp"

using namespace std;

EncoderControl::EncoderControl(std::string botid, bool isExternalEncoders)
{
    isExternalEncoders_ = isExternalEncoders;
    asservdriver = AAsservDriver::create(botid);
}

EncoderControl::~EncoderControl() {
}

long EncoderControl::getLeftEncoder() {
    if (isExternalEncoders_) return asservdriver->getLeftExternalEncoder();
    else return asservdriver->getLeftInternalEncoder();
}

long EncoderControl::getRightEncoder() {
    if (isExternalEncoders_) return asservdriver->getRightExternalEncoder();
    else return asservdriver->getRightInternalEncoder();
}

void EncoderControl::reset() {
    if (isExternalEncoders_) return asservdriver->resetExternalEncoders();
    else return asservdriver->resetInternalEncoders();
}

void EncoderControl::getCounts(int * countR, int * countL)
{
    if (isExternalEncoders_)
        asservdriver->getCountsExternal(countR, countL);
    else
        asservdriver->getCountsInternal(countR, countL);
}
