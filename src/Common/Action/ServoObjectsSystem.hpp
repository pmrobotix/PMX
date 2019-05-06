#ifndef SERVOOBJECTSSYSTEM_HPP_
#define SERVOOBJECTSSYSTEM_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "AActionsElement.hpp"

class ServoObjectsSystem: public AActionsElement
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoObjectsSystem.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoObjectsSystem");
        return instance;
    }

    AServoDriver* servodriver_;

public:

    /*!
     * \brief Constructor.
     */
    ServoObjectsSystem(Actions & actions, AServoDriver::ServoType type);

    /*!
     * \brief Destructor.
     */
    ~ServoObjectsSystem();

    void deploy(int servo, int pos, int keep_millisec = -1);

    void turn(int servo, int speed, int keep_millisec = 0);
    void release(int servo);
    void hold(int servo);
    void setSpeed(int servo, int speed);
    void setMinPulse(int servo, int value);
    void setMidPulse(int servo, int value);
    void setMaxPulse(int servo, int value);
    void setPolarity(int servo, bool inversed = false);
    int getTorque(int servo);

    void detectAll();

};

#endif
