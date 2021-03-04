#include "MagEncoders.hpp"

#include <unistd.h>
#include <iostream>

MagEncoders::MagEncoders(bool is1EncoderRight, bool invertEncoderRight, bool invertEncoderLeft) :
        m_mysensor1(AS5048B_ADDR(1, 1), 2), m_mysensor2(AS5048B_ADDR(1, 0), 2)
{

    m_invertEncoderR = invertEncoderRight;
    m_invertEncoderL = invertEncoderLeft;
    m_encoderRSum = 0;
    m_encoderLSum = 0;
    m_encoder1Previous = 0;
    m_encoder2Previous = 0;
    m_is1EncoderRight = is1EncoderRight;

    encoder1Previous_ = 0;
    encoder1Sum_ = 0;
    encoder2Previous_ = 0;
    encoder2Sum_ = 0;

}

MagEncoders::~MagEncoders() {
}

int MagEncoders::pingtest() {

    return m_mysensor1.ping();
}

void MagEncoders::init() {

    m_mysensor1.begin();

    m_mysensor2.begin();


    reset();

    //verification des statuts sur un test
    uint8_t agc = 0;
    uint8_t diag = 0;
    uint16_t mag = 0;
    uint16_t raw = 0;
    if (m_mysensor1.connected()) {
        m_mysensor1.getAllData(&agc, &diag, &mag, &raw);
        if (((diag & 0x02) >> 1) == 1) std::cout << "ERROR MagEncoders::init()  m_mysensor1 diag=" << (int) diag << std::endl;
        if (agc <= 30 && agc >= 70) std::cout << "ERROR MagEncoders::init()  m_mysensor1 out of range agc=" << agc << std::endl;
    }
    if (m_mysensor2.connected()) {
        m_mysensor2.getAllData(&agc, &diag, &mag, &raw);
        if (((diag & 0x02) >> 1) == 1) std::cout << "ERROR MagEncoders::init()  m_mysensor2 diag=" << (int) diag << std::endl;
        if (agc <= 30 && agc >= 70) std::cout << "ERROR MagEncoders::init()  m_mysensor2 out of range agc=" << agc << std::endl;
    }
}

void MagEncoders::reset() {

    m_encoder1Previous = (int16_t) ((m_mysensor1.angleR(U_RAW, true) - 8192.0) * 4.0);
    m_encoder2Previous = (int16_t) ((m_mysensor2.angleR(U_RAW, true) - 8192.0) * 4.0);
    m_encoderRSum = 0;
    m_encoderLSum = 0;

    encoder1Previous_ = m_encoder1Previous;
    encoder1Sum_ = 0;
    encoder2Previous_ = m_encoder2Previous;
    encoder2Sum_ = 0;
}
uint8_t MagEncoders::getDiag1() {
    return m_mysensor1.getDiagReg();
}
uint8_t MagEncoders::getAutoGain1() {
    return m_mysensor1.getAutoGain();
}


uint16_t MagEncoders::getAngle1() {
    return m_mysensor1.angleRegR();

}

uint16_t MagEncoders::getMag1() {
    return m_mysensor1.magnitudeR();

}

int32_t MagEncoders::getAccumulatedDirectValueEncoder1() {


    uint8_t agc = 0;
    uint8_t diag = 0;
    uint16_t mag = 0;
    uint16_t raw = 0;
    int err = 0;
    err = m_mysensor1.getAllData(&agc, &diag, &mag, &raw);
    if (err < 0) {
        std::cout << "MagEncoders::ERROR encoder1 err=" << err << std::endl;
        return encoder1Sum_;
    }
    if (((diag & 0x02) >> 1) == 1) {
        std::cout << "MagEncoders::ERROR encoder1 diag invalid data" << std::endl;
        return encoder1Sum_;
    }
    if (agc <= 30 && agc >= 70) {
        std::cout << "MagEncoders::ERROR encoder1 out of range agc=" << agc << std::endl;
        return encoder1Sum_;
    }

//    std::cout << "MagEncoders::encoder1_ "
//            << " agc="
//            << std::dec
//            << (int) agc
//            << std::dec
//            << " diag="
//            << std::hex
//            << (int) diag
//            << " mag="
//            << std::dec
//            << mag
//            << " raw="
//            << std::dec
//            << raw
//            << std::endl;

//    //[0;16383] -8192 * 4 = [-32768;32764]
    float val = ((float) raw - 8192.0) * 4.0;
    float delta = val - encoder1Previous_;
    delta = (((int16_t) (delta))) / 4.0; //saturation
    encoder1Sum_ += (int32_t) (delta);
    encoder1Previous_ = val;

    //filtrage de stabilité
    return encoder1Sum_;
}

