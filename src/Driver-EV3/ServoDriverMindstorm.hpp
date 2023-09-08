#ifndef EV3_SERVODRIVERMINDSTORM_HPP_
#define EV3_SERVODRIVERMINDSTORM_HPP_

#include "../Common/Interface.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ServoMotorExtend.hpp"

using namespace std;
using namespace ev3dev;

class ServoDriverMindstorm: public AServoDriver, device_extend
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoDriverMindstorm(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriverMindstorm.EV3");
        return instance;
    }
    int connected_;
    int connected_servo[8];
    ServoMotorExtend servo_motors_[8];

protected:

public:
    /*!
     * \brief Constructor.
     */
    ServoDriverMindstorm();

    /*!
     * \brief Destructor.
     */
    inline ~ServoDriverMindstorm()
    {
    }

    void hold(int servo);

    //pos : (-100% to 100%)
    void setPosition(int servo, int percent);

    void release(int servo);

    void setRate(int servo, int millisec);

    void turn(int servo, int speed);

    int getMoving(int servo);

    //pos : (-100% to 100%)
    int getPos(int servo);

    int ping(int);

    void setMinPulse(int servo, int value = 600);//default 600 [300 to 700]

    void setMidPulse(int servo, int value = 1500);//default 1500 [1300 to 1700]

    void setMaxPulse(int servo, int value = 2400);//default 2400 [2300 to 2700]

    void setPolarity(int servo, bool inversed);

    int getTorque(int servo);
    void setType(int servo, AServoDriver::ServoType);

    //long constrain(long value, long valeurMin, long valeurMax);
    //bool testIf(long value, long valeurMin, long valeurMax);
};

#endif
