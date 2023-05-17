#ifndef OPOS6UL_SERVODRIVER_HPP_
#define OPOS6UL_SERVODRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

//#define NB_SERVO_DYN 255
#define MAX_POS 1023
#define MAXPORTNUM 4255

class ServoDriver: public AServoDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoDriver(OPOS6UL).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriver.OPO");
        return instance;
    }

    bool testIf(long value, long valeurMin, long valeurMax);

    bool connected_;
    //ServoType servo_type_[NB_SERVO_DYN];

protected:

public:
    /*!
     * \brief Constructor.
     */
    ServoDriver();

    /*!
     * \brief Destructor.
     */
    inline ~ServoDriver()
    {

    }

    bool is_connected();

    void hold(int servo);

    //DEPRECATED
    void setType(int servo, ServoType type);

    //pos AX12 0->1023
    void setPulsePos(int servo, int pulse, int rate_milli = 0);

    void release(int servo);



    void turn(int servo, int speed);

    //return 1 when in progress, 0 when idle, -1 on error
    int getMoving(int servo);

    int getPulsePos(int servo);

    int ping(int servo);

    void setMinPulse(int servo, int value = 600); //default 600 [300 to 700]

    void setMidPulse(int servo, int value = 1500); //default 1500 [1300 to 1700]

    void setMaxPulse(int servo, int value = 2400); //default 2400 [2300 to 2700]

    void setPolarity(int servo, bool inversed);

    int getTorque(int servo);

    void setRate(int servo, int speed);
};

#endif
