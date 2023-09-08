#include "EncoderControl.hpp"

#include "../Interface.Driver/AAsservDriver.hpp"
//#include "AAsservElement.hpp"

using namespace std;

EncoderControl::EncoderControl(std::string botid, bool isExternalEncoders, AAsservDriver* asservdriver)
{
    isExternalEncoders_ = isExternalEncoders;
    asservdriver_ = asservdriver;
}

EncoderControl::~EncoderControl() {
}

long EncoderControl::getLeftEncoder() {
    if (isExternalEncoders_) return asservdriver_->getLeftExternalEncoder();
    else return asservdriver_->getLeftInternalEncoder();
}

long EncoderControl::getRightEncoder() {
    if (isExternalEncoders_) return asservdriver_->getRightExternalEncoder();
    else return asservdriver_->getRightInternalEncoder();
}

void EncoderControl::reset() {
    if (isExternalEncoders_) return asservdriver_->resetExternalEncoders();
    else return asservdriver_->resetInternalEncoders();
}

void EncoderControl::getCounts(int * countR, int * countL)
{
    if (isExternalEncoders_)
        asservdriver_->getCountsExternal(countR, countL);
    else
        asservdriver_->getCountsInternal(countR, countL);
}
