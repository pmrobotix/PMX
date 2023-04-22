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
#include "../Common/Action/ServoUsingMotor.hpp"

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


    ServoUsingMotor lanceurCerises_;

public:

    /*!
     * \brief Enumération des libellés des servos associés au numéro de servo
     * port-servo port1-4=1000-4000 servo=1-255 => 1000 à 4255
     * [num port] * 1000 + [num servo]
     */
    enum ServoAx12Label {
        AX12_SERVO_BRAS_D = 1 * 1000 + 5, //AX12

        AX12_SERVO_BRAS_G = 1000 + 7, //AX12

        AX12_SERVO_ASPIRATION = 1000 + 52, //AX12

        AX12_SERVO_FUNNY = 1000 + 51, //AX12

        AX12_SERVO_TETE_ASPI = 1000 + 50, //AX12

        AX18_SERVO_RUSSEL_LINKAGE = 1000 + 180, //AX18

        AX12_enumTypeEnd
    };

    /*!
     * \brief Enumération des libellés des servos STD associés au numéro de servo
     *  port=10000
     */
    enum ServoStdLabel {

        STD_SERVO_3 = 10000 * 5,

        STD_SERVO_4 = 10000 * 4,

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
     * \brief Cette methode retourne l'objet lanceur.
     * \return tirette_.
     */
    ServoUsingMotor& lanceur() {
        return lanceurCerises_;
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

        sensors_.stopTimerSensors(); //TODO rename stop
        ledbar_.stop();

        ledbar_.resetAll();
        lcd2x16_.reset();

        releaseAll();

    }

    //--------------------------------------------------------------
    //Actions 2023
    //--------------------------------------------------------------


    void ax12_init()
    {
        ax12_bras_droit();
        ax12_bras_gauche(-1);
        ax12_bras_droit_init();
        ax12_bras_gauche_init(-1);

        aspiration_closed_init();

        aspi_tete_init();
        aspi_centre(-1);

        funny_init(-1);

    }


    void turbine_aspiration(int activate) {
        tirette().setGPIO(4, activate);
    }

    //0 à 127
    void lancer_les_balles(int vitesse) {

        lanceur().turn(vitesse);
    }
    void stopper_lanceur_de_balles() {

        lanceur().stop();
    }

    void releaseAll() {

        servos().release(AX12_SERVO_BRAS_D);
        servos().release(AX12_SERVO_BRAS_G);

        servos().release(AX12_SERVO_ASPIRATION);
        servos().release(AX12_SERVO_FUNNY);
        servos().release(AX12_SERVO_TETE_ASPI);
        servos().release(AX18_SERVO_RUSSEL_LINKAGE);

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

    void ax12_bras_droit_init(int keep = 0, int speed = 1023) {
        servos().setSpeed(AX12_SERVO_BRAS_D, speed);
        servos().deploy(AX12_SERVO_BRAS_D, 815, keep);
    }
    void ax12_bras_droit(int keep = 0, int speed = 1023) {
        servos().setSpeed(AX12_SERVO_BRAS_D, speed);
        servos().deploy(AX12_SERVO_BRAS_D, 480, keep);
    }

    void ax12_bras_gauche_init(int keep = 0, int speed = 1023) {
        servos().setSpeed(AX12_SERVO_BRAS_G, speed);
        servos().deploy(AX12_SERVO_BRAS_G, 205, keep);
    }
    void ax12_bras_gauche(int keep = 0, int speed = 1023) {
        servos().setSpeed(AX12_SERVO_BRAS_G, speed);
        servos().deploy(AX12_SERVO_BRAS_G, 512, keep);
    }

    void aspiration_closed_init(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX12_SERVO_ASPIRATION, 512, keep, speed);

    }
    void aspiration_lacher_les_balles(int keep = -1, int speed = 200) {

        servos().setTorque(AX12_SERVO_ASPIRATION, 800);
        servos().deployWithVelocity(AX12_SERVO_ASPIRATION, 615, keep, speed);

    }

    void funny_init(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX12_SERVO_FUNNY, 512, keep, speed);

    }

    void funny_action_deploy(int keep = 1000, int speed = 1023) {

        servos().deployWithVelocity(AX12_SERVO_FUNNY, 680, keep, speed);

    }

    void aspi_tete_init(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX12_SERVO_TETE_ASPI, 512, keep, speed);

    }

    void aspi_tete_gauche(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX12_SERVO_TETE_ASPI, 430, keep, speed);
        //TODO torque 150
    }

    void aspi_tete_droite(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX12_SERVO_TETE_ASPI, 430, keep, speed);
        //TODO torque 150
    }

    void aspi_centre(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 450, keep, speed);

    }

    void aspi_droite(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 205, keep, speed);

    }
    void aspi_droite_full(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 105, keep, speed);

    }

    void aspi_gauche(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 688, keep, speed);

    }
    void aspi_gauche_full(int keep = 0, int speed = 150) {

        servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 788, keep, speed);

    }




    /*//2022
     void elevator2022_init(int keep = 0, int speed = 1024) {
     //        servos().setSpeed(AX12_SERVO_ELEVATOR_L, speed);
     //        servos().setSpeed(AX12_SERVO_ELEVATOR_R, speed);
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 459, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 562, keep, speed);
     }

     //deploy full avec bras devant
     void elevator2022_deploy(int keep = 0, int speed = 1024) {
     //        servos().setSpeed(AX12_SERVO_ELEVATOR_L, speed);
     //        servos().setSpeed(AX12_SERVO_ELEVATOR_R, speed);
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 766, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 254, keep, speed);
     }

     void elevator2022_niv1(int keep = 0, int speed = 1024) {
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 627, 0, speed);    //584
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 388, keep, speed);    //432
     }

     void elevator2022_niv2(int keep = 0, int speed = 1024) {

     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 660, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 348, keep, speed);
     }
     void elevator2022_niv3(int keep = 0, int speed = 1024) {

     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 734, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 274, keep, speed);
     }

     void arm_R_take(int keep = 0, int speed = 1024) {
     servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 463, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 266, keep, speed);
     }

     void arm_R_side_init(int keep = 0, int speed = 1024) {
     servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 575, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 141, keep, speed);
     }

     void arm_R_deploy1(int keep = 0, int speed = 1024) {

     servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 652, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 124, keep, speed);
     }
     void arm_R_deploy2(int keep = 0, int speed = 1024) {

     servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 780, -1, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 124, keep, speed);
     }

     void arm_R_deploy3(int keep = 0, int speed = 1024) {

     servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 635, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 380, keep, speed);

     }

     void arm_L_take(int keep = 0, int speed = 1024) {
     servos().deployWithVelocity(AX12_SERVO_ARM_L_TOP, 539, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_L_BOTTOM, 784, keep, speed);
     }

     void arm_L_side_init(int keep = 0, int speed = 1024) {
     servos().deployWithVelocity(AX12_SERVO_ARM_L_TOP, 445, 0, speed);
     servos().deployWithVelocity(AX12_SERVO_ARM_L_BOTTOM, 876, keep, speed);
     }

     void pump_R(int activate) {
     tirette().setGPIO(4, activate);
     }

     void pump_R_electrov(int activate) {
     tirette().setGPIO(5, activate);
     }

     void pump_L(int activate) {
     tirette().setGPIO(6, activate);
     }

     void pump_L_electrov(int activate) {
     tirette().setGPIO(7, activate);
     }
     */

    /*
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

     */
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

