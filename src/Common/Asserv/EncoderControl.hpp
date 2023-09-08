#ifndef COMMON_ASSERV_ENCODERCONTROL_HPP_
#define COMMON_ASSERV_ENCODERCONTROL_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"

class AAsservDriver;

class EncoderControl {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref EncoderControl.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("EncoderControl");
        return instance;
    }

    AAsservDriver* asservdriver_;

    bool isExternalEncoders_;

public:

    /*!
     * \brief Constructor.
     */
    EncoderControl(std::string botid, bool isExternalEncoders, AAsservDriver* a);

    /*!
     * \brief Destructor.
     */
    ~EncoderControl();

    /*!
     * \brief getEncoderLeft.
     */
    long getLeftEncoder();

    /*!
     * \brief getEncoderRight.
     */
    long getRightEncoder();

    /*!
     * \brief Recupere les 2 encoders de facon optimisée.
     */
    void getCounts(int * countR, int * countL);

    /*!
     * \brief reset encoders.
     */
    void reset();

};

#endif
