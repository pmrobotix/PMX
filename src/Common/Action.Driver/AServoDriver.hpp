/*
 * Pour les servomoteurs standards
 */

#ifndef ASERVODRIVER_HPP_
#define ASERVODRIVER_HPP_

class AServoDriver
{

public:

    enum ServoType
    {
        SERVO_STANDARD,
        SERVO_DYNAMIXEL
    };

    /*!
     * \brief single instance creation.
     */
    static AServoDriver * create(ServoType type);

    /*!
     * \brief Destructor.
     */
    virtual ~AServoDriver()
    {
    }

    /*!
     * \brief Constructor.
     */
    AServoDriver()
    {
    }

    //Limitation d'une valeur à un intervalle [valeurMin , valeurMax]
    long constrain(long value, long valeurMin, long valeurMax)
    {
        if (value <= valeurMin)
            return valeurMin;

        if (value >= valeurMax)
            return valeurMax;

        return value;
    }

    virtual void hold(int) = 0;

    virtual void setPosition(int servo, int pos) = 0;

    virtual void release(int servo) = 0;

    virtual void setRate(int servo, int speed) = 0;

    virtual void turn(int servo, int speed) = 0;

    virtual int getMoving(int servo) = 0;

    virtual int getPos(int servo) = 0;

    virtual int ping(int servo) = 0;

    virtual void setMinPulse(int servo, int value) = 0;

    virtual void setMidPulse(int servo, int value) = 0;

    virtual void setMaxPulse(int servo, int value) = 0;

    virtual void setPolarity(int servo, bool inversed) = 0;

private:

protected:

};

#endif
