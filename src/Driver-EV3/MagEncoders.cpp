#include "MagEncoders.hpp"

#include <unistd.h>
#include <iostream>

MagEncoders::MagEncoders(bool is1EncoderRight, bool invertEncoderRight, bool invertEncoderLeft) :
        m_mysensor1(AS5048B_ADDR(1, 1), 1), m_mysensor2(AS5048B_ADDR(0, 1), 1)
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

    f_encoder1Previous = 0;
    f_encoder2Previous = 0;
}

MagEncoders::~MagEncoders() {
}

int MagEncoders::pingSensor1() {
    return m_mysensor1.ping();
}
int MagEncoders::pingSensor2() {
    return m_mysensor2.ping();
}

int MagEncoders::connect() {

    bool s1 = m_mysensor1.connect();
    bool s2 = m_mysensor2.connect();

    if (!s1)
        return -10;
    if (!s2)
        return -20;

    reset();

    //verification des statuts sur un test
    uint8_t agc = 0;
    uint8_t diag = 0;
    uint16_t mag = 0;
    uint16_t raw = 0;
    if (m_mysensor1.connected()) {
        m_mysensor1.getAllData(&agc, &diag, &mag, &raw);
        //COF (CORDIC Overflow), logic high indicates an out of range error in the CORDIC part.
        //When this bit is set, the angle and magnitude data is invalid.
        //The absolute output maintains the last valid angular value.
        if (((diag & 0x02) >> 1) == 1) logger().error() << "init() m_mysensor1.getAllData() COF (CORDIC Overflow) diag=" << (int) diag << logs::end;

        //OCF (Offset Compensation Finished), logic high indicates the finished Offset Compensation Algorithm. After power
        //up the flag remains always to logic high.
        if ((diag & 0x01) != 1)
            logger().error() << "init() m_mysensor1.getAllData() Offset Compensation is not Finished!!! diag=" << (int) diag << logs::end;

        //Automatic Gain Control value. 0 decimal represents high magnetic field, 255 decimal represents low  magnetic field
        if (agc <= AGC_MIN && agc >= AGC_MAX) logger().error() << "ERROR MagEncoders::init() m_mysensor1 out of range agc=" << agc << logs::end;

        //logger().info() << "init() m_mysensor1 CONNECTED !!" << logs::end;
    }
    else {
        logger().error() << "init() m_mysensor1 NOT CONNECTED !!" << logs::end;
    }
    if (m_mysensor2.connected()) {
        m_mysensor2.getAllData(&agc, &diag, &mag, &raw);
        //COF (CORDIC Overflow), logic high indicates an out of range error in the CORDIC part.
        //When this bit is set, the angle and magnitude data is invalid.
        //The absolute output maintains the last valid angular value.
        if (((diag & 0x02) >> 1) == 1) logger().error() << "init() m_mysensor2.getAllData() COF (CORDIC Overflow) diag=" << (int) diag << logs::end;

        //OCF (Offset Compensation Finished), logic high indicates the finished Offset Compensation Algorithm.
        //After power up the flag remains always to logic high.
        if ((diag & 0x01) != 1)
            logger().error() << "init() m_mysensor2.getAllData() Offset Compensation is not Finished!!! diag=" << (int) diag << logs::end;

        //Automatic Gain Control value. 0 decimal represents high magnetic field, 255 decimal represents low  magnetic field
        if (agc <= AGC_MIN && agc >= AGC_MAX) logger().error() << "ERROR MagEncoders::init() m_mysensor2 out of range agc=" << agc << logs::end;
        //logger().info() << "init() m_mysensor2 CONNECTED !!" << logs::end;
    }
    else {
        logger().error() << "init() m_mysensor2 NOT CONNECTED !!" << logs::end;
    }
}