int32_t MagEncoders::getAccumulatedDirectValueEncoder2() {

    //verification
    uint8_t agc = 0;
    uint8_t diag = 0;
    uint16_t mag = 0;
    uint16_t raw = 0;
    int err = 0;
    err = m_mysensor2.getAllData(&agc, &diag, &mag, &raw);
    if (err < 0) {
        std::cout << "MagEncoders::ERROR encoder2 err=" << err << std::endl;
        return encoder2Sum_;
    }
    if (((diag & 0x02) >> 1) == 1) {
        std::cout << "MagEncoders::ERROR encoder2 diag invalid data" << std::endl;
        return encoder2Sum_;
    }
    if (agc <= 30 && agc >= 70) {
        std::cout << "MagEncoders::ERROR encoder2 out of range agc=" << agc << std::endl;
        return encoder2Sum_;
    }
//    std::cout << "MagEncoders::encoder2_ "
//            << " agc="
//            << std::dec
//            << (int) agc
//            << std::dec
//            << " diag="
//            << std::hex
//            << (int) diag
//            << " mag="
//            << std::dec
//            << mag
//            << " raw="
//            << std::dec
//            << raw
//            << std::endl;

//    //[0;16383] -8192 * 4 = [-32768;32764]
    float val = ((float) raw - 8192.0) * 4.0;
    float delta = val - encoder2Previous_;
    delta = (((int16_t) (delta))) / 4.0; //saturation
    encoder2Sum_ += (int32_t) (delta);
    encoder2Previous_ = val;


    return encoder2Sum_;
}

int32_t MagEncoders::getFiltered(int32_t value, int8_t shift) {
    int32_t filtered = value >> shift;
    filtered = filtered << shift;
    return filtered;
}

void MagEncoders::getValues(float *deltaEncoderRightFiltered, float *deltaEncoderLeftFiltered) {
    float deltaEncoderRight = 0.0;
    float deltaEncoderLeft = 0.0;
//utilisation du depassement d'un int16
//[0;16383] -8192 * 4 = [-32768;32764]
    float encoder1 = (m_mysensor1.angleR(U_RAW, true) - 8192.0) * 4.0;

    float encoder2 = (m_mysensor2.angleR(U_RAW, true) - 8192.0) * 4.0;

    if (m_is1EncoderRight) {
        deltaEncoderRight = encoder1 - m_encoder1Previous;
        deltaEncoderLeft = encoder2 - m_encoder2Previous;
    }
    else {
        deltaEncoderRight = encoder2 - m_encoder2Previous;
        deltaEncoderLeft = encoder1 - m_encoder1Previous;
    }

    if (m_invertEncoderR) deltaEncoderRight = -deltaEncoderRight;
    if (m_invertEncoderL) deltaEncoderLeft = -deltaEncoderLeft;

    deltaEncoderRight = ((int16_t) (deltaEncoderRight)) / 4.0;
    deltaEncoderLeft = ((int16_t) (deltaEncoderLeft)) / 4.0;

    m_encoderRSum += (int32_t) (deltaEncoderRight);
    m_encoderLSum += (int32_t) (deltaEncoderLeft);

    m_encoder1Previous = encoder1;
    m_encoder2Previous = encoder2;

    *deltaEncoderRightFiltered = (deltaEncoderRight);
    *deltaEncoderLeftFiltered = (deltaEncoderLeft);

}

void MagEncoders::getEncodersTotalCount(int32_t *encoderRight, int32_t *encoderLeft) {
    *encoderRight = getFiltered(m_encoderRSum, 3);
    *encoderLeft = getFiltered(m_encoderLSum, 3);
}

void MagEncoders::getValuesStatus(uint16_t *rawR, uint16_t *rawL, uint8_t *agcR, uint8_t *agcL, uint8_t *diagR, uint8_t *diagL, uint16_t *magR,
        uint16_t *magL)
{
    m_mysensor1.getAllData(agcR, diagR, magR, rawR);
    m_mysensor2.getAllData(agcL, diagL, magL, rawL);
}
