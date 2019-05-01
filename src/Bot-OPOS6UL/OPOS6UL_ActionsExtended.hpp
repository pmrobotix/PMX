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

    OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
            ledbar_(botId, *this, 8), buttonbar_(*this), lcd2x16_(botId, *this), tirette_(*this), sensors_(*this,
                    robot), servos_ax12_(*this, AServoDriver::SERVO_DYNAMIXEL), servos_std_(*this,
                    AServoDriver::SERVO_STANDARD)

    {
        lcd2x16_.init();
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

    void stop()
    {

        ledbar_.stop(true);
        ledbar_.resetAll();
        lcd2x16_.reset();

        releaseAll();

        //sensors stop...

        //stop all current task in the actionManagerTimer
        Actions::stop();
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
        //
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
        //
    }
    void ax12_right_cil(int keep = 0, int speed = 512)
    {
        servosAx12().setSpeed(181, speed);
        servosAx12().deploy(181, 792, keep);
    }
    /*
     void doorLeft(int keep = 0, int speed = 512)
     {
     servosStd().setSpeed(STD_SERVO_3, speed);
     servosStd().deploy(STD_SERVO_3, 512, keep);
     }
     */
    //--------------------------------------------------------------
    //Actions 2018
    //--------------------------------------------------------------
    /*void servo_lowspeed()
     {
     servoObjects().setSpeedAll(100);
     }

     void servo_mediumspeed()
     {
     servoObjects().setSpeedAll(200);
     }
     void servo_maxspeed()
     {
     servoObjects().setSpeedAll(1023);
     }
     void arm_right_retract(int keep = 0, int speed = 512)
     {
     servoObjects().setSpeed(7, speed);
     servoObjects().deploy(7, 512, keep);
     }*/
    /*
     void arm_right_full(int keep = 0, int speed = 100)
     {
     servoObjects().setSpeed(7, speed);
     servoObjects().deploy(7, 750, keep);
     }
     void arm_right_little(int keep = 0, int speed = 100)
     {
     servoObjects().setSpeed(7, speed);
     servoObjects().deploy(7, 600, keep);
     }

     void arm_left_retract(int keep = 0, int speed = 512)
     {
     servoObjects().setSpeed(5, speed);
     servoObjects().deploy(5, 512, keep);
     }
     void arm_left_full(int keep = 0, int speed = 100)
     {
     servoObjects().setSpeed(5, speed);
     servoObjects().deploy(5, 200, keep);
     }
     void arm_left_little(int keep = 0, int speed = 100)
     {
     servoObjects().setSpeed(5, speed);
     servoObjects().deploy(5, 420, keep);
     }

     void ball_front(int keep = 0, int speed = 100)
     {
     servoObjects().setSpeed(4, speed);
     servoObjects().deploy(4, 540, keep);
     }
     void ball_back(int keep = 0, int speed = 100)
     {
     servoObjects().setSpeed(4, speed);
     servoObjects().deploy(4, 740, keep);
     }

     void servo_init()
     {
     arm_right_retract();
     arm_left_retract();
     }

     void servo_init_end()
     {
     arm_left_full(0);
     arm_right_full(-1);

     arm_left_retract(0);
     arm_right_retract(-1);

     arm_left_full(0);
     arm_right_full(-1);

     arm_left_retract(0);
     arm_right_retract(-1);

     servoObjects().releaseAll();

     }
     void servo_init_orange()
     {
     arm_left_little(0);
     arm_right_little(-1);

     arm_left_retract(0);
     arm_right_retract(-1);

     arm_left_little(0);
     arm_right_little(-1);

     arm_left_retract(0);
     arm_right_retract(-1);

     servoObjects().releaseAll();
     }
     void servo_init_green()
     {
     arm_left_little(0);
     arm_right_little(-1);

     arm_left_retract(0);
     arm_right_retract(-1);

     arm_left_little(0);
     arm_right_little(-1);

     arm_left_retract(0);
     arm_right_retract(-1);

     servoObjects().releaseAll();
     }*/

};

#endif

