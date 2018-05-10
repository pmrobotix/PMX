#ifndef OPOS6UL_ACTIONSEXTENDED_HPP_
#define OPOS6UL_ACTIONSEXTENDED_HPP_

#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsAx12.hpp"
#include "../Common/Action/Tirette.hpp"

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
    ServoObjectsAx12 servoObjects_;

public:
    OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
            ledbar_(botId, *this, 8), buttonbar_(*this), lcd2x16_(botId, *this), tirette_(*this), sensors_(*this,
                    robot), servoObjects_(*this)

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

    ServoObjectsAx12 & servoObjects()
    {
        return servoObjects_;
    }

    void stop()
    {

        ledbar_.stop(true);
        ledbar_.resetAll();
        lcd2x16_.reset();
        servoObjects().releaseAll();

        //sensors stop...

        //stop all current task in the actionManagerTimer
        Actions::stop();
    }
    void servo_lowspeed()
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

    //--------------------------------------------------------------
    //Actions 2018
    //--------------------------------------------------------------

    void arm_right_retract(int keep = 0, int speed = 512)
    {
        servoObjects().setSpeed(7, speed);
        servoObjects().deploy(7, 512, keep);
    }
    void arm_right_full(int keep = 0, int speed = 100)
    {
        servoObjects().setSpeed(7, speed);
        servoObjects().deploy(7, 750, keep);
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

    }
    void servo_init_green()
    {

    }

};

#endif

