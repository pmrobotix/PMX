#ifndef OPOS6UL_ACTIONSEXTENDED_HPP_
#define OPOS6UL_ACTIONSEXTENDED_HPP_

#include <string>
#include <iostream>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Action.Driver/AServoDriver.hpp"

class OPOS6UL_ActionsExtended: public Actions {
private:

    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("OPOS6UL_ActionsExtended");
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
     * \brief objets avec servomotors ax12 et std.
     */
    ServoObjectsSystem servos_;

public:

    /*!
     * \brief Enumération des libellés des servos associés au numéro de servo
     */
    enum ServoAx12Label //TODO se servir de cette liste
    {
        AX12_SERVO_BRAS_D = 5, //AX12

        AX12_SERVO_BRAS_G = 7, //AX12

        AX12_SERVO_PINCE_SS_ROBOT = 4, //AX12

        AX12_SERVO_ASCENSEUR = 100, //MX28

        AX12_SERVO_ROTATION = 102, //MX28

        AX12_SERVO_PORTES_NOIRE = 61, //AX18

        AX12_SERVO_PORTES_BLEU = 63, //AX18

        AX12_SERVO_DRAPEAUX = 51, //AX12

        AX12_enumTypeEnd
    };

    enum ServoStdLabel {

        STD_SERVO_3 = 5,

        STD_SERVO_4,

        SERVO_enumTypeEnd
    };

    OPOS6UL_ActionsExtended(std::string botId, Robot *robot);

    ~OPOS6UL_ActionsExtended() {
    }

    /*!
     * \brief Cette methode retourne l'objet ledbar.
     * \return ledbar_.
     */
    LedBar& ledBar() {
        return ledbar_;
    }

    ButtonBar& buttonBar() {
        return buttonbar_;
    }

    /*!
     * \brief Cette methode retourne l'objet LcdShield.
     * \return lcd2x16_.
     */
    LcdShield& lcd2x16() {
        return lcd2x16_;
    }

    /*!
     * \brief Cette methode retourne l'objet tirette.
     * \return tirette_.
     */
    Tirette& tirette() {
        return tirette_;
    }

    /*!
     * \brief Cette methode retourne l'objet sensors.
     * \return sensors_.
     */
    Sensors& sensors() {
        return sensors_;
    }

    /*!
     * \brief Cette methode retourne l'objet servos.
     * \return servos_.
     */
    ServoObjectsSystem& servos() {
        return servos_;
    }

    void stopExtra() {
        logger().debug() << "stopExtra - ledbar" << logs::end;
        ledbar_.stop(true);
        sensors_.stopTimerSensors(); //TODO rename stop
        ledbar_.resetAll();
        lcd2x16_.reset();

        releaseAll();

    }

    //--------------------------------------------------------------
    //Actions 2021
    //--------------------------------------------------------------
    void releaseAll() {
//        servosAx12().release(51);
//        servosAx12().release(5);
//        servosAx12().release(7);
//        servosAx12().release(4);
//        servosAx12().release(100);
//        servosAx12().release(102);
//        servosAx12().release(61);
//        servosAx12().release(63);

//        for (int fooInt = 0; fooInt != AX12_enumTypeEnd; fooInt++) {
//            ServoAx12Label foo = static_cast<ServoAx12Label>(fooInt);
//            //servosAx12().setPosition(foo, 0);
//            servosAx12().release(foo);
//        }

//         for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
//         ServoStdLabel foo = static_cast<ServoStdLabel>(fooInt);
//         //servosStd().setPosition(foo, 0);
//         servosStd().release(foo);
//         }

//         for (int fooInt = 0; fooInt <= SERVO_enumTypeEnd; fooInt++) {
//          ServoStdLabel foo = static_cast<ServoStdLabel>(fooInt);
//          //servosStd().setPosition(foo, 0);
//          servosStd().release(foo);
//          }
    }

    void ax12_init() {

        //on monte le barillet
        ax12_elevator_up(true);
        //TODO fermer les portes

        //monter baisser le drapeau
        //ax12_drapeaux();
        ax12_drapeaux_init();

        //remonter les bras
        ax12_bras_droit_init();
        ax12_bras_gauche_init();
    }

    void ax12_drapeaux_init() {
//        servosAx12().setSpeed(AX12_SERVO_DRAPEAUX, 400);
//        servosAx12().deploy(AX12_SERVO_DRAPEAUX, 806, -1);
    }
    void ax12_drapeaux() {
//        servosAx12().setSpeed(AX12_SERVO_DRAPEAUX, 400);
//        servosAx12().deploy(AX12_SERVO_DRAPEAUX, 204, -1);
    }

    void ax12_bras_droit_init(int keep = 0, int speed = 1024) {
//        servosAx12().setSpeed(AX12_SERVO_BRAS_D, speed);
//        servosAx12().deploy(AX12_SERVO_BRAS_D, 815, keep);
    }
    void ax12_bras_droit(int keep = 0, int speed = 1024) {
//        servosAx12().setSpeed(AX12_SERVO_BRAS_D, speed);
//        servosAx12().deploy(AX12_SERVO_BRAS_D, 480, keep);
    }
    void ax12_bras_droit_fanion(int keep = 0, int speed = 1024) {
//        servosAx12().setSpeed(AX12_SERVO_BRAS_D, speed);
//        servosAx12().deploy(AX12_SERVO_BRAS_D, 396, keep);
    }

