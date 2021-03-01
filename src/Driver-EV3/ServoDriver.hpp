#ifndef EV3_SERVODRIVER_HPP_
#define EV3_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "Adafruit_PWMServoDriver.hpp"

#define NB_SERVO 8

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
    int servo_current_usec_[NB_SERVO];
    int servo_min_[NB_SERVO];
    int servo_mid_[NB_SERVO];
    int servo_max_[NB_SERVO];
    bool servo_inv_[NB_SERVO];
    int servo_rate_[NB_SERVO];

protected:

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

    void hold(int servo);

    //pos : (from 500us to 2500us)
    void setPosition(int servo, int microseconds);
    void setPositionWithRate(int servo, int microsec);

    void release(int servo);

    //0 = deactivate = max speed
    //165ms is the minimum for std servo
    void setRate(int servo, int millisec);

    void turn(int servo, int speed);

    int getMoving(int servo);

    //pos : (from 500us to 2500us)
    int getPos(int servo);

    int ping(int);

    void setMinPulse(int servo, int value = 600); //default 600 [0 to 1000]

    void setMidPulse(int servo, int value = 1500); //default 1500 [1000 to 2000]

    void setMaxPulse(int servo, int value = 2400); //default 2400 [2000 to 3000]

    void setPolarity(int servo, bool inverted);

    int getTorque(int servo);

};

#endif
