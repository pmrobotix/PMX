#ifndef LEGOEV3_ACTIONSEXTENDED_HPP_
#define LEGOEV3_ACTIONSEXTENDED_HPP_

#include <unistd.h>
#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/ServoUsingMotor.hpp"
#include "../Common/Action/SoundBar.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Enumération des libellés des servos.
 */
enum ServoLabel
{
    SERVO_1,

    SERVO_2,

    SERVO_3,

    SERVO_4,

    SERVO_5,

    SERVO_6,

    SERVO_7,

    SERVO_8,

    SERVO_enumTypeEnd
};

/*
 * Contains all specific robot element parts.
 */
class LegoEV3ActionsExtended: public Actions
{
private:

    /*!
     * \brief Return \ref Logger linked to \ref LegoEV3ActionsExtended.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3ActionsExtended");
        return instance;
    }

    /*!
     * \brief LED Bar.
     */
    LedBar ledbar_;

    /*!
     * \brief Button Bar.
     */
    ButtonBar buttonbar_;

    /*!
     * \brief Sound Bar.
     */
    SoundBar soundbar_;

    /*!
     * \brief Tirette.
     */
    Tirette tirette_;

    /*!
     * \brief capteurs IR/US.
     */
    Sensors sensors_;

    /*!
     * \brief capteurs IR/US.
     */
    LcdShield lcd_;

    /*!
     * \brief Servomotors standard.
     */
    ServoObjectsSystem servos_std_;

    ServoUsingMotor servoUsingMotor_;

public:
    LegoEV3ActionsExtended(std::string botId, Robot * robot);

    ~LegoEV3ActionsExtended()
    {
        //printf("~LegoEV3ActionsExtended()\n");
    }

    /*!
     * \brief Cette methode retourne l'objet ledbar.
     * \return ledbar_.
     */
    LedBar & ledBar()
    {
        return ledbar_;
    }

    /*!
     * \brief Cette methode retourne l'objet buttonBar.
     * \return buttonbar_.
     */
    ButtonBar & buttonBar()
    {
        return buttonbar_;
    }

    /*!
     * \brief Cette methode retourne l'objet soundBar.
     * \return soundbar_.
     */
    SoundBar & soundBar()
    {
        return soundbar_;
    }

    /*!
     * \brief Cette methode retourne l'objet tirette.
     * \return tirette_.
     */
    Tirette & tirette()
    {
        return tirette_;
    }

    /*!
     * \brief Cette methode retourne l'objet sensors.
     * \return sensors_.
     */
    Sensors & sensors()
    {
        return sensors_;
    }

    LcdShield & lcd()
    {
        return lcd_;
    }

    ServoObjectsSystem & servosStd()
    {
        return servos_std_;
    }

    ServoUsingMotor & servoUsingMotor()
    {
        return servoUsingMotor_;
    }

    void stop()
    {
        Actions::stop(); //stop devices and wait manager to finish

        ledbar_.resetAll();
        ledbar_.stop();
        soundbar_.stop();
        releaseAll();
    }

    void releaseAll()
    {
        for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
            ServoLabel foo = static_cast<ServoLabel>(fooInt);
            servosStd().deploy(foo, 0);
            servosStd().release(foo);
        }
    }

    //--------------------------------------------------------------
    //Actions 2019
    //--------------------------------------------------------------

    void right_eject_all()
    {
        logger().info() << "conveyorBelt_Left_low" << logs::end;
        conveyorBelt_Left_low();
        logger().info() << "conveyorBelt_EjectRight" << logs::end;
        conveyorBelt_EjectRight();
    }

    void left_eject_all()
    {
        logger().info() << "conveyorBelt_Right_low" << logs::end;
        conveyorBelt_Right_low();
        logger().info() << "conveyorBelt_EjectLeft" << logs::end;
        conveyorBelt_EjectLeft();
    }

    void left_prendre_palet()
    {
        logger().info() << "left_arm_take" << logs::end;
        left_arm_take();
        logger().info() << "left_arm_retract" << logs::end;
        left_arm_retract();
        //2nd time
        logger().info() << "left_arm_take" << logs::end;
        left_arm_take();
        logger().info() << "left_arm_retract" << logs::end;
        left_arm_retract();
        conveyorBelt_PushRight(1000);
    }
    void right_prendre_palet()
    {
        logger().info() << "right_arm_take" << logs::end;
        right_arm_take();

        logger().info() << "right_arm_retract" << logs::end;
        right_arm_retract();

        //2nd time
        logger().info() << "right_arm_take" << logs::end;
        right_arm_take();

        logger().info() << "right_arm_retract" << logs::end;
        right_arm_retract();

        conveyorBelt_PushLeft(1000);
    }

    void left_arm_center(int speed = 512)
    {
        servosStd().deploy(SERVO_7, 0, 0);
        usleep(1000000);
        servosStd().release(SERVO_7);
    }
    void left_arm_take()
    {
        servosStd().deploy(SERVO_7, -90, 0);
        usleep(1000000);
        servosStd().release(SERVO_7);
    }
    void left_arm_retract()
    {
        servosStd().deploy(SERVO_7, 90, 0);
        usleep(1000000);
        servosStd().release(SERVO_7);
    }

    void right_arm_center(int keep = 0)
    {
        servosStd().deploy(SERVO_5, 0, 0);
        usleep(1000000);
        servosStd().release(SERVO_5);
    }
    void right_arm_take(int keep = 0)
    {
        servosStd().deploy(SERVO_5, 90, 0);
        usleep(1000000);
        servosStd().release(SERVO_5);
    }
    void right_arm_retract()
    {
        servosStd().deploy(SERVO_5, -90, 0);
        usleep(1000000);
        servosStd().release(SERVO_5);
    }

    void conveyorBelt_Left_center()
    {
        servosStd().deploy(SERVO_1, 0, 0);
        usleep(2000000);
        servosStd().release(SERVO_1);
    }
    void conveyorBelt_Right_low(bool keep_activated = false)
    {
        servosStd().deploy(SERVO_1, -90, 0);
        usleep(2000000);
        if (!keep_activated)
            servosStd().release(SERVO_1);
    }

    void conveyorBelt_Left_low(bool keep_activated = false)
    {
        servosStd().deploy(SERVO_1, 90, 0);
        usleep(2000000);
        if (!keep_activated)
            servosStd().release(SERVO_1);
    }
    void conveyorBelt_PushRight(int nb_position)
    {
        servoUsingMotor().moveRight(nb_position);
        sleep(1);
    }

    void conveyorBelt_PushLeft(int nb_position)
    {
        servoUsingMotor().moveLeft(nb_position);
        sleep(1);
    }
    void conveyorBelt_EjectLeft()
    {
        servoUsingMotor().ejectLeft();
        sleep(1);
    }
    void conveyorBelt_EjectRight()
    {
        servoUsingMotor().ejectRight();
        sleep(1);
    }

};

#endif

