#ifndef EV3_ASSERVDRIVER_MAGENCODERS_HPP_
#define EV3_ASSERVDRIVER_MAGENCODERS_HPP_

#include <cstdint>

#include "ams_as5048b.hpp"
#include "../Log/LoggerFactory.hpp"

#define AGC_MIN 30
#define AGC_MAX 79

#define M_2PI (2.0*M_PI)

class MagEncoders {

public:

    MagEncoders(bool is1EncoderRight, bool invertEncoderRight = false, bool invertEncoderLeft = false);
    ~MagEncoders();

    int pingSensor1();
    int pingSensor2();

    void init();
    void reset();

    //(for testing)
    void getEncodersTotalCount(int32_t *sumEncoderRight, int32_t *sumEncoderLeft);
    uint8_t getDiag1();
    uint8_t getAutoGain1();
    uint16_t getAngle1();
    uint16_t getMag1();
    uint8_t getDiag2();
    uint8_t getAutoGain2();
    uint16_t getAngle2();
    uint16_t getMag2();

    int getAccumulatedDirectValueEncoder1(int32_t * value);
    int getAccumulatedDirectValueEncoder2(int32_t * value);
//    int32_t getFiltered(int32_t value, int8_t shift);

    /*!
     * \brief Retourne l'avancement(delta) droite et gauche avec le calcul.
     * \param deltaEncoderRight.
     * \param deltaEncoderLeft.
     * \return retourne 0 si ok, negatif si erreur.
     */
    int getValues(float *deltaEncoderRight, float *deltaEncoderLeft);

    /*!
     * \brief Retourne les données raw des codeurs dans les pointeurs.
     * \return retourne 0 si ok, negatif si erreurs.
     */
    int getValuesStatus(uint16_t *encoderRight, uint16_t *encoderLeft, uint8_t *agcR, uint8_t *agcL, uint8_t *diagR, uint8_t *diagL, uint16_t *magR,
            uint16_t *magL);
private:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref MagEncoders(EV3).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("MagEncoders.EV3");
        return instance;
    }

    AMS_AS5048B m_mysensor1;
    AMS_AS5048B m_mysensor2;

    bool m_invertEncoderL;
    bool m_invertEncoderR;
    int32_t m_encoderLSum;
    int32_t m_encoderRSum;
    float m_encoder1Previous;
    float m_encoder2Previous;

    bool m_is1EncoderRight;

    //specifique pour enc1 et enc2
    float encoder1Previous_;
    int32_t encoder1Sum_;
    float encoder2Previous_;
    int32_t encoder2Sum_;

};

#endif
