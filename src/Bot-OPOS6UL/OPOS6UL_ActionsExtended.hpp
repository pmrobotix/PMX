#ifndef OPOS6UL_ACTIONSEXTENDED_HPP_
#define OPOS6UL_ACTIONSEXTENDED_HPP_

#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Action.Driver/AServoDriver.hpp"

class OPOS6UL_ActionsExtended: public Actions
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
     * \brief LcdShield 2x16 characters.
     */
    LcdShield lcd2x16_;

    /*!
     * \brief la tirette du robot.
     */
    Tirette tirette_;

    /*!
     * \brief capteurs IR/US.
     */
    Sensors sensors_;

    /*!
     * \brief objets avec servomotors.
     */

    //new
    ServoObjectsSystem servos_ax12_;
    ServoObjectsSystem servos_std_;

public:

    /*!
     * \brief Enumération des libellés des servos associés au numéro de servo
     */
    enum ServoAx12Label
    {
        AX12_SERVO_1 = 5,

        AX12_SERVO_2 = 51,

        AX12_SERVO_4 = 1,

        AX12_enumTypeEnd
    };

    enum ServoStdLabel
    {

        STD_SERVO_3 = 5,

        STD_SERVO_4,

        SERVO_enumTypeEnd
    };

    OPOS6UL_ActionsExtended(std::string botId, Robot * robot);

    ~OPOS6UL_ActionsExtended()
    {
    }

    /*!
     * \brief Cette methode retourne l'objet ledbar.
     * \return ledbar_.
     */
    LedBar & ledBar()
    {
        return ledbar_;
    }

    ButtonBar & buttonBar()
    {
        return buttonbar_;
    }

    /*!
     * \brief Cette methode retourne l'objet LcdShield.
     * \return lcd2x16_.
     */
    LcdShield & lcd2x16()
    {
        return lcd2x16_;
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

    ServoObjectsSystem & servosAx12()
    {
        return servos_ax12_;
    }

    ServoObjectsSystem & servosStd()
    {
        return servos_std_;
    }

    void stopExtra()
    {

        ledbar_.stop(true);
        ledbar_.resetAll();
        lcd2x16_.reset();

        releaseAll();

        //TODO sensors stop...?

    }

    //--------------------------------------------------------------
    //Actions 2019
    //--------------------------------------------------------------
    void releaseAll()
    {
        for (int fooInt = 0; fooInt != AX12_enumTypeEnd; fooInt++) {
            ServoAx12Label foo = static_cast<ServoAx12Label>(fooInt);
            //servosAx12().setPosition(foo, 0);
            servosAx12().release(foo);
        }

        for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
            ServoStdLabel foo = static_cast<ServoStdLabel>(fooInt);
            //servosStd().setPosition(foo, 0);
            servosStd().release(foo);
        }

    }

    void ax12_init()
    {
        servosAx12().detect();

        ax12_leftHand();
        ax12_rightHand();
        ax12_left_cil_retract();
        ax12_right_cil_retract(-1);
        usleep(200000);
        ax12_leftHand_retract();
        ax12_rightHand_retract();
        ax12_left_cil();
        ax12_right_cil(-1);

        ax12_left_cil_retract();
        ax12_right_cil_retract();
    }

    void ax12_rightHand_retract(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(7, speed);
        servosAx12().deploy(7, 512, keep);
    }

    void ax12_rightHand(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(7, speed);
        servosAx12().deploy(7, 732, keep);
    }

    void ax12_leftHand_retract(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(5, speed);
        servosAx12().deploy(5, 512, keep);
    }
    void ax12_leftHand(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(5, speed);
        servosAx12().deploy(5, 300, keep);
    }

    void ax12_left_cil_release(int keep = 0, int speed = 512)
    {
        servosAx12().release(180);
        servosAx12().release(181);
    }

    void ax12_left_cil_retract(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(180, speed);
        servosAx12().deploy(180, 512, keep);

    }
    void ax12_left_cil(int keep = 0, int speed = 512)
    {

        servosAx12().setSpeed(180, speed);
        servosAx12().deploy(180, 232, keep);
    }

    void ax12_right_cil_retract(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(181, speed);
        servosAx12().deploy(181, 512, keep);

    }
    void ax12_right_cil(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(181, speed);
        servosAx12().deploy(181, 792, keep);
    }

    bool ax12_goldenium_in_cil()
    {
        int leftT = servosAx12().getTorque(180);
        int RightT = servosAx12().getTorque(181);

        if (RightT > 900 && leftT < 300)
            return true;
        else
            return false;

    }

};

#endif

