#ifndef EV3_SERVODRIVER_HPP_
#define EV3_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "Adafruit_PWMServoDriver.hpp"
#include "CCAx12Adc.hpp"

#define NB_SERVO_STD 16

using namespace std;

class ServoDriver: public AServoDriver {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoDriver(EV3).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriver.EV3");
        return instance;
    }

    Adafruit_PWMServoDriver pwm_;

    bool connected_;

    int servo_current_usec_[NB_SERVO_STD];
    int servo_min_[NB_SERVO_STD]; //minPulseWidth
    int servo_mid_[NB_SERVO_STD]; //midPulseWidth
    int servo_max_[NB_SERVO_STD]; //maxPulseWidth
    bool servo_inv_[NB_SERVO_STD];
    int servo_rate_[NB_SERVO_STD];
    ServoType servo_type_[NB_SERVO_STD];

public:
    /*!
     * \brief Constructor.
     */
    ServoDriver();

    /*!
     * \brief Destructor.
     */
    inline ~ServoDriver() {
    }

    bool is_connected();

    void hold(int servo);

    /*!
     * \brief Position du servo en indiquant le pulseWidth en picroseconds.
     * pulseWidth : (from 500us to 2500us).
     * millisec0To90 :
     */
    void setPulsePos(int servo, int pulse_us, int millisec0To90 = 0);
    /*!
     * \brief Position du servo en indiquant le pulseWidth en picroseconds.
     * Prend en compte la vitesse desirée.
     * pulseWidth : (from 500us to 2500us)
     */
    void setPulsePosWithRate(int servo, int pulse_us, int millisec0To90 = 0);


    void setpwm(int servo, int pulsewidth_us);


    /*!
     * \brief retourne la Position du servo, le pulseWidth en picroseconds.
     * pulseWidth : (from 500us to 2500us)
     * \return pulsewidth du servo (from 0us to 3000us); negatif si erreur
     */
    int getPulsePos(int servo);

    //0 = deactivate = max speed
    //165ms is the minimum for std servo
    void setRate(int servo, int millisec);

    void setMinPulse(int servo, int value = 600); //default 600 [0 to 1000]

    void setMidPulse(int servo, int value = 1500); //default 1500 [1000 to 2000]

    void setMaxPulse(int servo, int value = 2400); //default 2400 [2000 to 3000]

    void setPolarity(int servo, bool inverted);

    void setType(int servo, ServoType type);

    void release(int servo);

    void turn(int servo, int speed);

    int getMoving(int servo);

    int getTorque(int servo);

    int ping(int);

};

#endif
