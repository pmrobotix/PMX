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

    virtual void hold(int) = 0;

    virtual void setPosition(int servo, int pos) = 0;

    virtual void release(int servo) = 0;

    virtual void setRate(int servo, int speed) = 0;

    virtual void turn(int servo, int speed) = 0;

    virtual int getMoving(int servo) = 0;

    virtual int getPos(int servo) = 0;

    virtual int ping(int servo) = 0;

    //TODO
    //set_min_pulse_sp
    //set_mid_pulse_sp
    //set_max_pulse_sp


private:

protected:

};

#endif
