#ifndef EV3_ASSERVDRIVER_MAGENCODERS_HPP_
#define EV3_ASSERVDRIVER_MAGENCODERS_HPP_

#include <cstdint>

#include "ams_as5048b.hpp"


class MagEncoders {
public:

    MagEncoders(bool is1EncoderRight, bool invertEncoderRight = false, bool invertEncoderLeft = false);
    ~MagEncoders();

    int pingtest();
    void init();
    void reset();

    void getEncodersTotalCount(int32_t *sumEncoderRight, int32_t *sumEncoderLeft);
    uint8_t getDiag1();
    uint8_t getAutoGain1();
    uint16_t getAngle1();
    uint16_t getMag1();

    int32_t getAccumulatedDirectValueEncoder1();
    int32_t getAccumulatedDirectValueEncoder2();
    int32_t getFiltered(int32_t value, int8_t shift);

    void getValues(float *deltaEncoderRight, float *deltaEncoderLeft);

    void getValuesStatus(uint16_t *encoderRight, uint16_t *encoderLeft, uint8_t *agcR, uint8_t *agcL, uint8_t *diagR, uint8_t *diagL, uint16_t *magR,
            uint16_t *magL);
private:

    AMS_AS5048B m_mysensor1;
    AMS_AS5048B m_mysensor2;

    bool m_invertEncoderL;
    bool m_invertEncoderR;
    int32_t m_encoderLSum;
    int32_t m_encoderRSum;
    float m_encoder1Previous;
    float m_encoder2Previous;

    bool m_is1EncoderRight;

    //specifique pour enc1
    float encoder1Previous_;
    int32_t encoder1Sum_;
    float encoder2Previous_;
    int32_t encoder2Sum_;

};

#endif
