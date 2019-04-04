#ifndef LEGOEV3_ACTIONSEXTENDED_HPP_
#define LEGOEV3_ACTIONSEXTENDED_HPP_

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/SoundBar.hpp"
#include "../Common/Action/Tirette.hpp"

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
     * \brief Servomotors standard.
     */
    ServoObjectsSystem servos_std_;

public:
    LegoEV3ActionsExtended(std::string botId, Robot * robot) :
            ledbar_(botId, *this, 2), buttonbar_(*this), soundbar_(*this), tirette_(*this), sensors_(*this, robot), servos_std_(
                    *this, AServoDriver::SERVO_STANDARD)
    {
    }

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

    ServoObjectsSystem & servosStd()
    {
        return servos_std_;
    }

    void stop()
    {

        Actions::stop(); //stop devices and wait manager to finish

        ledbar_.resetAll();
        ledbar_.stop();
        soundbar_.stop();
        releaseAll();
    }

    //--------------------------------------------------------------
    //Actions 2019
    //--------------------------------------------------------------

    void releaseAll()
    {
        for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
            ServoLabel foo = static_cast<ServoLabel>(fooInt);
            servosStd().deploy(foo, 0);
            servosStd().release(foo);
        }
    }
    void leftHand(int keep = 0, int speed = 512)
    {
        servosStd().setSpeed(SERVO_1, speed);
        servosStd().deploy(SERVO_1, 512, keep);
    }

    /*
     //--------------------------------------------------------------
     //Actions 2017
     //--------------------------------------------------------------
     void funnyAction_Init()
     {
     servoObjects().deploy(SERVO_FUNNY_ACTION, 0);
     }

     void funnyAction_Activate()
     {
     servoObjects().deploy(SERVO_FUNNY_ACTION, -45);
     }

     void pince_HerculeDown()
     {
     servoObjects().deploy(SERVO_HERCULE, 80);
     }

     void pince_HerculeMiddle()
     {
     servoObjects().deploy(SERVO_HERCULE, 40);
     }

     void pince_HerculeUp()
     {
     servoObjects().deploy(SERVO_HERCULE, 0);
     }

     void pince_HerculeUp8cm(int keep_sec)
     {
     servoObjects().deploy(SERVO_HERCULE, 50, keep_sec);
     }

     void pince_InitRotation()
     {
     servoObjects().deploy(SERVO_BASCULE, -100, 1);
     }
     void pince_Rotate()
     {
     servoObjects().deploy(SERVO_BASCULE, 80, 2);
     }

     void pince_Open()
     {
     servoObjects().deploy(SERVO_PINCE, 0, 1); //ouverte
     }

     void pince_Close(int keep_sec)
     {
     servoObjects().deploy(SERVO_PINCE, 45, keep_sec); //fermée
     }
     */
};

#endif

