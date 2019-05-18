#ifndef SERVOUSINGMOTOR_HPP_
#define SERVOUSINGMOTOR_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

class AServoUsingMotorDriver;

class ServoUsingMotor: public AActionsElement
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoUsingMotor.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoUsingMotor");
        return instance;
    }

    AServoUsingMotorDriver* servoMotordriver_;

    int ticks_place;

public:

    /*!
     * \brief Constructor.
     */
    ServoUsingMotor(Actions & actions);

    /*!
     * \brief Destructor.
     */
    ~ServoUsingMotor();

    void moveRight(int);
    void moveLeft(int);
    void ejectRight();
    void ejectLeft();

};

#endif
