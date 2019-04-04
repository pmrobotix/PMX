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
        releaseAll();

        //Servo config specifique
        servosStd().setPolarity(0);
        servosStd().setMinPulse(0, 600);
        servosStd().setMidPulse(0, 1470);
        servosStd().setMaxPulse(0, 2350);

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

    void conveyorBelt_Push(int keep = 0, int speed = 512)
    {
        servosStd().setSpeed(SERVO_1, speed);
        servosStd().deploy(SERVO_1, 512, keep);
    }

};

#endif