    void ax12_bras_gauche_init(int keep = 0, int speed = 1024) {
//        servosAx12().setSpeed(AX12_SERVO_BRAS_G, speed);
//        servosAx12().deploy(AX12_SERVO_BRAS_G, 205, keep);
    }
    void ax12_bras_gauche(int keep = 0, int speed = 1024) {
//        servosAx12().setSpeed(AX12_SERVO_BRAS_G, speed);
//        servosAx12().deploy(AX12_SERVO_BRAS_G, 512, keep);
    }
    void ax12_bras_gauche_fanion(int keep = 0, int speed = 1024) {
//        servosAx12().setSpeed(AX12_SERVO_BRAS_G, speed);
//        servosAx12().deploy(AX12_SERVO_BRAS_G, 598, keep);
    }

    void ax12_elevator_up(bool keep = true) {
//        int high = 3865;
//        servosAx12().setSpeed(AX12_SERVO_ASCENSEUR, 1023);
//        servosAx12().deploy(AX12_SERVO_ASCENSEUR, high, -1);
//        while (servosAx12().getPos(AX12_SERVO_ASCENSEUR) > (high - 5)) {
//            servosAx12().deploy(AX12_SERVO_ASCENSEUR, high, -1);
//        }
//        if (!keep) servosAx12().release(AX12_SERVO_ASCENSEUR);
    }
    void ax12_elevator_down(bool keep = true) {
//        servosAx12().setSpeed(AX12_SERVO_ASCENSEUR, 1000);
//        servosAx12().deploy(AX12_SERVO_ASCENSEUR, 553, -1);
//        if (!keep) servosAx12().release(AX12_SERVO_ASCENSEUR);
    }

    void ax12_rotation_black_side(bool keep = true) {
//        servosAx12().setSpeed(AX12_SERVO_ROTATION, 1000);
//        servosAx12().deploy(AX12_SERVO_ROTATION, 3905, -1);
//        if (!keep) servosAx12().release(AX12_SERVO_ROTATION);
    }

    void ax12_rotation_blue_side(bool keep = true) {
//        servosAx12().setSpeed(AX12_SERVO_ROTATION, 1000);
//        servosAx12().deploy(AX12_SERVO_ROTATION, 123, -1);
//        if (!keep) servosAx12().release(AX12_SERVO_ROTATION);
    }

    void ax12_pince_a_droite(bool keep = true) {
//        servosAx12().setSpeed(AX12_SERVO_PINCE_SS_ROBOT, 512);
//        servosAx12().deploy(AX12_SERVO_PINCE_SS_ROBOT, 889, -1);
//        if (!keep) servosAx12().release(AX12_SERVO_PINCE_SS_ROBOT);
    }

    void ax12_pince_a_gauche(bool keep = true) {
//        servosAx12().setSpeed(AX12_SERVO_PINCE_SS_ROBOT, 512);
//        servosAx12().deploy(AX12_SERVO_PINCE_SS_ROBOT, 108, -1);
//        if (!keep) servosAx12().release(AX12_SERVO_PINCE_SS_ROBOT);
    }

    void ax12_pince_au_centre(bool keep = true) {
//        servosAx12().setSpeed(AX12_SERVO_PINCE_SS_ROBOT, 512);
//        servosAx12().deploy(AX12_SERVO_PINCE_SS_ROBOT, 486, -1);
//        if (!keep) servosAx12().release(AX12_SERVO_PINCE_SS_ROBOT);
    }
    /*
     void ax12_init()
     {
     servosAx12().detect();

     ax12_leftHand();
     ax12_rightHand();
     ax12_left_cil_retract();
     ax12_right_cil_retract(-1);
     utils::sleep_for_micros(200000);
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
     //activation du capteur central
     sensors_.addConfigFront(true, true, true);

     servosAx12().setSpeed(180, speed);
     servosAx12().deploy(180, 512, keep);

     }

     void ax12_left_cil_retract_less(int keep = 0, int speed = 512)
     {
     //activation du capteur central
     sensors_.addConfigFront(true, true, true);

     servosAx12().setSpeed(180, speed);
     servosAx12().deploy(180, 350, keep);

     }

     void ax12_left_cil_retract_more(int keep = 0, int speed = 512)
     {
     //activation du capteur central
     sensors_.addConfigFront(true, true, true);

     servosAx12().setSpeed(180, speed);
     servosAx12().deploy(180, 572, keep);

     }
     void ax12_left_cil(int keep = 0, int speed = 512)
     {
     //desactivation du capteur central
     sensors_.addConfigFront(true, false, true);

     servosAx12().setSpeed(180, speed);
     servosAx12().deploy(180, 152, keep);    //232 142
     }

     void ax12_right_cil_retract(int keep = 0, int speed = 512)
     {
     servosAx12().setSpeed(181, speed);
     servosAx12().deploy(181, 512, keep);

     }

     void ax12_right_cil_retract_less(int keep = 0, int speed = 512)
     {
     servosAx12().setSpeed(181, speed);
     servosAx12().deploy(181, 650, keep);

     }
     void ax12_right_cil_retract_more(int keep = 0, int speed = 512)
     {
     servosAx12().setSpeed(181, speed);
     servosAx12().deploy(181, 462, keep);

     }
     void ax12_right_cil(int keep = 0, int speed = 512)
     {
     servosAx12().setSpeed(181, speed);
     servosAx12().deploy(181, 842, keep);    //792 852
     }

     bool ax12_goldenium_in_cil()
     {
     int leftT = servosAx12().getTorque(180);
     int RightT = servosAx12().getTorque(181);

     if (RightT > 900 && leftT < 300)
     return true;
     else
     return false;

     }*/

};

#endif