void MagEncoders::reset() {
    uint8_t agc = 0;
    uint8_t diag = 0;
    uint16_t mag = 0;
    uint16_t raw = 0;
    int err1 = 0;
    int err2 = 0;

    if (m_mysensor1.connected()) {
        err1 = m_mysensor1.getAllData(&agc, &diag, &mag, &raw);
        if (err1 < 0) {
            logger().error() << "reset() m_mysensor1.getAllData()  err1=" << err1 << logs::end;
            err1 = err1 * 100;
        }
        //COF (CORDIC Overflow), logic high indicates an out of range error in the CORDIC part.
        //When this bit is set, the angle and magnitude data is invalid.
        //The absolute output maintains the last valid angular value.
        if (((diag & 0x02) >> 1) == 1) {
            logger().error() << "reset() m_mysensor1.getAllData COF (CORDIC Overflow) diag=" << (int) diag << logs::end;
            err1 = -900;
        }
        //Automatic Gain Control value. 0 decimal represents high magnetic field, 255 decimal represents low  magnetic field
        if (agc <= AGC_MIN && agc >= AGC_MAX) {
            logger().error() << "reset() m_mysensor1.getAllData out of range agc=" << agc << logs::end;
            err1 = -800;
        }
        if (err1 >= 0) m_encoder1Previous = (int16_t) ((raw - 8192.0) * 4.0);
        else logger().error() << "reset() m_encoder1Previous IMPOSSIBLE to RESET !" << logs::end;
    }

    if (m_mysensor2.connected()) {
        err2 = m_mysensor2.getAllData(&agc, &diag, &mag, &raw);
        if (err2 < 0) {
            logger().error() << "reset() m_mysensor2.getAllData() err2=" << err2 << logs::end;
            err2 = err2 * 100;
        }
        //COF (CORDIC Overflow), logic high indicates an out of range error in the CORDIC part.
        //When this bit is set, the angle and magnitude data is invalid.
        //The absolute output maintains the last valid angular value.
        if (((diag & 0x02) >> 1) == 1) {
            logger().error() << "reset() m_mysensor2.getAllData COF (CORDIC Overflow) diag=" << (int) diag << logs::end;
            err2 = -900;
        }
        //Automatic Gain Control value. 0 decimal represents high magnetic field, 255 decimal represents low  magnetic field
        if (agc <= AGC_MIN && agc >= AGC_MAX) {
            logger().error() << "reset() m_mysensor2.getAllData out of range agc=" << agc << logs::end;
            err2 = -800;
        }
        if (err2 >= 0) m_encoder2Previous = (int16_t) ((raw - 8192.0) * 4.0);
        else logger().error() << "reset() m_encoder2Previous IMPOSSIBLE to RESET !" << logs::end;
    }

//    m_encoder1Previous = (int16_t) ((m_mysensor1.angleR(U_RAW, true) - 8192.0) * 4.0);
//    m_encoder2Previous = (int16_t) ((m_mysensor2.angleR(U_RAW, true) - 8192.0) * 4.0);

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
uint8_t MagEncoders::getDiag2() {
    return m_mysensor2.getDiagReg();
}
uint8_t MagEncoders::getAutoGain2() {
    return m_mysensor2.getAutoGain();
}
uint16_t MagEncoders::getAngle2() {
    return m_mysensor2.angleRegR();
}
uint16_t MagEncoders::getMag2() {
    return m_mysensor2.magnitudeR();
}

int MagEncoders::getAccumulatedDirectValueEncoder1(int32_t* value) {
    uint16_t raw1 = 0;
    uint8_t agc1 = 0;
    uint8_t diag1 = 0;
    uint16_t mag1 = 0;
    int err1 = -10;

    int again = 3;
    if (m_mysensor1.connected()) {
        for (int i = 0; i < again; i++) {
            err1 = m_mysensor1.getAllData(&agc1, &diag1, &mag1, &raw1);
            if (err1 < 0) {
                logger().error() << "m_mysensor1.getAllData() err1=" << err1 << logs::end;
                err1 = err1 * 100;
            }
            if (((diag1 & 0x02) >> 1) == 1) {
                logger().error() << "m_mysensor1.getAllData() diag1=" << diag1 << logs::end;
                err1 = -900;
            }
            if (agc1 <= AGC_MIN && agc1 >= AGC_MAX) {
                logger().error() << "m_mysensor1.getAllData() agc1=" << agc1 << logs::end;
                err1 = -800;
            }
            if (err1 >= 0) break;
            else logger().error() << "getAccumulatedDirectValueEncoder1() m_mysensor1.getAllData() TRY AGAIN..." << logs::end;
        }
    }

    //pas de calcul s'il n'y a pas de données.
    if (err1 < 0) return err1;

    //[0;16383] -8192 * 4 = [-32768;32764]
    float val = ((float) raw1 - 8192.0) * 4.0;
    float delta = val - encoder1Previous_;
    delta = (((int16_t) (delta))) / 4.0; //saturation
    encoder1Sum_ += (int32_t) (delta);
    encoder1Previous_ = val;

    *value = encoder1Sum_;
    return 0;
}

int MagEncoders::getAccumulatedDirectValueEncoder2(int32_t* value) {
    uint16_t raw2 = 0;
    uint8_t agc2 = 0;
    uint8_t diag2 = 0;
    uint16_t mag2 = 0;
    int err2 = -10;

    int again = 3;
    if (m_mysensor2.connected()) {
        for (int i = 0; i < again; i++) {
            err2 = m_mysensor2.getAllData(&agc2, &diag2, &mag2, &raw2);
            if (err2 < 0) {
                logger().error() << "m_mysensor2.getAllData() err2=" << err2 << logs::end;
                err2 = err2 * 100;
            }
            if (((diag2 & 0x02) >> 1) == 1) {
                logger().error() << "m_mysensor2.getAllData() diag2=" << diag2 << logs::end;
                err2 = -900;
            }
            if (agc2 <= AGC_MIN && agc2 >= AGC_MAX) {
                logger().error() << "m_mysensor2.getAllData() agc2=" << agc2 << logs::end;
                err2 = -800;
            }

            if (err2 >= 0) break;
            else logger().error() << "getAccumulatedDirectValueEncoder2() m_mysensor2.getAllData() TRY AGAIN..." << logs::end;
        }
    }
    //pas de calcul s'il n'y a pas de données.
    if (err2 < 0) return err2;

    //[0;16383] -8192 * 4 = [-32768;32764]
    float val = ((float) raw2 - 8192.0) * 4.0;
    float delta = val - encoder2Previous_;
    delta = (((int16_t) (delta))) / 4.0; //saturation
    encoder2Sum_ += (int32_t) (delta);
    encoder2Previous_ = val;

    //return the total nb of encoder (for testing)
    *value = encoder2Sum_;
    return 0;
}

int MagEncoders::getValues(int32_t *deltaEncoderRightFiltered, int32_t *deltaEncoderLeftFiltered) {
    int16_t deltaEncoderRight = 0;
    int16_t deltaEncoderLeft = 0;
    uint16_t raw1 = 0;
    uint16_t raw2 = 0;
    uint8_t agc1 = 0;
    uint8_t agc2 = 0;
    uint8_t diag1 = 0;
    uint8_t diag2 = 0;
    uint16_t mag1 = 0;
    uint16_t mag2 = 0;
    int err1 = -10;
    int err2 = -20;

    int again = 3;

    if (m_mysensor1.connected()) {
        for (int i = 0; i < again; i++) {
            err1 = m_mysensor1.getAllData(&agc1, &diag1, &mag1, &raw1);
            if (err1 < 0) {
                logger().error() << "m_mysensor1.getAllData() err1=" << err1 << logs::end;
                err1 = err1 * 100;
            }
            if (((diag1 & 0x02) >> 1) == 1) {
                logger().error() << "m_mysensor1.getAllData() diag1=" << diag1 << logs::end;
                err1 = -900;
            }
            if (agc1 <= AGC_MIN && agc1 >= AGC_MAX) {
                logger().error() << "m_mysensor1.getAllData() agc1=" << agc1 << logs::end;
                err1 = -800;
            }

            if (err1 >= 0) break;
            else logger().error() << "getValues() m_mysensor1.getAllData() TRY AGAIN..." << logs::end;
        }
    }

    if (m_mysensor2.connected()) {
        for (int i = 0; i < again; i++) {
            err2 = m_mysensor2.getAllData(&agc2, &diag2, &mag2, &raw2);
            if (err2 < 0) {
                logger().error() << "m_mysensor2.getAllData() err2=" << err2 << logs::end;
                err2 = err2 * 1000;
            }

            if (((diag2 & 0x02) >> 1) == 1) {
                logger().error() << "m_mysensor2.getAllData() diag2=" << diag2 << logs::end;
                err2 = -9000;
            }

            if (agc2 <= AGC_MIN && agc2 >= AGC_MAX) {
                logger().error() << "m_mysensor2.getAllData() agc2=" << agc2 << logs::end;
                err2 = -8000;
            }

            if (err2 >= 0) break;
            else logger().error() << "getValues() m_mysensor2.getAllData() TRY AGAIN..." << logs::end;
        }
    }

    int16_t encoder1 = ((int16_t) (raw1) - 8192.0) * 4.0; //utilisation du depassement d'un int16 : [0;16383] -8192 * 4 = [-32768;32764]
    int16_t encoder2 = ((int16_t) (raw2) - 8192.0) * 4.0; //utilisation du depassement d'un int16 : [0;16383] -8192 * 4 = [-32768;32764]

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

    //printf("MagEncoders::getValues L:%d  R:%d  Lsum:%d Rsum:%d\n", deltaEncoderLeft, deltaEncoderRight, m_encoderLSum, m_encoderRSum);

    //on retourne l'erreur, si la donnée n'est pas fiable
    if (err1 < 0 || err2 < 0) return err1 + err2;
    return 0;
}
int MagEncoders::getValuesf(float *deltaEncoderRightFiltered, float *deltaEncoderLeftFiltered) {
    float deltaEncoderRight = 0.0;
    float deltaEncoderLeft = 0.0;
    uint16_t raw1 = 0;
    uint16_t raw2 = 0;
    uint8_t agc1 = 0;
    uint8_t agc2 = 0;
    uint8_t diag1 = 0;
    uint8_t diag2 = 0;
    uint16_t mag1 = 0;
    uint16_t mag2 = 0;
    int err1 = -10;
    int err2 = -20;

    int again = 3;

    if (m_mysensor1.connected()) {
        for (int i = 0; i < again; i++) {
            err1 = m_mysensor1.getAllData(&agc1, &diag1, &mag1, &raw1);
            if (err1 < 0) {
                logger().error() << "m_mysensor1.getAllData() err1=" << err1 << logs::end;
                err1 = err1 * 100;
            }
            if (((diag1 & 0x02) >> 1) == 1) {
                logger().error() << "m_mysensor1.getAllData() diag1=" << diag1 << logs::end;
                err1 = -900;
            }
            if (agc1 <= AGC_MIN && agc1 >= AGC_MAX) {
                logger().error() << "m_mysensor1.getAllData() agc1=" << agc1 << logs::end;
                err1 = -800;
            }

            if (err1 >= 0) break;
            else logger().error() << "getValues() m_mysensor1.getAllData() TRY AGAIN..." << logs::end;
        }
    }

    if (m_mysensor2.connected()) {
        for (int i = 0; i < again; i++) {
            err2 = m_mysensor2.getAllData(&agc2, &diag2, &mag2, &raw2);
            if (err2 < 0) {
                logger().error() << "m_mysensor2.getAllData() err2=" << err2 << logs::end;
                err2 = err2 * 1000;
            }

            if (((diag2 & 0x02) >> 1) == 1) {
                logger().error() << "m_mysensor2.getAllData() diag2=" << diag2 << logs::end;
                err2 = -9000;
            }

            if (agc2 <= AGC_MIN && agc2 >= AGC_MAX) {
                logger().error() << "m_mysensor2.getAllData() agc2=" << agc2 << logs::end;
                err2 = -8000;
            }

            if (err2 >= 0) break;
            else logger().error() << "getValues() m_mysensor2.getAllData() TRY AGAIN..." << logs::end;
        }
    }

    float encoder1 = ((float) (raw1) - 8192.0) * 4.0; //utilisation du depassement d'un int16 : [0;16383] -8192 * 4 = [-32768;32764]
    float encoder2 = ((float) (raw2) - 8192.0) * 4.0; //utilisation du depassement d'un int16 : [0;16383] -8192 * 4 = [-32768;32764]

    if (m_is1EncoderRight) {
        deltaEncoderRight = encoder1 - f_encoder1Previous;
        deltaEncoderLeft = encoder2 - f_encoder2Previous;
    }
    else {
        deltaEncoderRight = encoder2 - f_encoder2Previous;
        deltaEncoderLeft = encoder1 - f_encoder1Previous;
    }

    if (m_invertEncoderR) deltaEncoderRight = -deltaEncoderRight;
    if (m_invertEncoderL) deltaEncoderLeft = -deltaEncoderLeft;

    deltaEncoderRight = ((int16_t) (deltaEncoderRight)) / 4.0;
    deltaEncoderLeft = ((int16_t) (deltaEncoderLeft)) / 4.0;

    m_encoderRSum += (int32_t) (deltaEncoderRight);
    m_encoderLSum += (int32_t) (deltaEncoderLeft);

    f_encoder1Previous = encoder1;
    f_encoder2Previous = encoder2;

    *deltaEncoderRightFiltered = (deltaEncoderRight);
    *deltaEncoderLeftFiltered = (deltaEncoderLeft);

    //printf("MagEncoders::getValues L:%f  R:%f  Lsum:%d Rsum:%d\n", deltaEncoderLeft, deltaEncoderRight, m_encoderLSum, m_encoderRSum);

    //on retourne l'erreur, si la donnée n'est pas fiable
    if (err1 < 0 || err2 < 0) return err1 + err2;
    return 0;
}

void MagEncoders::getEncodersTotalCount(int32_t *encoderRight, int32_t *encoderLeft) {

    *encoderRight = m_encoderRSum;
    *encoderLeft = m_encoderLSum;
}

/*
 int MagEncoders::getValuesStatus(uint16_t *raw1, uint16_t *raw2, uint8_t *agc1, uint8_t *agc2, uint8_t *diag1, uint8_t *diag2, uint16_t *mag1,
 uint16_t *mag2)
 {
 int err1 = m_mysensor1.getAllData(agc1, diag1, mag1, raw1);
 int err2 = m_mysensor2.getAllData(agc2, diag2, mag2, raw2);
 if (err1 < 0) return err1 * 100;
 if (err2 < 0) return err2 * 1000;
 return 0;
 }*/
