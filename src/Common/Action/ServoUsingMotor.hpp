#ifndef SERVOUSINGMOTOR_HPP_
#define SERVOUSINGMOTOR_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

class AServoUsingMotorDriver;

class ServoUsingMotor: public AActionsElement {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoUsingMotor.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoUsingMotor");
        return instance;
    }

    AServoUsingMotorDriver* servoMotordriver_;

    //int ticks_place;

    /*!
     * \brief ID du robot.
     */
    std::string botId_;

public:

    /*!
     * \brief Constructor.
     */
    ServoUsingMotor(std::string botId, Actions & actions);

    /*!
     * \brief Destructor.
     */
    ~ServoUsingMotor();

    bool is_connected();

    void turn(int speed);

    void stop();
//    void moveRight(int);
//    void moveLeft(int);
//    void ejectRight();
//    void ejectLeft();

};

#endif
